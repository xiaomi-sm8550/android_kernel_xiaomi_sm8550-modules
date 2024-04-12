/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 * Copyright (c) 2020 XiaoMi, Inc. All rights reserved.
 */

#define pr_fmt(fmt)	"mi-disp-parse:[%s:%d] " fmt, __func__, __LINE__
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include "mi_disp_print.h"
#include "dsi_panel.h"
#include "dsi_parser.h"
#include "mi_panel_id.h"
#include <linux/soc/qcom/smem.h>

#define DEFAULT_MAX_BRIGHTNESS_CLONE 4095
#define SMEM_SW_DISPLAY_LHBM_TABLE 498
#define SMEM_SW_DISPLAY_GRAY_SCALE_TABLE 499
#define SMEM_SW_DISPLAY_LOCKDOWN_TABLE 500

int mi_dsi_panel_parse_esd_gpio_config(struct dsi_panel *panel)
{
	int rc = 0;
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	mi_cfg->esd_err_irq_gpio = of_get_named_gpio_flags(
			utils->data, "mi,esd-err-irq-gpio",
			0, (enum of_gpio_flags *)&(mi_cfg->esd_err_irq_flags));
	if (gpio_is_valid(mi_cfg->esd_err_irq_gpio)) {
		mi_cfg->esd_err_irq = gpio_to_irq(mi_cfg->esd_err_irq_gpio);
		rc = gpio_request(mi_cfg->esd_err_irq_gpio, "esd_err_irq_gpio");
		if (rc)
			DISP_ERROR("Failed to request esd irq gpio %d, rc=%d\n",
				mi_cfg->esd_err_irq_gpio, rc);
		else
			gpio_direction_input(mi_cfg->esd_err_irq_gpio);
	} else {
		rc = -EINVAL;
	}

	if(!strcmp(panel->name,"xiaomi n81a 36 02 0a vid mode duledsi dsc panel")) {
		mi_cfg->esd_err_irq_gpio_second = of_get_named_gpio_flags(
			utils->data, "mi,esd-err-irq-gpio-second",
			0, (enum of_gpio_flags *)&(mi_cfg->esd_err_irq_flags_second));
		if (gpio_is_valid(mi_cfg->esd_err_irq_gpio_second)) {
			mi_cfg->esd_err_irq_second = gpio_to_irq(mi_cfg->esd_err_irq_gpio_second);
			rc = gpio_request(mi_cfg->esd_err_irq_gpio_second, "esd_err_irq_gpio_second");
			if (rc)
				DISP_ERROR("Failed to request esd irq gpio second %d, rc=%d\n",
					mi_cfg->esd_err_irq_gpio_second, rc);
			else
				gpio_direction_input(mi_cfg->esd_err_irq_gpio_second);
		} else {
			rc = -EINVAL;
		}
	}

	return rc;
}

static void mi_dsi_panel_parse_round_corner_config(struct dsi_panel *panel)
{
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	mi_cfg->ddic_round_corner_enabled =
			utils->read_bool(utils->data, "mi,ddic-round-corner-enabled");
	if (mi_cfg->ddic_round_corner_enabled)
		DISP_INFO("mi,ddic-round-corner-enabled is defined\n");
}

static void mi_dsi_panel_parse_lhbm_config(struct dsi_panel *panel)
{
	int rc = 0;
	int i  = 0, tmp = 0;
	size_t item_size;
	void *lhbm_ptr = NULL;

	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	mi_cfg->local_hbm_enabled =
			utils->read_bool(utils->data, "mi,local-hbm-enabled");
	if (mi_cfg->local_hbm_enabled)
		DISP_INFO("local hbm feature enabled\n");

	rc = utils->read_u32(utils->data, "mi,local-hbm-ui-ready-delay-num-frame",
			&mi_cfg->lhbm_ui_ready_delay_frame);
	if (rc)
		mi_cfg->lhbm_ui_ready_delay_frame = 0;
	DISP_INFO("local hbm ui_ready delay %d frame\n",
			mi_cfg->lhbm_ui_ready_delay_frame);

	mi_cfg->need_fod_animal_in_normal =
			utils->read_bool(utils->data, "mi,need-fod-animal-in-normal-enabled");
	if (mi_cfg->need_fod_animal_in_normal)
		DISP_INFO("need fod animal in normal enabled\n");


	mi_cfg->lhbm_g500_update_flag =
			utils->read_bool(utils->data, "mi,local-hbm-green-500nit-update-flag");
	if (mi_cfg->lhbm_g500_update_flag)
		DISP_INFO("mi,local-hbm-green-500nit-update-flag\n");

	mi_cfg->lhbm_w1000_update_flag =
			utils->read_bool(utils->data, "mi,local-hbm-white-1000nit-update-flag");
	if (mi_cfg->lhbm_w1000_update_flag)
		DISP_INFO("mi,local-hbm-white-1000nit-update-flag\n");

	mi_cfg->lhbm_w110_update_flag =
			utils->read_bool(utils->data, "mi,local-hbm-white-110nit-update-flag");
	if (mi_cfg->lhbm_w110_update_flag)
		DISP_INFO("mi,local-hbm-white-110nit-update-flag\n");

	mi_cfg->lhbm_alpha_ctrlaa =
			utils->read_bool(utils->data, "mi,local-hbm-alpha-ctrl-aa-area");
	if (mi_cfg->lhbm_alpha_ctrlaa)
		DISP_INFO("mi,local-hbm-alpha-ctrl-aa-area\n");

	mi_cfg->lhbm_ctrl_df_reg =
			utils->read_bool(utils->data, "mi,local-hbm-ctrl-df-reg");
	if (mi_cfg->lhbm_ctrl_df_reg)
		DISP_INFO("mi,local-hbm-ctrl-df-reg\n");

	if ((mi_get_panel_id(panel->mi_cfg.mi_panel_id) == M11_PANEL_PA) ||
		(mi_get_panel_id(panel->mi_cfg.mi_panel_id) == N11_PANEL_PA)) {
		lhbm_ptr = qcom_smem_get(QCOM_SMEM_HOST_ANY, SMEM_SW_DISPLAY_LHBM_TABLE, &item_size);
		if (!IS_ERR(lhbm_ptr) && item_size > 0) {
			DSI_INFO("lhbm data size %d\n", item_size);
			memcpy(mi_cfg->lhbm_rgb_param, lhbm_ptr, item_size);
			for (i = 1; i < item_size; i += 2) {
				tmp = ((mi_cfg->lhbm_rgb_param[i-1]) << 8) | mi_cfg->lhbm_rgb_param[i];
				DSI_INFO("lhbm_rgb_param index %d = 0x%04X\n", i, tmp);
				if (tmp == 0x0000) {
					DSI_INFO("uefi read lhbm data failed, need kernel read!\n");
					mi_cfg->uefi_read_lhbm_success = false;
					break;
				} else {
					mi_cfg->uefi_read_lhbm_success = true;
				}
			}
		}
	} else if (mi_get_panel_id(panel->mi_cfg.mi_panel_id) == M1_PANEL_PA) {
		lhbm_ptr = qcom_smem_get(QCOM_SMEM_HOST_ANY, SMEM_SW_DISPLAY_LHBM_TABLE, &item_size);
		if (!IS_ERR(lhbm_ptr) && item_size > 0) {
			DSI_INFO("lhbm data size %d\n", item_size);
			memcpy(mi_cfg->lhbm_param, lhbm_ptr, item_size);
			for (i = 1; i < item_size; i += 2) {
				tmp = ((mi_cfg->lhbm_param[i-1]) << 8) | mi_cfg->lhbm_param[i];
				DSI_INFO("lhbm index %d = 0x%04X\n", i, tmp);
				if (tmp == 0x0000) {
					DSI_INFO("uefi read lhbm data failed, use default param!\n");
					mi_cfg->read_lhbm_gamma_success = false;
					break;
				} else {
					mi_cfg->read_lhbm_gamma_success = true;
				}
			}
		}
	}
}

static void mi_dsi_panel_parse_lockdown_config(struct dsi_panel *panel)
{
	size_t item_size;
	void *lockdown_ptr = NULL;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	int i =0;
	DISP_INFO("lockdown kernel  debug start !! \n");
	if (mi_get_panel_id(panel->mi_cfg.mi_panel_id) == N81A_PANEL_PA) {
		DISP_INFO("N81A product lockdown kernel get !! \n");
		lockdown_ptr = qcom_smem_get(QCOM_SMEM_HOST_ANY, SMEM_SW_DISPLAY_LOCKDOWN_TABLE, &item_size);
		if (!IS_ERR(lockdown_ptr) && item_size > 0) {
			DISP_INFO("N81A lockdown data size= %d\n",item_size);
			memcpy(mi_cfg->lockdown_cfg.lockdown_param, lockdown_ptr, item_size);
		}
		for (i=0; i<8 ; i++)
		{
			DISP_INFO("N81A lockdown data mi_cfg->lockdown_cfg.lockdown_param[%d] = 0x%0x\n",i, mi_cfg->lockdown_cfg.lockdown_param[i]);
		}
	}
}

static void mi_dsi_panel_parse_flat_config(struct dsi_panel *panel)
{
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	mi_cfg->flat_sync_te = utils->read_bool(utils->data, "mi,flat-need-sync-te");
	if (mi_cfg->flat_sync_te)
		DISP_INFO("mi,flat-need-sync-te is defined\n");
	else
		DISP_DEBUG("mi,flat-need-sync-te is undefined\n");

	mi_cfg->flatmode_default_on_enabled = utils->read_bool(utils->data,
				"mi,flatmode-default-on-enabled");
	if (mi_cfg->flatmode_default_on_enabled)
		DISP_INFO("flat mode is  default enabled\n");
#ifdef DISPLAY_FACTORY_BUILD
	mi_cfg->flat_sync_te = false;
#endif

	mi_cfg->flat_update_flag = utils->read_bool(utils->data, "mi,flat-update-flag");
	if (mi_cfg->flat_update_flag) {
		DISP_INFO("mi,flat-update-flag is defined\n");
	} else {
		DISP_DEBUG("mi,flat-update-flag is undefined\n");
	}
}

static int mi_dsi_panel_parse_dc_config(struct dsi_panel *panel)
{
	int rc = 0;
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;
	const char *string;

	mi_cfg->dc_feature_enable = utils->read_bool(utils->data, "mi,dc-feature-enabled");
	if (!mi_cfg->dc_feature_enable) {
		DISP_DEBUG("mi,dc-feature-enabled not defined\n");
		return rc;
	}
	DISP_INFO("mi,dc-feature-enabled is defined\n");

	rc = utils->read_string(utils->data, "mi,dc-feature-type", &string);
	if (rc){
		DISP_ERROR("mi,dc-feature-type not defined!\n");
		return -EINVAL;
	}
	if (!strcmp(string, "lut_compatible_backlight")) {
		mi_cfg->dc_type = TYPE_LUT_COMPATIBLE_BL;
	} else if (!strcmp(string, "crc_skip_backlight")) {
		mi_cfg->dc_type = TYPE_CRC_SKIP_BL;
	} else {
		DISP_ERROR("No valid mi,dc-feature-type string\n");
		return -EINVAL;
	}
	DISP_INFO("mi, dc type is %s\n", string);

	mi_cfg->dc_update_flag = utils->read_bool(utils->data, "mi,dc-update-flag");
	if (mi_cfg->dc_update_flag) {
		DISP_INFO("mi,dc-update-flag is defined\n");
	} else {
		DISP_DEBUG("mi,dc-update-flag not defined\n");
	}

	rc = utils->read_u32(utils->data, "mi,mdss-dsi-panel-dc-threshold", &mi_cfg->dc_threshold);
	if (rc) {
		mi_cfg->dc_threshold = 440;
		DISP_INFO("default dc threshold is %d\n", mi_cfg->dc_threshold);
	} else {
		DISP_INFO("dc threshold is %d \n", mi_cfg->dc_threshold);
	}

	return rc;
}

void mi_dsi_panel_parse_multi_timing_config(struct dsi_panel *panel)
{
	struct dsi_parser_utils *utils = &panel->utils;

	panel->mi_cfg.multi_timing_enable = utils->read_bool(utils->data, "mi,video-mode-multi-timing");
	DISP_INFO("mi-video-mode-multi-timing: %d \n", panel->mi_cfg.multi_timing_enable);
}

static int mi_dsi_panel_parse_backlight_config(struct dsi_panel *panel)
{
	int rc = 0;
#ifdef DISPLAY_FACTORY_BUILD
	u32 val = 0;
#endif
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	rc = utils->read_u32(utils->data, "mi,panel-on-dimming-delay", &mi_cfg->panel_on_dimming_delay);
	if (rc) {
		mi_cfg->panel_on_dimming_delay = 0;
		DISP_INFO("mi,panel-on-dimming-delay not specified\n");
	} else {
		DISP_INFO("mi,panel-on-dimming-delay is %d\n", mi_cfg->panel_on_dimming_delay);
	}

	mi_cfg->dimming_need_update_speed = utils->read_bool(utils->data,
					"mi,dimming-need-update-speed");
	if (mi_cfg->dimming_need_update_speed) {
		DISP_INFO("mi,dimming-need-update-speed is defined\n");
	} else {
		DISP_INFO("mi,dimming-need-update-speed is undefined\n");
	}

	rc = utils->read_u32_array(utils->data, "mi,dimming-node",
			mi_cfg->dimming_node, 5);
	if (rc) {
		DISP_INFO("mi,dimming-node is undefined\n");
	} else {
		DISP_INFO("mi,dimming-node is %d,%d,%d,%d,%d\n", mi_cfg->dimming_node[0],
				mi_cfg->dimming_node[1], mi_cfg->dimming_node[2],
				mi_cfg->dimming_node[3], mi_cfg->dimming_node[4]);
	}

	rc = utils->read_u32(utils->data, "mi,doze-hbm-dbv-level", &mi_cfg->doze_hbm_dbv_level);
	if (rc) {
		mi_cfg->doze_hbm_dbv_level = 0;
		DISP_INFO("mi,doze-hbm-dbv-level not specified\n");
	} else {
		DISP_INFO("mi,doze-hbm-dbv-level is %d\n", mi_cfg->doze_hbm_dbv_level);
	}

	rc = utils->read_u32(utils->data, "mi,doze-lbm-dbv-level", &mi_cfg->doze_lbm_dbv_level);
	if (rc) {
		mi_cfg->doze_lbm_dbv_level = 0;
		DISP_INFO("mi,doze-lbm-dbv-level not specified\n");
	} else {
		DISP_INFO("mi,doze-lbm-dbv-level is %d\n", mi_cfg->doze_lbm_dbv_level);
	}

	rc = utils->read_u32(utils->data, "mi,max-brightness-clone", &mi_cfg->max_brightness_clone);
	if (rc) {
		mi_cfg->max_brightness_clone = DEFAULT_MAX_BRIGHTNESS_CLONE;
	}
	DISP_INFO("max_brightness_clone=%d\n", mi_cfg->max_brightness_clone);

	rc = utils->read_u32(utils->data, "mi,normal-max-brightness-clone", &mi_cfg->normal_max_brightness_clone);
	if (rc) {
		mi_cfg->normal_max_brightness_clone = DEFAULT_MAX_BRIGHTNESS_CLONE;
	}
	DISP_INFO("normal_max_brightness_clone=%d\n", mi_cfg->normal_max_brightness_clone);

	mi_cfg->thermal_dimming_enabled = utils->read_bool(utils->data, "mi,thermal-dimming-flag");
	if (mi_cfg->thermal_dimming_enabled) {
		DISP_INFO("thermal_dimming enabled\n");
	}

	mi_cfg->disable_ic_dimming = utils->read_bool(utils->data, "mi,disable-ic-dimming-flag");
	if (mi_cfg->disable_ic_dimming) {
		DISP_INFO("disable_ic_dimming\n");
	}

	rc = utils->read_u32(utils->data, "mi,normal-to-aod-doze-51reg-update-value", &mi_cfg->doze_51reg_update_value);
	if (rc) {
		mi_cfg->doze_51reg_update_value = 0;
		DISP_INFO("mi,normal-to-aod-doze-51reg-update-value not specified\n");
	} else {
		DISP_INFO("mi,normal-to-aod-doze-51reg-update-value is %d\n", mi_cfg->doze_51reg_update_value);
	}

#ifdef DISPLAY_FACTORY_BUILD
	rc = utils->read_u32(utils->data, "mi,mdss-dsi-fac-bl-max-level", &val);
	if (rc) {
		rc = 0;
		DSI_DEBUG("[%s] factory bl-max-level unspecified\n", panel->name);
	} else {
		panel->bl_config.bl_max_level = val;
	}

	rc = utils->read_u32(utils->data, "mi,mdss-fac-brightness-max-level",&val);
	if (rc) {
		rc = 0;
		DSI_DEBUG("[%s] factory brigheness-max-level unspecified\n", panel->name);
	} else {
		panel->bl_config.brightness_max_level = val;
	}
	DISP_INFO("bl_max_level is %d, brightness_max_level is %d\n",
		panel->bl_config.bl_max_level, panel->bl_config.brightness_max_level);
#endif

	return rc;
}

static int mi_dsi_panel_parse_em_cycle(struct dsi_panel *panel)
{
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;
	u32 length = 0;
	const char *data;
	int rc = 0;

	if (mi_get_panel_id(mi_cfg->mi_panel_id) == M2_PANEL_PA) {
		rc = utils->read_u32(utils->data, "mi,panel-1920pwm-dbv-threshold",
				&mi_cfg->panel_1920pwm_dbv_threshold);
		if (rc)
			DISP_ERROR("mi,panel-1920pwm-dbv-threshold not defined\n");
		else
			DISP_INFO("mi,panel-1920pwm-dbv-threshold is %d\n", mi_cfg->panel_1920pwm_dbv_threshold);

		data = utils->get_property(utils->data, "mi,panel-em-cycle-3pulse-gamma-paramerer",
				&length);
		if (!data) {
			DISP_ERROR("mi,panel-em-cycle-3pulse-gamma-paramerer not defined\n");
			rc = -ENOTSUPP;
		} else {
			DISP_INFO("mi,panel-em-cycle-3pulse-gamma-paramerer:");
			print_hex_dump(KERN_INFO, "mi_disp:", DUMP_PREFIX_NONE, 8, 1, data, length, false);
			if (length == sizeof(mi_cfg->gamma_3pulse_param))
				memcpy(mi_cfg->gamma_3pulse_param, data, length);
		}

		data = utils->get_property(utils->data, "mi,panel-em-cycle-16pulse-gamma-paramerer",
				&length);
		if (!data) {
			DISP_ERROR("mi,panel-em-cycle-16pulse-gamma-paramerer not defined\n");
			rc = -ENOTSUPP;
		} else {
			DISP_INFO("mi,panel-em-cycle-16pulse-gamma-paramerer:");
			print_hex_dump(KERN_INFO, "mi_disp:", DUMP_PREFIX_NONE, 8, 1, data, length, false);
			if (length == sizeof(mi_cfg->gamma_16pulse_param))
				memcpy(mi_cfg->gamma_16pulse_param, data, length);
		}

		data = utils->get_property(utils->data, "mi,panel-em-cycle-gamma-paramerer-auto",
				&length);
		if (!data) {
			DISP_ERROR("mi,panel-em-cycle-gamma-paramerer-auto not defined\n");
			rc = -ENOTSUPP;
		} else {
			DISP_INFO("mi,panel-em-cycle-gamma-paramerer-auto");
			print_hex_dump(KERN_INFO, "mi_disp:", DUMP_PREFIX_NONE, 8, 1, data, length, false);
			if (length == sizeof(mi_cfg->gamma_auto_param))
				memcpy(mi_cfg->gamma_auto_param, data, length);
		}
	}

	if (mi_get_panel_id(mi_cfg->mi_panel_id) == N11_PANEL_PA) {
		rc = utils->read_u32(utils->data, "mi,panel-3840pwm-dbv-threshold",
				&mi_cfg->panel_3840pwm_dbv_threshold);
		if (rc)
			DISP_ERROR("mi,panel-3840pwm-dbv-threshold not defined\n");
		else
			DISP_INFO("mi,panel-3840pwm-dbv-threshold is %d\n", mi_cfg->panel_3840pwm_dbv_threshold);
	}

	return rc;
}


int mi_dsi_panel_parse_config(struct dsi_panel *panel)
{
	int rc = 0;
	struct dsi_parser_utils *utils = &panel->utils;
	struct mi_dsi_panel_cfg *mi_cfg = &panel->mi_cfg;

	rc = utils->read_u64(utils->data, "mi,panel-id", &mi_cfg->mi_panel_id);
	if (rc) {
		mi_cfg->mi_panel_id = 0;
		DISP_INFO("mi,panel-id not specified\n");
	} else {
		DISP_INFO("mi,panel-id is 0x%llx (%s)\n",
			mi_cfg->mi_panel_id, mi_get_panel_id_name(mi_cfg->mi_panel_id));
	}

	mi_cfg->panel_build_id_read_needed =
		utils->read_bool(utils->data, "mi,panel-build-id-read-needed");
	if (mi_cfg->panel_build_id_read_needed) {
		rc = dsi_panel_parse_build_id_read_config(panel);
		if (rc) {
			mi_cfg->panel_build_id_read_needed = false;
			DSI_ERR("[%s] failed to get panel build id read infos, rc=%d\n",
					panel->name, rc);
		}
	}
	rc = dsi_panel_parse_wp_reg_read_config(panel);
	if (rc) {
		DSI_ERR("[%s] failed to get panel wp read infos, rc=%d\n",
			panel->name, rc);
		rc = 0;
	}

	mi_cfg->is_tddi_flag = false;
	mi_cfg->panel_dead_flag = false;
	mi_cfg->tddi_doubleclick_flag = false;
	mi_cfg->is_tddi_flag = utils->read_bool(utils->data, "mi,is-tddi-flag");
	if (mi_cfg->is_tddi_flag)
		pr_info("panel is tddi\n");

	mi_dsi_panel_parse_round_corner_config(panel);
	mi_dsi_panel_parse_lhbm_config(panel);
	mi_dsi_panel_parse_lockdown_config(panel);
	mi_dsi_panel_parse_flat_config(panel);
	mi_dsi_panel_parse_dc_config(panel);
	mi_dsi_panel_parse_backlight_config(panel);
	mi_dsi_panel_parse_em_cycle(panel);

	rc = utils->read_u32(utils->data, "mi,panel-hbm-backlight-threshold", &mi_cfg->hbm_backlight_threshold);
	if (rc)
		mi_cfg->hbm_backlight_threshold = 8192;
	DISP_INFO("panel hbm backlight threshold %d\n", mi_cfg->hbm_backlight_threshold);

	mi_cfg->count_hbm_by_backlight = utils->read_bool(utils->data, "mi,panel-count-hbm-by-backlight-flag");
	if (mi_cfg->count_hbm_by_backlight)
		DISP_INFO("panel count hbm by backlight\n");

	mi_cfg->ignore_esd_in_aod = utils->read_bool(utils->data, "mi,panel-ignore-esd-in-aod");
	if (mi_cfg->ignore_esd_in_aod)
		DISP_INFO("panel don't recovery in aod\n");

	mi_cfg->dbi_3dlut_compensate_enable = utils->read_bool(utils->data, "mi,panel-dbi-3dlut-compensate-enable");
	if (mi_cfg->dbi_3dlut_compensate_enable)
		DISP_INFO("panel dbi_3dlut_compensate_enable\n");

	return rc;
}

