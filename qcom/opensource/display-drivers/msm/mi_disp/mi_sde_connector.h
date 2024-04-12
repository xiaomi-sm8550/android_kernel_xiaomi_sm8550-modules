/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 * Copyright (c) 2020 XiaoMi, Inc. All rights reserved.
 */

#ifndef _MI_SDE_CONNECTOR_H_
#define _MI_SDE_CONNECTOR_H_

#include <linux/types.h>
#include "mi_disp_config.h"
#include <drm/drm_connector.h>

struct sde_connector;

struct mi_layer_flags {
  union {
    struct {
      u32 aod_changed : 1;         /* aod layer state changed */
      u32 aod_present : 1;
      u32 gxzw_anim_changed : 1;  /* gxzw_anim layer state changed */
      u32 gxzw_anim_present : 1;
      u32 aod_layer_remove : 1;
      u32 reserved : 8;
    };
    u32 mi_layer_flags;
  };
};

int mi_sde_connector_register_esd_irq(struct sde_connector *c_conn);

int mi_sde_connector_check_layer_flags(struct drm_connector *connector);

int mi_sde_connector_flat_fence(struct drm_connector *connector);

#if MI_DISP_DEBUGFS_ENABLE
int mi_sde_connector_debugfs_esd_sw_trigger(void *display);
#else
static inline int mi_sde_connector_debugfs_esd_sw_trigger(void *display) { return 0; }
#endif

int mi_sde_connector_panel_ctl(struct drm_connector *connector, uint32_t op_code);

#endif /* _MI_SDE_CONNECTOR_H_ */
