/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 * Copyright (c) 2020 XiaoMi, Inc. All rights reserved.
 */

#define pr_fmt(fmt) "mi_sde_connector:[%s:%d] " fmt, __func__, __LINE__

#include <drm/sde_drm.h>
#include "msm_drv.h"
#include "sde_connector.h"
#include "sde_encoder.h"
#include "sde_trace.h"
#include "dsi_display.h"
#include "dsi_panel.h"

#include "mi_disp_print.h"
#include "mi_disp_feature.h"
#include "mi_dsi_panel.h"
#include "mi_dsi_display.h"
#include "mi_sde_connector.h"
#include "mi_disp_lhbm.h"
#include "mi_panel_id.h"

static irqreturn_t mi_esd_err_irq_handle(int irq, void *data)
{
	struct sde_connector *c_conn = (struct sde_connector *)data;
	struct dsi_display *display = c_conn ? (c_conn->display) : NULL;
	struct drm_event event;
	int power_mode;

	if (!display || !display->panel) {
		DISP_ERROR("invalid display/panel\n");
		return IRQ_HANDLED;
	}

	DISP_INFO("%s display esd irq trigging \n", display->display_type);

	if (display->panel->mi_cfg.ignore_esd_in_aod) {
		power_mode = display->panel->power_mode;
		if(power_mode == SDE_MODE_DPMS_LP1 ||
			power_mode == SDE_MODE_DPMS_LP2) {
			DISP_INFO("%s don't report panel_dead in aod\n", display->display_type);
			dsi_panel_acquire_panel_lock(display->panel);
			mi_dsi_panel_esd_irq_ctrl_locked(display->panel, false);
			dsi_panel_release_panel_lock(display->panel);
			return IRQ_HANDLED;
		}
	}

	if (c_conn->connector_type == DRM_MODE_CONNECTOR_DSI) {
		dsi_panel_acquire_panel_lock(display->panel);
		mi_dsi_panel_esd_irq_ctrl_locked(display->panel, false);

		if (!dsi_panel_initialized(display->panel)) {
			DISP_ERROR("%s display panel not initialized!\n",
					display->display_type);
			dsi_panel_release_panel_lock(display->panel);
			return IRQ_HANDLED;
		}

		if (atomic_read(&(display->panel->esd_recovery_pending))) {
			DISP_INFO("%s display ESD recovery already pending\n",
					display->display_type);
			dsi_panel_release_panel_lock(display->panel);
			return IRQ_HANDLED;
		}

		if (!c_conn->panel_dead) {
			atomic_set(&display->panel->esd_recovery_pending, 1);
		} else {
			DISP_INFO("%s display already notify PANEL_DEAD\n",
					display->display_type);
			dsi_panel_release_panel_lock(display->panel);
			return IRQ_HANDLED;
		}

		power_mode = display->panel->power_mode;
		DISP_INFO("%s display, power_mode (%s)\n", display->display_type,
			get_display_power_mode_name(power_mode));

		dsi_panel_release_panel_lock(display->panel);

		if (power_mode == SDE_MODE_DPMS_ON ||
			power_mode == SDE_MODE_DPMS_LP1) {
			_sde_connector_report_panel_dead(c_conn, false);
		} else {
			c_conn->panel_dead = true;
			event.type = DRM_EVENT_PANEL_DEAD;
			event.length = sizeof(bool);
			msm_mode_object_event_notify(&c_conn->base.base,
				c_conn->base.dev, &event, (u8 *)&c_conn->panel_dead);
			SDE_EVT32(SDE_EVTLOG_ERROR);
			DISP_ERROR("%s display esd irq check failed report"
				" PANEL_DEAD conn_id: %d enc_id: %d\n",
				display->display_type,
				c_conn->base.base.id, c_conn->encoder->base.id);
		}
	}

	return IRQ_HANDLED;

}


int mi_sde_connector_register_esd_irq(struct sde_connector *c_conn)
{
	struct dsi_display *display = c_conn ? (c_conn->display) : NULL;
	int rc = 0;

	/* register esd irq and enable it after panel enabled */
	if (c_conn->connector_type == DRM_MODE_CONNECTOR_DSI) {
		if (!display || !display->panel) {
			DISP_ERROR("invalid display/panel\n");
			return -EINVAL;
		}
		if (display->panel->mi_cfg.esd_err_irq_gpio > 0) {
			rc = request_threaded_irq(display->panel->mi_cfg.esd_err_irq,
				NULL, mi_esd_err_irq_handle,
				display->panel->mi_cfg.esd_err_irq_flags,
				"esd_err_irq", c_conn);
			if (rc) {
				DISP_ERROR("%s display register esd irq failed\n",
					display->display_type);
			} else {
				DISP_INFO("%s display register esd irq success\n",
					display->display_type);
				disable_irq(display->panel->mi_cfg.esd_err_irq);
			}
		}

		if (mi_get_panel_id(display->panel->mi_cfg.mi_panel_id) == N81A_PANEL_PA) {
			if (display->panel->mi_cfg.esd_err_irq_gpio_second > 0) {
				rc = request_threaded_irq(display->panel->mi_cfg.esd_err_irq_second,
					NULL, mi_esd_err_irq_handle,
					display->panel->mi_cfg.esd_err_irq_flags_second,
					"esd_err_irq_second", c_conn);
				if (rc) {
					DISP_ERROR("%s display register esd irq second failed\n",
						display->display_type);
				} else {
					DISP_INFO("%s display register esd irq second success\n",
						display->display_type);
					disable_irq(display->panel->mi_cfg.esd_err_irq_second);
				}
			}
		}
	}

	return rc;
}

int mi_sde_connector_debugfs_esd_sw_trigger(void *display)
{
	struct dsi_display *dsi_display = (struct dsi_display *)display;
	struct drm_connector *connector = NULL;
	struct sde_connector *c_conn = NULL;
	struct drm_event event;
	struct dsi_panel *panel;
	int power_mode;

	if (!dsi_display || !dsi_display->panel || !dsi_display->drm_conn) {
		DISP_ERROR("invalid display/panel/drm_conn ptr\n");
		return -EINVAL;
	}

	panel = dsi_display->panel;

	connector = dsi_display->drm_conn;
	c_conn = to_sde_connector(connector);
	if (!c_conn) {
		DISP_ERROR("invalid sde_connector ptr\n");
		return -EINVAL;
	}

	if (!dsi_panel_initialized(dsi_display->panel)) {
		DISP_ERROR("Panel not initialized\n");
		return -EINVAL;
	}

	if (atomic_read(&(dsi_display->panel->esd_recovery_pending))) {
		DISP_INFO("[esd-test]ESD recovery already pending\n");
		return 0;
	}

	if (c_conn->panel_dead) {
		DISP_INFO("panel_dead is true, return!\n");
		return 0;
	}

	dsi_panel_acquire_panel_lock(panel);
	atomic_set(&dsi_display->panel->esd_recovery_pending, 1);
	dsi_panel_release_panel_lock(panel);

	c_conn->panel_dead = true;
	DISP_ERROR("[esd-test]esd irq check failed report PANEL_DEAD conn_id: %d enc_id: %d\n",
			c_conn->base.base.id, c_conn->encoder->base.id);

	power_mode = dsi_display->panel->power_mode;
	DISP_INFO("[esd-test]%s display enabled (%d), power_mode (%s)\n", dsi_display->display_type,
		dsi_display->enabled, get_display_power_mode_name(power_mode));
	if (dsi_display->enabled) {
		if (power_mode == SDE_MODE_DPMS_ON || power_mode == SDE_MODE_DPMS_LP1) {
			sde_encoder_display_failure_notification(c_conn->encoder, false);
		}
	}

	event.type = DRM_EVENT_PANEL_DEAD;
	event.length = sizeof(bool);
	msm_mode_object_event_notify(&c_conn->base.base,
			c_conn->base.dev, &event, (u8 *)&c_conn->panel_dead);

	return 0;
}

int mi_sde_connector_check_layer_flags(struct drm_connector *connector)
{
        int ret = 0;
	struct sde_connector *c_conn;
	struct dsi_display *display = NULL;
	u32 value;
	struct mi_layer_flags flags;

	if (!connector) {
		DISP_ERROR("invalid connector ptr\n");
		return -EINVAL;
	}

	if (connector->connector_type == DRM_MODE_CONNECTOR_DSI) {
		c_conn = to_sde_connector(connector);

		display = (struct dsi_display *)c_conn->display;
		if (!display) {
			DISP_ERROR("invalid display ptr\n");
			return -EINVAL;
		}

		value = sde_connector_get_property(connector->state, CONNECTOR_PROP_MI_LAYER_INFO);
		memcpy(&flags, &value, sizeof(u32));
		display->panel->mi_cfg.aod_layer_remove  = flags.aod_layer_remove;

		if (flags.gxzw_anim_changed) {
			DISP_INFO("layer gxzw_anim = %d\n", flags.gxzw_anim_present);
			if (display && mi_disp_lhbm_fod_enabled(display->panel)) {
				ret = mi_disp_lhbm_aod_to_normal_optimize(display, flags.gxzw_anim_present);
				if (ret == -EAGAIN) {
					display->panel->mi_cfg.aod_to_normal_pending = true;
				} else {
					display->panel->mi_cfg.aod_to_normal_pending = false;
				}
			}
                        display->panel->mi_cfg.lhbm_gxzw= flags.gxzw_anim_present;
			mi_disp_update_0size_lhbm_layer(display, flags.gxzw_anim_present);
		}
		if (flags.aod_changed) {
			DISP_INFO("layer aod = %d\n", flags.aod_present);

			if (mi_get_panel_id(display->panel->mi_cfg.mi_panel_id) == M18_PANEL_PA ||
				mi_get_panel_id(display->panel->mi_cfg.mi_panel_id) == M18_PANEL_SA) {
				if (!flags.aod_present &&
					(c_conn->lp_mode == SDE_MODE_DPMS_LP1 ||
					c_conn->lp_mode == SDE_MODE_DPMS_LP2)) {
					DISP_INFO("set nolp when home layer comming\n");
					dsi_panel_set_nolp(display->panel);
				}
			}
		}
	}

	return 0;
}

int mi_sde_connector_flat_fence(struct drm_connector *connector)
{
	int rc = 0;
	struct sde_connector *c_conn;
	struct dsi_display *dsi_display;
	struct mi_dsi_panel_cfg *mi_cfg;
	int flat_mode_val = FEATURE_OFF;

	if (!connector) {
		DISP_ERROR("invalid connector ptr\n");
		return -EINVAL;
	}

	c_conn = to_sde_connector(connector);

	if (c_conn->connector_type != DRM_MODE_CONNECTOR_DSI)
		return 0;

	dsi_display = (struct dsi_display *) c_conn->display;
	if (!dsi_display || !dsi_display->panel) {
		DISP_ERROR("invalid display/panel ptr\n");
		return -EINVAL;
	}

	if (mi_get_disp_id(dsi_display->display_type) != MI_DISP_PRIMARY)
		return -EINVAL;

	mi_cfg = &dsi_display->panel->mi_cfg;

	if (mi_cfg->flat_sync_te) {
		dsi_panel_acquire_panel_lock(dsi_display->panel);
		flat_mode_val = mi_cfg->feature_val[DISP_FEATURE_FLAT_MODE];
		if (flat_mode_val != mi_cfg->flat_cfg.cur_flat_state) {
			if (flat_mode_val == FEATURE_ON) {
				dsi_panel_tx_cmd_set(dsi_display->panel, DSI_CMD_SET_MI_FLAT_MODE_ON);
				sde_encoder_wait_for_event(c_conn->encoder,MSM_ENC_VBLANK);
				dsi_panel_tx_cmd_set(dsi_display->panel, DSI_CMD_SET_MI_FLAT_MODE_SEC_ON);
				DISP_INFO("DSI_CMD_SET_MI_FLAT_MODE_ON sync with te");
			} else {
				dsi_panel_tx_cmd_set(dsi_display->panel, DSI_CMD_SET_MI_FLAT_MODE_OFF);
				sde_encoder_wait_for_event(c_conn->encoder,MSM_ENC_VBLANK);
				dsi_panel_tx_cmd_set(dsi_display->panel, DSI_CMD_SET_MI_FLAT_MODE_SEC_OFF);
				DISP_INFO("DSI_CMD_SET_MI_FLAT_MODE_OFF sync with te");
			}
			mi_disp_feature_event_notify_by_type(mi_get_disp_id(dsi_display->display_type),
				MI_DISP_EVENT_FLAT_MODE, sizeof(flat_mode_val), flat_mode_val);
			mi_cfg->flat_cfg.cur_flat_state = flat_mode_val;
		}
		dsi_panel_release_panel_lock(dsi_display->panel);
	}

	return rc;
}

