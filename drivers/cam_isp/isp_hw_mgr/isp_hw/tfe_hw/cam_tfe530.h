/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2023, Qualcomm Innovation Center, Inc. All rights reserved.
 */


#ifndef _CAM_TFE530_H_
#define _CAM_TFE530_H_
#include "cam_tfe_core.h"
#include "cam_tfe_bus.h"


static struct cam_tfe_top_reg_offset_common  tfe530_top_commong_reg  = {
	.hw_version                             = 0x00001000,
	.hw_capability                          = 0x00001004,
	.lens_feature                           = 0x00001008,
	.stats_feature                          = 0x0000100C,
	.zoom_feature                           = 0x00001010,
	.global_reset_cmd                       = 0x00001014,
	.core_cgc_ctrl_0                        = 0x00001018,
	.ahb_cgc_ctrl                           = 0x0000101C,
	.core_cfg_0                             = 0x00001024,
	.core_cfg_1                             = 0x00001028,
	.reg_update_cmd                         = 0x0000102C,
	.diag_config                            = 0x00001060,
	.diag_sensor_status_0                   = 0x00001064,
	.diag_sensor_status_1                   = 0x00001068,
	.diag_sensor_frame_cnt_status           = 0x0000106C,
	.violation_status                       = 0x00001070,
	.stats_throttle_cnt_cfg_0               = 0x00001074,
	.stats_throttle_cnt_cfg_1               = 0x00001078,
	.num_debug_reg                          = 4,
	.debug_reg = {
		0x000010A0,
		0x000010A4,
		0x000010A8,
		0x000010AC,
	},
	.debug_cfg                              = 0x000010DC,
	.num_perf_cfg                           = 1,
	.perf_cfg = {
		{
			.perf_cnt_cfg           = 0x000010E0,
			.perf_pixel_count       = 0x000010E4,
			.perf_line_count        = 0x000010E8,
			.perf_stall_count       = 0x000010EC,
			.perf_always_count      = 0x000010F0,
			.perf_count_status      = 0x000010F4,
		},
	},
	.diag_min_hbi_error_shift               = 15,
	.diag_neq_hbi_shift                     = 14,
	.diag_sensor_hbi_mask                   = 0x3FFF,
	.serializer_supported                   = false,
	.pp_camif_violation_bit                 = BIT(0),
	.pp_violation_bit                       = BIT(1),
	.rdi0_camif_violation_bit               = BIT(2),
	.rdi1_camif_violation_bit               = BIT(3),
	.rdi2_camif_violation_bit               = BIT(4),
	.diag_violation_bit                     = BIT(5),
	.pp_frame_drop_bit                      = BIT(8),
	.rdi0_frame_drop_bit                    = BIT(9),
	.rdi1_frame_drop_bit                    = BIT(10),
	.rdi2_frame_drop_bit                    = BIT(11),
	.pp_overflow_bit                        = BIT(16),
	.rdi0_overflow_bit                      = BIT(17),
	.rdi1_overflow_bit                      = BIT(18),
	.rdi2_overflow_bit                      = BIT(19),
	.mup_shift_val                          = 0,
	.mup_supported                          = false,
};

static struct cam_tfe_camif_reg  tfe530_camif_reg = {
	.hw_version                   = 0x00001200,
	.hw_status                    = 0x00001204,
	.module_cfg                   = 0x00001260,
	.pdaf_raw_crop_width_cfg      = 0x00001268,
	.pdaf_raw_crop_height_cfg     = 0x0000126C,
	.line_skip_pattern            = 0x00001270,
	.pixel_skip_pattern           = 0x00001274,
	.period_cfg                   = 0x00001278,
	.irq_subsample_pattern        = 0x0000127C,
	.epoch_irq_cfg                = 0x00001280,
	.debug_1                      = 0x000013F0,
	.debug_0                      = 0x000013F4,
	.test_bus_ctrl                = 0x000013F8,
	.spare                        = 0x000013F8,
	.reg_update_cmd               = 0x0000102C,
};

static struct cam_tfe_camif_reg_data tfe530_camif_reg_data = {
	.extern_reg_update_mask       = 0x00000001,
	.dual_tfe_pix_en_shift        = 0x00000001,
	.extern_reg_update_shift      = 0x0,
	.camif_pd_rdi2_src_sel_shift  = 0x2,
	.dual_tfe_sync_sel_shift      = 18,
	.delay_line_en_shift          = 8,
	.pixel_pattern_shift          = 24,
	.pixel_pattern_mask           = 0x7000000,
	.module_enable_shift          = 0,
	.pix_out_enable_shift         = 8,
	.pdaf_output_enable_shift     = 9,
	.dsp_mode_shift               = 0,
	.dsp_mode_mask                = 0,
	.dsp_en_shift                 = 0,
	.dsp_en_mask                  = 0,
	.reg_update_cmd_data          = 0x1,
	.epoch_line_cfg               = 0x00140014,
	.sof_irq_mask                 = 0x00000001,
	.epoch0_irq_mask              = 0x00000004,
	.epoch1_irq_mask              = 0x00000008,
	.eof_irq_mask                 = 0x00000002,
	.reg_update_irq_mask          = 0x00000001,
	.error_irq_mask0              = 0x00010100,
	.error_irq_mask2              = 0x00000023,
	.subscribe_irq_mask           = {
		0x00000000,
		0x00000007,
		0x00000000,
	},
	.enable_diagnostic_hw         = 0x1,
	.perf_cnt_start_cmd_shift     = 0,
	.perf_cnt_continuous_shift    = 2,
	.perf_client_sel_shift        = 8,
	.perf_window_start_shift      = 16,
	.perf_window_end_shift        = 20,
};

static struct cam_tfe_rdi_reg  tfe530_rdi0_reg = {
	.rdi_hw_version              = 0x00001400,
	.rdi_hw_status               = 0x00001404,
	.rdi_module_config           = 0x00001460,
	.rdi_skip_period             = 0x00001468,
	.rdi_irq_subsample_pattern   = 0x0000146C,
	.rdi_epoch_irq               = 0x00001470,
	.rdi_debug_1                 = 0x000015F0,
	.rdi_debug_0                 = 0x000015F4,
	.rdi_test_bus_ctrl           = 0x000015F8,
	.rdi_spare                   = 0x000015FC,
	.reg_update_cmd              = 0x0000102C,
};

static struct cam_tfe_rdi_reg_data tfe530_rdi0_reg_data = {
	.reg_update_cmd_data         = 0x2,
	.epoch_line_cfg              = 0x00140014,
	.pixel_pattern_shift         = 24,
	.pixel_pattern_mask          = 0x07000000,
	.rdi_out_enable_shift        = 0,

	.sof_irq_mask                = 0x00000010,
	.epoch0_irq_mask             = 0x00000040,
	.epoch1_irq_mask             = 0x00000080,
	.eof_irq_mask                = 0x00000020,
	.error_irq_mask0             = 0x00020200,
	.error_irq_mask2             = 0x00000004,
	.subscribe_irq_mask          = {
		0x00000000,
		0x00000030,
		0x00000000,
	},
	.enable_diagnostic_hw        = 0x1,
	.diag_sensor_sel             = 0x1,
	.diag_sensor_shift           = 0x1,
};

static struct cam_tfe_rdi_reg  tfe530_rdi1_reg = {
	.rdi_hw_version              = 0x00001600,
	.rdi_hw_status               = 0x00001604,
	.rdi_module_config           = 0x00001660,
	.rdi_skip_period             = 0x00001668,
	.rdi_irq_subsample_pattern   = 0x0000166C,
	.rdi_epoch_irq               = 0x00001670,
	.rdi_debug_1                 = 0x000017F0,
	.rdi_debug_0                 = 0x000017F4,
	.rdi_test_bus_ctrl           = 0x000017F8,
	.rdi_spare                   = 0x000017FC,
	.reg_update_cmd              = 0x0000102C,
};

static struct cam_tfe_rdi_reg_data tfe530_rdi1_reg_data = {
	.reg_update_cmd_data         = 0x4,
	.epoch_line_cfg              = 0x00140014,
	.pixel_pattern_shift         = 24,
	.pixel_pattern_mask          = 0x07000000,
	.rdi_out_enable_shift        = 0,

	.sof_irq_mask                = 0x00000100,
	.epoch0_irq_mask             = 0x00000400,
	.epoch1_irq_mask             = 0x00000800,
	.eof_irq_mask                = 0x00000200,
	.error_irq_mask0             = 0x00040400,
	.error_irq_mask2             = 0x00000008,
	.subscribe_irq_mask          = {
		0x00000000,
		0x00000300,
		0x00000000,
	},
	.enable_diagnostic_hw        = 0x1,
	.diag_sensor_sel             = 0x2,
	.diag_sensor_shift           = 0x1,
};

static struct cam_tfe_rdi_reg  tfe530_rdi2_reg = {
	.rdi_hw_version              = 0x00001800,
	.rdi_hw_status               = 0x00001804,
	.rdi_module_config           = 0x00001860,
	.rdi_skip_period             = 0x00001868,
	.rdi_irq_subsample_pattern   = 0x0000186C,
	.rdi_epoch_irq               = 0x00001870,
	.rdi_debug_1                 = 0x000019F0,
	.rdi_debug_0                 = 0x000019F4,
	.rdi_test_bus_ctrl           = 0x000019F8,
	.rdi_spare                   = 0x000019FC,
	.reg_update_cmd              = 0x0000102C,
};

static struct cam_tfe_rdi_reg_data tfe530_rdi2_reg_data = {
	.reg_update_cmd_data         = 0x8,
	.epoch_line_cfg              = 0x00140014,
	.pixel_pattern_shift         = 24,
	.pixel_pattern_mask          = 0x07000000,
	.rdi_out_enable_shift        = 0,

	.sof_irq_mask                = 0x00001000,
	.epoch0_irq_mask             = 0x00004000,
	.epoch1_irq_mask             = 0x00008000,
	.eof_irq_mask                = 0x00002000,
	.error_irq_mask0             = 0x00080800,
	.error_irq_mask2             = 0x00000004,
	.subscribe_irq_mask          = {
		0x00000000,
		0x00003000,
		0x00000000,
	},
	.enable_diagnostic_hw        = 0x1,
	.diag_sensor_sel             = 0x3,
	.diag_sensor_shift           = 0x1,
};

static struct cam_tfe_clc_hw_status  tfe530_clc_hw_info[CAM_TFE_MAX_CLC] = {
	{
		.name = "CLC_CAMIF",
		.hw_status_reg = 0x1204,
	},
	{
		.name = "CLC_RDI0_CAMIF",
		.hw_status_reg = 0x1404,
	},
	{
		.name = "CLC_RDI1_CAMIF",
		.hw_status_reg = 0x1604,
	},
	{
		.name = "CLC_RDI2_CAMIF",
		.hw_status_reg = 0x1804,
	},
	{
		.name = "CLC_CHANNEL_GAIN",
		.hw_status_reg = 0x2604,
	},
	{
		.name = "CLC_LENS_ROLL_OFF",
		.hw_status_reg = 0x2804,
	},
	{
		.name = "CLC_WB_BDS",
		.hw_status_reg = 0x2A04,
	},
	{
		.name = "CLC_STATS_BHIST",
		.hw_status_reg = 0x2C04,
	},
	{
		.name = "CLC_STATS_TINTLESS_BG",
		.hw_status_reg = 0x2E04,
	},
	{
		.name = "CLC_STATS_BAF",
		.hw_status_reg = 0x3004,
	},
	{
		.name = "CLC_STATS_AWB_BG",
		.hw_status_reg = 0x3204,
	},
	{
		.name = "CLC_STATS_AEC_BG",
		.hw_status_reg = 0x3404,
	},
	{
		.name = "CLC_STATS_RAW_OUT",
		.hw_status_reg = 0x3604,
	},
	{
		.name = "CLC_STATS_CROP_POST_BDS",
		.hw_status_reg = 0x3804,
	},
};

static struct  cam_tfe_top_hw_info tfe530_top_hw_info = {
	.common_reg = &tfe530_top_commong_reg,
	.camif_hw_info = {
		.camif_reg = &tfe530_camif_reg,
		.reg_data  = &tfe530_camif_reg_data,
	},
	.rdi_hw_info  = {
		{
			.rdi_reg  = &tfe530_rdi0_reg,
			.reg_data = &tfe530_rdi0_reg_data,
		},
		{
			.rdi_reg  = &tfe530_rdi1_reg,
			.reg_data = &tfe530_rdi1_reg_data,
		},
		{
			.rdi_reg  = &tfe530_rdi2_reg,
			.reg_data = &tfe530_rdi2_reg_data,
		},
	},
	.in_port = {
		CAM_TFE_CAMIF_VER_1_0,
		CAM_TFE_RDI_VER_1_0,
		CAM_TFE_RDI_VER_1_0,
		CAM_TFE_RDI_VER_1_0
	},
	.reg_dump_data  = {
		.num_reg_dump_entries    = 19,
		.num_lut_dump_entries    = 0,
		.bus_start_addr          = 0x2000,
		.bus_write_top_end_addr  = 0x2120,
		.bus_client_start_addr   = 0x2200,
		.bus_client_offset       = 0x100,
		.num_bus_clients         = 10,
		.reg_entry = {
			{
				.start_offset = 0x1000,
				.end_offset   = 0x10F4,
			},
			{
				.start_offset = 0x1260,
				.end_offset   = 0x1280,
			},
			{
				.start_offset = 0x13F0,
				.end_offset   = 0x13FC,
			},
			{
				.start_offset = 0x1460,
				.end_offset   = 0x1470,
			},
			{
				.start_offset = 0x15F0,
				.end_offset   = 0x15FC,
			},
			{
				.start_offset = 0x1660,
				.end_offset   = 0x1670,
			},
			{
				.start_offset = 0x17F0,
				.end_offset   = 0x17FC,
			},
			{
				.start_offset = 0x1860,
				.end_offset   = 0x1870,
			},
			{
				.start_offset = 0x19F0,
				.end_offset   = 0x19FC,
			},
			{
				.start_offset = 0x2660,
				.end_offset   = 0x2694,
			},
			{
				.start_offset = 0x2860,
				.end_offset   = 0x2884,
			},
			{
				.start_offset = 0x2A60,
				.end_offset   = 0X2B34,
			},
			{
				.start_offset = 0x2C60,
				.end_offset   = 0X2C80,
			},
			{
				.start_offset = 0x2E60,
				.end_offset   = 0X2E7C,
			},
			{
				.start_offset = 0x3060,
				.end_offset   = 0X3110,
			},
			{
				.start_offset = 0x3260,
				.end_offset   = 0X3278,
			},
			{
				.start_offset = 0x3460,
				.end_offset   = 0X3478,
			},
			{
				.start_offset = 0x3660,
				.end_offset   = 0X3684,
			},
			{
				.start_offset = 0x3860,
				.end_offset   = 0X3884,
			},
		},
		.lut_entry = {
			{
				.lut_word_size = 1,
				.lut_bank_sel  = 0x40,
				.lut_addr_size = 180,
				.dmi_reg_offset = 0x2800,
			},
			{
				.lut_word_size = 1,
				.lut_bank_sel  = 0x41,
				.lut_addr_size = 180,
				.dmi_reg_offset = 0x3000,
			},
		},
	},
};

static struct cam_tfe_bus_hw_info  tfe530_bus_hw_info = {
	.common_reg = {
		.hw_version  = 0x00001A00,
		.cgc_ovd     = 0x00001A08,
		.comp_cfg_0  = 0x00001A0C,
		.comp_cfg_1  = 0x00001A10,
		.frameheader_cfg  = {
			0x00001A34,
			0x00001A38,
			0x00001A3C,
			0x00001A40,
		},
		.pwr_iso_cfg = 0x00001A5C,
		.overflow_status_clear = 0x00001A60,
		.ccif_violation_status = 0x00001A64,
		.overflow_status       = 0x00001A68,
		.image_size_violation_status = 0x00001A70,
		.perf_count_cfg = {
			0x00001A74,
			0x00001A78,
			0x00001A7C,
			0x00001A80,
			0x00001A84,
			0x00001A88,
			0x00001A8C,
			0x00001A90,
		},
		.perf_count_val = {
			0x00001A94,
			0x00001A98,
			0x00001A9C,
			0x00001AA0,
			0x00001AA4,
			0x00001AA8,
			0x00001AAC,
			0x00001AB0,
		},
		.perf_count_status = 0x00001AB4,
		.debug_status_top_cfg = 0x00001AD4,
		.debug_status_top = 0x00001AD8,
		.test_bus_ctrl = 0x00001ADC,
		.irq_mask = {
			0x00001A18,
			0x00001A1C,
		},
		.irq_clear = {
			0x00001A20,
			0x00001A24,
		},
		.irq_status = {
			0x00001A28,
			0x00001A2C,
		},
		.irq_cmd = 0x00001A30,
		.cons_violation_shift = 28,
		.violation_shift  = 30,
		.image_size_violation = 31,
	},
	.num_client = 10,
	.bus_client_reg = {
		/* BUS Client 0 BAYER */
		{
			.cfg                   = 0x00001C00,
			.image_addr            = 0x00001C04,
			.frame_incr            = 0x00001C08,
			.image_cfg_0           = 0x00001C0C,
			.image_cfg_1           = 0x00001C10,
			.image_cfg_2           = 0x00001C14,
			.packer_cfg            = 0x00001C18,
			.bw_limit              = 0x00001C1C,
			.frame_header_addr     = 0x00001C20,
			.frame_header_incr     = 0x00001C24,
			.frame_header_cfg      = 0x00001C28,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00001C30,
			.irq_subsample_pattern = 0x00001C34,
			.framedrop_period      = 0x00001C38,
			.framedrop_pattern     = 0x00001C3C,
			.addr_status_0         = 0x00001C68,
			.addr_status_1         = 0x00001C6C,
			.addr_status_2         = 0x00001C70,
			.addr_status_3         = 0x00001C74,
			.debug_status_cfg      = 0x00001C78,
			.debug_status_0        = 0x00001C7C,
			.debug_status_1        = 0x00001C80,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_0,
			.client_name           = "BAYER",
		},
		/* BUS Client 1 IDEAL RAW*/
		{
			.cfg                   = 0x00001D00,
			.image_addr            = 0x00001D04,
			.frame_incr            = 0x00001D08,
			.image_cfg_0           = 0x00001D0C,
			.image_cfg_1           = 0x00001D10,
			.image_cfg_2           = 0x00001D14,
			.packer_cfg            = 0x00001D18,
			.bw_limit              = 0x00001D1C,
			.frame_header_addr     = 0x00001D20,
			.frame_header_incr     = 0x00001D24,
			.frame_header_cfg      = 0x00001D28,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00001D30,
			.irq_subsample_pattern = 0x00001D34,
			.framedrop_period      = 0x00001D38,
			.framedrop_pattern     = 0x00001D3C,
			.addr_status_0         = 0x00001D68,
			.addr_status_1         = 0x00001D6C,
			.addr_status_2         = 0x00001D70,
			.addr_status_3         = 0x00001D74,
			.debug_status_cfg      = 0x00001D78,
			.debug_status_0        = 0x00001D7C,
			.debug_status_1        = 0x00001D80,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_1,
			.client_name           = "IDEAL_RAW",
		},
		/* BUS Client 2 Stats BE Tintless */
		{
			.cfg                   = 0x00001E00,
			.image_addr            = 0x00001E04,
			.frame_incr            = 0x00001E08,
			.image_cfg_0           = 0x00001E0C,
			.image_cfg_1           = 0x00001E10,
			.image_cfg_2           = 0x00001E14,
			.packer_cfg            = 0x00001E18,
			.bw_limit              = 0x00001E1C,
			.frame_header_addr     = 0x00001E20,
			.frame_header_incr     = 0x00001E24,
			.frame_header_cfg      = 0x00001E28,
			.line_done_cfg         = 0x00001E00,
			.irq_subsample_period  = 0x00001E30,
			.irq_subsample_pattern = 0x00000E34,
			.framedrop_period      = 0x00001E38,
			.framedrop_pattern     = 0x00001E3C,
			.addr_status_0         = 0x00001E68,
			.addr_status_1         = 0x00001E6C,
			.addr_status_2         = 0x00001E70,
			.addr_status_3         = 0x00001E74,
			.debug_status_cfg      = 0x00001E78,
			.debug_status_0        = 0x00001E7C,
			.debug_status_1        = 0x00001E80,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_2,
			.client_name           = "STATS BE TINTLESS",
		},
		/* BUS Client 3 Stats Bhist */
		{
			.cfg                   = 0x00001F00,
			.image_addr            = 0x00001F04,
			.frame_incr            = 0x00001F08,
			.image_cfg_0           = 0x00001F0C,
			.image_cfg_1           = 0x00001F10,
			.image_cfg_2           = 0x00001F14,
			.packer_cfg            = 0x00001F18,
			.bw_limit              = 0x00001F1C,
			.frame_header_addr     = 0x00001F20,
			.frame_header_incr     = 0x00001F24,
			.frame_header_cfg      = 0x00001F28,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00001F30,
			.irq_subsample_pattern = 0x00001F34,
			.framedrop_period      = 0x00001F38,
			.framedrop_pattern     = 0x00001F3C,
			.addr_status_0         = 0x00001F68,
			.addr_status_1         = 0x00001F6C,
			.addr_status_2         = 0x00001F70,
			.addr_status_3         = 0x00001F74,
			.debug_status_cfg      = 0x00001F78,
			.debug_status_0        = 0x00001F7C,
			.debug_status_1        = 0x00001F80,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_2,
			.client_name           = "STATS BHIST",
		},
		/* BUS Client 4 Stats AWB BG */
		{
			.cfg                   = 0x00002000,
			.image_addr            = 0x00002004,
			.frame_incr            = 0x00002008,
			.image_cfg_0           = 0x0000200C,
			.image_cfg_1           = 0x00002010,
			.image_cfg_2           = 0x00002014,
			.packer_cfg            = 0x00002018,
			.bw_limit              = 0x0000201C,
			.frame_header_addr     = 0x00002020,
			.frame_header_incr     = 0x00002024,
			.frame_header_cfg      = 0x00002028,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002030,
			.irq_subsample_pattern = 0x00002034,
			.framedrop_period      = 0x00002038,
			.framedrop_pattern     = 0x0000203C,
			.addr_status_0         = 0x00002068,
			.addr_status_1         = 0x0000206C,
			.addr_status_2         = 0x00002070,
			.addr_status_3         = 0x00002074,
			.debug_status_cfg      = 0x00002078,
			.debug_status_0        = 0x0000207C,
			.debug_status_1        = 0x00002080,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_3,
			.client_name           = "STATS AWB BG",
		},
		/* BUS Client 5 Stats AEC BG */
		{
			.cfg                   = 0x00002100,
			.image_addr            = 0x00002104,
			.frame_incr            = 0x00002108,
			.image_cfg_0           = 0x0000210C,
			.image_cfg_1           = 0x00002110,
			.image_cfg_2           = 0x00002114,
			.packer_cfg            = 0x00002118,
			.bw_limit              = 0x0000211C,
			.frame_header_addr     = 0x00002120,
			.frame_header_incr     = 0x00002124,
			.frame_header_cfg      = 0x00002128,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002130,
			.irq_subsample_pattern = 0x00002134,
			.framedrop_period      = 0x00002138,
			.framedrop_pattern     = 0x0000213C,
			.addr_status_0         = 0x00002168,
			.addr_status_1         = 0x0000216C,
			.addr_status_2         = 0x00002170,
			.addr_status_3         = 0x00002174,
			.debug_status_cfg      = 0x00002178,
			.debug_status_0        = 0x0000217C,
			.debug_status_1        = 0x00002180,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_3,
			.client_name           = "STATS AEC BG",
		},
		/* BUS Client 6 Stats BAF */
		{
			.cfg                   = 0x00002200,
			.image_addr            = 0x00002204,
			.frame_incr            = 0x00002208,
			.image_cfg_0           = 0x0000220C,
			.image_cfg_1           = 0x00002210,
			.image_cfg_2           = 0x00002214,
			.packer_cfg            = 0x00002218,
			.bw_limit              = 0x0000221C,
			.frame_header_addr     = 0x00002220,
			.frame_header_incr     = 0x00002224,
			.frame_header_cfg      = 0x00002228,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002230,
			.irq_subsample_pattern = 0x00002234,
			.framedrop_period      = 0x00002238,
			.framedrop_pattern     = 0x0000223C,
			.addr_status_0         = 0x00002268,
			.addr_status_1         = 0x0000226C,
			.addr_status_2         = 0x00002270,
			.addr_status_3         = 0x00002274,
			.debug_status_cfg      = 0x00002278,
			.debug_status_0        = 0x0000227C,
			.debug_status_1        = 0x00002280,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_4,
			.client_name           = "STATS BAF",
		},
		/* BUS Client 7 RDI0 */
		{
			.cfg                   = 0x00002300,
			.image_addr            = 0x00002304,
			.frame_incr            = 0x00002308,
			.image_cfg_0           = 0x0000230C,
			.image_cfg_1           = 0x00002310,
			.image_cfg_2           = 0x00002314,
			.packer_cfg            = 0x00002318,
			.bw_limit              = 0x0000231C,
			.frame_header_addr     = 0x00002320,
			.frame_header_incr     = 0x00002324,
			.frame_header_cfg      = 0x00002328,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002330,
			.irq_subsample_pattern = 0x00002334,
			.framedrop_period      = 0x00002338,
			.framedrop_pattern     = 0x0000233C,
			.addr_status_0         = 0x00002368,
			.addr_status_1         = 0x0000236C,
			.addr_status_2         = 0x00002370,
			.addr_status_3         = 0x00002374,
			.debug_status_cfg      = 0x00002378,
			.debug_status_0        = 0x0000237C,
			.debug_status_1        = 0x00002380,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_5,
			.client_name           = "RDI0",
		},
		/* BUS Client 8 RDI1 */
		{
			.cfg                   = 0x00002400,
			.image_addr            = 0x00002404,
			.frame_incr            = 0x00002408,
			.image_cfg_0           = 0x0000240C,
			.image_cfg_1           = 0x00002410,
			.image_cfg_2           = 0x00002414,
			.packer_cfg            = 0x00002418,
			.bw_limit              = 0x0000241C,
			.frame_header_addr     = 0x00002420,
			.frame_header_incr     = 0x00002424,
			.frame_header_cfg      = 0x00002428,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002430,
			.irq_subsample_pattern = 0x00002434,
			.framedrop_period      = 0x00002438,
			.framedrop_pattern     = 0x0000243C,
			.addr_status_0         = 0x00002468,
			.addr_status_1         = 0x0000246C,
			.addr_status_2         = 0x00002470,
			.addr_status_3         = 0x00002474,
			.debug_status_cfg      = 0x00002478,
			.debug_status_0        = 0x0000247C,
			.debug_status_1        = 0x00002480,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_6,
			.client_name           = "RDI1",
		},
		/* BUS Client 9 PDAF/RDI2*/
		{
			.cfg                   = 0x00002500,
			.image_addr            = 0x00002504,
			.frame_incr            = 0x00002508,
			.image_cfg_0           = 0x0000250C,
			.image_cfg_1           = 0x00002510,
			.image_cfg_2           = 0x00002514,
			.packer_cfg            = 0x00002518,
			.bw_limit              = 0x0000251C,
			.frame_header_addr     = 0x00002520,
			.frame_header_incr     = 0x00002524,
			.frame_header_cfg      = 0x00002528,
			.line_done_cfg         = 0x00000000,
			.irq_subsample_period  = 0x00002530,
			.irq_subsample_pattern = 0x00002534,
			.framedrop_period      = 0x00002538,
			.framedrop_pattern     = 0x0000253C,
			.addr_status_0         = 0x00002568,
			.addr_status_1         = 0x0000256C,
			.addr_status_2         = 0x00002570,
			.addr_status_3         = 0x00002574,
			.debug_status_cfg      = 0x00002578,
			.debug_status_0        = 0x0000257C,
			.debug_status_1        = 0x00002580,
			.comp_group            = CAM_TFE_BUS_COMP_GRP_7,
			.client_name           = "RDI2/PADF",
		},
	},
	.num_out  = 11,
	.tfe_out_hw_info = {
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_RDI0,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_5,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_1,
			.mid[0]              = 23,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_RDI1,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_6,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_2,
			.mid[0]              = 24,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_RDI2,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_7,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_3,
			.mid[0]              = 25,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_FULL,
			.max_width        = 4096,
			.max_height       = 4096,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_0,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 16,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_RAW_DUMP,
			.max_width        = 4096,
			.max_height       = 4096,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_1,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 17,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_PDAF,
			.max_width        = 4096,
			.max_height       = 4096,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_7,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_3,
			.mid[0]              = 25,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_STATS_HDR_BE,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_3,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 21,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_STATS_HDR_BHIST,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_2,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 19,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_STATS_TL_BG,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_2,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 18,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_STATS_AWB_BG,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_3,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 20,
		},
		{
			.tfe_out_id       = CAM_TFE_BUS_TFE_OUT_STATS_BF,
			.max_width        = -1,
			.max_height       = -1,
			.composite_group  = CAM_TFE_BUS_COMP_GRP_4,
			.rup_group_id     = CAM_TFE_BUS_RUP_GRP_0,
			.mid[0]              = 22,
		},
	},
	.num_comp_grp             = 8,
	.max_wm_per_comp_grp      = 2,
	.comp_done_shift          = 8,
	.top_bus_wr_irq_shift     = 1,
	.comp_buf_done_mask = 0xFF00,
	.comp_rup_done_mask = 0xF,
	.bus_irq_error_mask = {
		0xD0000000,
		0x00000000,
	},
	.support_consumed_addr = true,
	.pdaf_rdi2_mux_en = true,
	.rdi_width = 64,
};

struct cam_tfe_hw_info cam_tfe530 = {
	.top_irq_mask = {
		0x00001034,
		0x00001038,
		0x0000103C,
	},
	.top_irq_clear = {
		0x00001040,
		0x00001044,
		0x00001048,
	},
	.top_irq_status = {
		0x0000104C,
		0x00001050,
		0x00001054,
	},
	.top_irq_cmd                       = 0x00001030,
	.global_clear_bitmask              = 0x00000001,

	.bus_irq_mask = {
		0x00001A18,
		0x00001A1C,
	},
	.bus_irq_clear = {
		0x00001A20,
		0x00001A24,
	},
	.bus_irq_status = {
		0x00001A28,
		0x00001A2C,
	},
	.bus_irq_cmd = 0x00001A30,
	.bus_violation_reg = 0x00001A64,
	.bus_overflow_reg = 0x00001A68,
	.bus_image_size_vilation_reg = 0x1A70,
	.bus_overflow_clear_cmd = 0x1A60,
	.debug_status_top = 0x1AD8,

	.reset_irq_mask = {
		0x00000001,
		0x00000000,
		0x00000000,
	},
	.error_irq_mask = {
		0x000F0F00,
		0x00000000,
		0x0000003F,
	},
	.bus_reg_irq_mask = {
		0x00000002,
		0x00000000,
	},
	.bus_error_irq_mask = {
		0xC0000000,
		0x00000000,
	},

	.num_clc = 14,
	.clc_hw_status_info            = tfe530_clc_hw_info,
	.bus_version                   = CAM_TFE_BUS_1_0,
	.bus_hw_info                   = &tfe530_bus_hw_info,

	.top_version                   = CAM_TFE_TOP_1_0,
	.top_hw_info                   = &tfe530_top_hw_info,
};

#endif /* _CAM_TFE530_H_ */
