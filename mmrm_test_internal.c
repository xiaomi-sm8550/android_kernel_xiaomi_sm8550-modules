/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "mmrm_test: " fmt

#include <linux/slab.h>
#include <linux/clk.h>

#include "mmrm_test_internal.h"

#define MMRM_TEST_MAX_CLK_CLIENTS 30
#define MMRM_TEST_NUM_CASES 3

enum mmrm_test_vdd_level {
	MMRM_TEST_VDD_LEVEL_LOW_SVS,
	MMRM_TEST_VDD_LEVEL_SVS,
	MMRM_TEST_VDD_LEVEL_SVS_L1,
	MMRM_TEST_VDD_LEVEL_NOM,
	MMRM_TEST_VDD_LEVEL_TURBO,
	MMRM_TEST_VDD_LEVEL_MAX
};

enum mmrm_test_result {
	TEST_MMRM_SUCCESS = 0,
	TEST_MMRM_FAIL_REGISTER,
	TEST_MMRM_FAIL_SETVALUE,
	TEST_MMRM_FAIL_CLKGET,
};

struct mmrm_test_clk_client {
	struct mmrm_clk_client_desc clk_client_desc;
	unsigned long clk_rate[MMRM_TEST_VDD_LEVEL_MAX];
	struct mmrm_client *client;
};

static struct mmrm_test_clk_client mmrm_test_clk_client_list[][MMRM_TEST_MAX_CLK_CLIENTS] = {
	/* LAHAINA */
	{
		{{MMRM_CLIENT_DOMAIN_CAMERA, 42, "cam_cc_ife_0_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 51, "cam_cc_ife_1_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 60, "cam_cc_ife_2_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 4, "cam_cc_bps_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 67, "cam_cc_ife_lite_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 77, "cam_cc_jpeg_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 6, "cam_cc_camnoc_axi_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 70, "cam_cc_ife_lite_csid_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 37, "cam_cc_icp_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 14, "cam_cc_cphy_rx_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 16, "cam_cc_csi0phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 18, "cam_cc_csi1phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 20, "cam_cc_csi2phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 22, "cam_cc_csi3phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 24, "cam_cc_csi4phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 26, "cam_cc_csi5phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 9, "cam_cc_cci_0_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 11, "cam_cc_cci_1_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 118, "cam_cc_slow_ahb_clk_src", NULL},
			{80000000, 80000000, 80000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 33, "cam_cc_fast_ahb_clk_src", NULL},
			{300000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CVP, 8, "video_cc_mvs1_clk_src", NULL},
			{1098000000, 1332000000, 1332000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 41, "disp_cc_mdss_mdp_clk_src", NULL},
			{345000000, 460000000, 460000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_VIDEO, 3, "video_cc_mvs0_clk_src", NULL},
			{1098000000, 1332000000, 1332000000},
			NULL},
	},
	/* WAIPIO */
	{
		{{MMRM_CLIENT_DOMAIN_CAMERA, 51, "cam_cc_ife_0_clk_src", NULL},
			{432000000, 594000000, 675000000, 785000000, 785000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 55, "cam_cc_ife_1_clk_src", NULL},
			{432000000, 594000000, 675000000, 785000000, 785000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 59, "cam_cc_ife_2_clk_src", NULL},
			{432000000, 594000000, 675000000, 785000000, 785000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 37, "cam_cc_csid_clk_src", NULL},
			{400000000, 480000000, 480000000, 480000000, 480000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 116, "cam_cc_sfe_0_clk_src", NULL},
			{432000000, 594000000, 675000000, 785000000, 785000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 119, "cam_cc_sfe_1_clk_src", NULL},
			{432000000, 594000000, 675000000, 785000000, 785000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 70, "cam_cc_ipe_nps_clk_src", NULL},
			{364000000, 500000000, 600000000, 700000000, 700000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 2, "cam_cc_bps_clk_src", NULL},
			{200000000, 400000000, 480000000, 600000000, 600000000}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 64, "cam_cc_ife_lite_clk_src", NULL},
			{400000000, 480000000, 480000000, 480000000, 480000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 75, "cam_cc_jpeg_clk_src", NULL},
			{200000000, 400000000, 480000000, 600000000, 600000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 5, "cam_cc_camnoc_axi_clk_src", NULL},
			{300000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 67, "cam_cc_ife_lite_csid_clk_src", NULL},
			{400000000, 480000000, 480000000, 480000000, 480000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 49, "cam_cc_icp_clk_src", NULL},
			{400000000, 480000000, 600000000, 600000000, 600000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 23, "cam_cc_cphy_rx_clk_src", NULL},
			{400000000, 480000000, 480000000, 480000000, 480000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 25, "cam_cc_csi0phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 27, "cam_cc_csi1phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 29, "cam_cc_csi2phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 31, "cam_cc_csi3phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 33, "cam_cc_csi4phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 35, "cam_cc_csi5phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 8, "cam_cc_cci_0_clk_src", NULL},
			{37500000, 37500000, 37500000, 37500000, 37500000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 10, "cam_cc_cci_1_clk_src", NULL},
			{37500000, 37500000, 37500000, 37500000, 37500000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 123, "cam_cc_slow_ahb_clk_src", NULL},
			{80000000, 80000000, 80000000, 80000000, 80000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 45, "cam_cc_fast_ahb_clk_src", NULL},
			{100000000, 200000000, 300000000, 400000000, 400000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_CVP, 8, "video_cc_mvs1_clk_src", NULL},
			{1050000000, 1350000000, 1500000000, 1650000000, 1650000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 61, "disp_cc_mdss_mdp_clk_src", NULL},
			{200000000, 325000000, 375000000, 500000000, 500000000},
			NULL},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 15, "disp_cc_mdss_dptx0_link_clk_src", NULL},
			{270000000, 270000000, 540000, 810000, 810000}, // kHz
			NULL},
		{{MMRM_CLIENT_DOMAIN_VIDEO, 3, "video_cc_mvs0_clk_src", NULL},
			{720000000, 1014000000, 1098000000, 1332000000, 1332000000},
			NULL},
	},
};

static int test_mmrm_client_callback(struct mmrm_client_notifier_data *notifier_data)
{
	// TODO: Test callback here
	return 0;
}

static struct mmrm_client *test_mmrm_client_register(struct mmrm_client_desc *desc)
{
	struct mmrm_client *client;

	if (!desc) {
		pr_info("%s: Invalid input\n", __func__);
		return NULL;
	}

	pr_info("%s: domain(%d) cid(%d) name(%s) type(%d) pri(%d) pvt(%p) "
		"notifier(%p)\n",
		__func__,
		desc->client_info.desc.client_domain,
		desc->client_info.desc.client_id,
		desc->client_info.desc.name,
		desc->client_type,
		desc->priority,
		desc->pvt_data,
		desc->notifier_callback_fn);

	pr_info("%s: Registering mmrm client %s\n", __func__, desc->client_info.desc.name);
	client = mmrm_client_register(desc);
	if (client == NULL) {
		pr_info("%s: Failed to register mmrm client %s\n",
			__func__,
			desc->client_info.desc.name);
		return NULL;
	}
	pr_info("%s: cuid(%d)\n", __func__, client->client_uid);
	return client;
}

static int test_mmrm_client_deregister(struct mmrm_client *client)
{
	int rc;

	if (!client) {
		pr_info("%s: Invalid input\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: cuid(%d) Deregistering mmrm client\n", __func__, client->client_uid);
	rc = mmrm_client_deregister(client);
	if (rc != 0) {
		pr_info("%s: cuid(%d) Failed to deregister mmrm client with %d\n",
			__func__,
			client->client_uid,
			rc);
		return rc;
	}
	return rc;
}

static int test_mmrm_client_set_value(
	struct mmrm_client *client, struct mmrm_client_data *client_data, unsigned long val)
{
	int rc;

	if (!client || !client_data) {
		pr_info("%s: Invalid input\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: cuid(%d) Setting value(%d) for mmrm client\n",
		__func__,
		client->client_uid,
		val);
	rc = mmrm_client_set_value(client, client_data, val);
	if (rc != 0) {
		pr_info("%s: cuid(%d) Failed to set value(%d) for mmrm client with %d\n",
			__func__,
			client->client_uid,
			val,
			rc);
		return rc;
	}
	return rc;
}

static int test_mmrm_client_get_value(struct mmrm_client *client, struct mmrm_client_res_value *val)
{
	int rc;

	if (!client || !val) {
		pr_info("%s: Invalid input\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: cuid(%d) Getting value for mmrm client\n", __func__, client->client_uid);
	rc = mmrm_client_get_value(client, val);
	if (rc != 0) {
		pr_info("%s: cuid(%d) Failed to get value for mmrm client with %d\n",
			__func__,
			client->client_uid,
			rc);
		return rc;
	}
	pr_info("%s: cuid(%d) min(%d) cur(%d) max(%d)\n",
		__func__,
		client->client_uid,
		val->min,
		val->cur,
		val->max);
	return rc;
}

void test_mmrm_client(struct platform_device *pdev, int index, int count)
{
	struct clk *clk; // clk struct
	struct mmrm_client *client; // mmrm client
	struct mmrm_client_data client_data; // mmrm client data
	struct mmrm_client_res_value res_val; // mmrm get_val
	int level; // mmrm set_val
	unsigned long val; // mmrm set_val

	int i, pass_count;
	int rc = 0;

	pr_info("%s: Running individual client tests\n", __func__);

	// Run nominal test for each individual clock source
	for (i = 0, pass_count = 0; i < count; i++) {
		// Create callback used to pass resource data to client
		struct mmrm_client_notifier_data notifier_data = {
			MMRM_CLIENT_RESOURCE_VALUE_CHANGE, // cb_type
			{{0, 0}}, // cb_data (old_val, new_val)
			NULL}; // pvt_data

		// Create client descriptor
		struct mmrm_client_desc desc = {MMRM_CLIENT_CLOCK, // client type
			{mmrm_test_clk_client_list[index][i]
					.clk_client_desc}, // clock client descriptor
			MMRM_CLIENT_PRIOR_HIGH, // client priority
			notifier_data.pvt_data, // pvt_data
			test_mmrm_client_callback}; // callback fn

		// Get clk
		clk = clk_get(&pdev->dev, (const char *)&desc.client_info.desc.name);
		if (IS_ERR_OR_NULL(clk)) {
			pr_info("%s: Failed clk_get for %s\n",
				__func__,
				desc.client_info.desc.name);
			rc = -EINVAL;
			goto err_clk;
		}
		desc.client_info.desc.clk = clk;
		pr_info("%s: clk(%pK)\n", __func__, clk);

		// Register client
		client = test_mmrm_client_register(&desc);
		if (client == NULL) {
			rc = -EINVAL;
			goto err_register;
		}

		// Set values (Use reserve only)
		client_data = (struct mmrm_client_data){1, MMRM_CLIENT_DATA_FLAG_RESERVE_ONLY};
		for (level = 0; level < MMRM_TEST_VDD_LEVEL_MAX; level++) {
			val = mmrm_test_clk_client_list[index][i].clk_rate[level];
			rc = test_mmrm_client_set_value(client, &client_data, val);
			if (rc != 0)
				goto err_setval;
		}

		// Get value
		rc = test_mmrm_client_get_value(client, &res_val);
		if (rc != 0)
			goto err_getval;

	err_setval:
	err_getval:

		// Reset clk rate
		test_mmrm_client_set_value(client, &client_data, 0);

		// Deregister client
		rc = test_mmrm_client_deregister(client);
		if (rc != 0)
			goto err_deregister;

	err_clk:
	err_register:
	err_deregister:

		if (rc == 0)
			pass_count++;

		if (clk)
			clk_put(clk);
	}

	pr_info("%s: Finish individual client tests (pass / total): (%d / %d)\n",
		__func__, pass_count, count);
}

#define WP_CAM_CC_IFE_0_IDX        0
#define WP_CAM_CC_IFE_1_IDX        1
#define WP_CAM_CC_IFE_2_IDX        2
#define WP_CAM_CC_CSID_IDX         3
#define WP_CAM_CC_SFE_0_IDX        4
#define WP_CAM_CC_SFE_1_IDX        5
#define WP_CAM_CC_IPE_NPS_IDX      6
#define WP_CAM_CC_BPS_IDX          7
#define WP_CAM_CC_IFE_LITE_IDX     8
#define WP_CAM_CC_JPEG_IDX         9
#define WP_CAM_CC_CAMNOC_AXI_IDX    10
#define WP_CAM_CC_IFE_LITE_CSID_IDX 11
#define WP_CAM_CC_ICP_IDX           12
#define WP_CAM_CC_CPHY_RX_IDX       13
#define WP_CAM_CC_CSI0PHYTIMER_IDX       14
#define WP_CAM_CC_CSI1PHYTIMER_IDX       15
#define WP_CAM_CC_CSI2PHYTIMER_IDX       16
#define WP_CAM_CC_CSI3PHYTIMER_IDX       17
#define WP_CAM_CC_CSI4PHYTIMER_IDX       18
#define WP_CAM_CC_CSI5PHYTIMER_IDX       19
#define WP_CAM_CC_CCI_0_IDX              20
#define WP_CAM_CC_CCI_1_IDX              21
#define WP_CAM_CC_SLOW_AHB_IDX           22
#define WP_CAM_CC_FAST_AHB_IDX           23
#define WP_VIDEO_CC_MVS1_IDX             24
#define WP_DISP_CC_MDSS_MDP_IDX          25
#define WP_DISP_CC_MDSS_DPTX0_IDX        26
#define WP_VIDEO_CC_MVS0_IDX             27


#define get_csid_id(n) (mmrm_test_clk_client_list[n][WP_CAM_CC_CSID_IDX].clk_client_desc.client_id)
#define get_ife0_id(n) (mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX].clk_client_desc.client_id)
#define get_ife1_id(n) (mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX].clk_client_desc.client_id)
#define get_ife2_id(n) (mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX].clk_client_desc.client_id)

// for camera ife/ipe/bps at nom
// display mdss_mdp/dp_tx0 at nom
// video/cvp at nom
// all camera +cvp at nom
// all camera +cvp + mdss_mdp at nom
// all camera + cvp +mdss_mdp +video at nom
// all camera at nom + mdp/cvp/video svsl1
// mdp at svsl1 + video at nom : voltage corner scaling

// mdp at svsl1 + video at svsl1 + cvp at svsl1 + camera at nom


// for camera ife/ipe/bps at nom
//
#define  decl_test_case_1(n) static struct mmrm_test_desc test_case_1_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// display mdss_mdp/dptx0 at nom
//
//
//
#define  decl_test_case_2(n) static struct mmrm_test_desc test_case_2_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_DPTX0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// video/cvp at nom
//
#define  decl_test_case_3(n) static struct mmrm_test_desc test_case_3_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// all camera +cvp at nom
//
#define  decl_test_case_4(n) static struct mmrm_test_desc test_case_4_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// all camera +cvp + mdss_mdp at nom
//
#define  decl_test_case_5(n) static struct mmrm_test_desc test_case_5_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// all camera + cvp +mdss_mdp +video at nom
//
#define  decl_test_case_6(n) static struct mmrm_test_desc test_case_6_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// all camera at nom + mdp/cvp/video svsl1
//
#define  decl_test_case_7(n) static struct mmrm_test_desc test_case_7_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{NULL, 0} \
}

// mdp at svsl1 + video at nom : voltage corner scaling
//
//
//
#define  decl_test_case_8(n) static struct mmrm_test_desc test_case_8_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

//	ife0, ife1 (lowsvs) + ipe (svs) + bps (nom) + mdp (svsl1) + eva (svsl1) + vid (svsl1)
//	ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (lowsvs)
//	ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (svs)
//	ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (nom) + vid (lowsvs)
//	ife0, ife1, ife2 (svs) + ipe (nom) + bps (nom) + sbi (svs) + mdp (svsl1) + eva (svs) + vid (lowsvs)
//	ife0, ife1 (svs) , ife2 (lowsvs) +
//	sfe0 (svs) + sfe1(svs) + ipe (nom) + bps (nom) + sbi (svs) + mdp (svsl1) + eva (lowsvs)
//	ife0, ife1 (svs) + ipe (nom) + bps (nom) + sbi (svs) + mdp (svsl1) + eva (svs) + vid (svs)
//	ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (lowsvs)
//	ife0, ife1 (lowsvs) + ipe (svs) + bps (nom) + mdp (svsl1) + eva (svsl1) + vid (svsl1)

#define  decl_test_case_11(n) static struct mmrm_test_desc test_case_11_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{NULL, 0} \
}

//		ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (lowsvs)
#define  decl_test_case_12(n) static struct mmrm_test_desc test_case_12_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

//		ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (svs)
#define  decl_test_case_13(n) static struct mmrm_test_desc test_case_13_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

//		ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (nom) + vid (lowsvs)
#define  decl_test_case_14(n) static struct mmrm_test_desc test_case_14_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

//		ife0, ife1, ife2 (svs) + ipe (nom) + bps (nom) + sbi (svs) + mdp (svsl1) + eva (svs) + vid (lowsvs)
#define  decl_test_case_15(n) static struct mmrm_test_desc test_case_15_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

//		ife0, ife1 (svs) , ife2 (lowsvs) + sfe0 (svs) + sfe1(svs) + ipe (nom) +
//		bps (nom) + sbi (svs) + mdp (svsl1) + eva (lowsvs)
#define  decl_test_case_16(n) static struct mmrm_test_desc test_case_16_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{NULL, 0} \
}

//		ife0, ife1 (svs) + ipe (nom) + bps (nom) + sbi (svs) + mdp (svsl1) + eva (svs) + vid (svs)
#define  decl_test_case_17(n) static struct mmrm_test_desc test_case_17_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{NULL, 0} \
}

//		ife0, ife1 (lowsvs) + ipe (nom) + bps (nom) + sbi (lowsvs) + mdp (svsl1) + eva (lowsvs) + vid (lowsvs)
#define  decl_test_case_18(n) static struct mmrm_test_desc test_case_18_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

// throttle display
// vid(nom) + eva (nom) +mdp (nom) + bps(nom) + ipe(nom) +sfe0(nom) + sfe1(nom) +camnoc(nom) + ife0(nom) + csid0(nom)+ ife1(nom) + csid1(nom) + ife2(lowsvs)
//
//
#define  decl_test_case_19(n) static struct mmrm_test_desc test_case_19_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_LOW_SVS},\
	{NULL, 0} \
}

// throttle video
// vid(nom) + eva (nom) +mdp (nom) + bps(nom) + ipe(nom) +sfe0(nom) + sfe1(nom) +camnoc(nom) + ife0(nom) + csid0(nom)+ ife1(nom) + csid1(nom) + ife2(svs)
//
//
#define  decl_test_case_20(n) static struct mmrm_test_desc test_case_20_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_SVS},\
	{NULL, 0} \
}

// throttle ipe
// vid(nom) + eva (nom) +mdp (nom) + bps(nom) + ipe(nom) +sfe0(nom) + sfe1(nom) +camnoc(nom) + ife0(nom) + csid0(nom)+ ife1(nom) + csid1(nom) + ife2(svs)
//
//
#define  decl_test_case_21(n) static struct mmrm_test_desc test_case_21_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{NULL, 0} \
}

// all camera +cam_cc_csid + cvp at nom
//
#define  decl_test_case_9(n) static struct mmrm_test_desc test_case_9_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{NULL, 0} \
}

// all camera at nom + cam_cc_csid + mdp/cvp/video svsl1
//
#define  decl_test_case_10(n) static struct mmrm_test_desc test_case_10_##n[] = {\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_2_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SFE_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IFE_LITE_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_IPE_NPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_BPS_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSID_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_JPEG_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CAMNOC_AXI_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_ICP_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CPHY_RX_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI0PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI1PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI2PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI3PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI4PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CSI5PHYTIMER_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_0_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_CCI_1_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_SLOW_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
	{&mmrm_test_clk_client_list[n][WP_CAM_CC_FAST_AHB_IDX], MMRM_TEST_VDD_LEVEL_NOM},\
\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS1_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_DISP_CC_MDSS_MDP_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{&mmrm_test_clk_client_list[n][WP_VIDEO_CC_MVS0_IDX], MMRM_TEST_VDD_LEVEL_SVS_L1},\
	{NULL, 0} \
}

decl_test_case_1(MMRM_TEST_WAIPIO);
decl_test_case_2(MMRM_TEST_WAIPIO);
decl_test_case_3(MMRM_TEST_WAIPIO);
decl_test_case_4(MMRM_TEST_WAIPIO);
decl_test_case_5(MMRM_TEST_WAIPIO);
decl_test_case_6(MMRM_TEST_WAIPIO);
decl_test_case_7(MMRM_TEST_WAIPIO);
decl_test_case_8(MMRM_TEST_WAIPIO);
decl_test_case_9(MMRM_TEST_WAIPIO);
decl_test_case_10(MMRM_TEST_WAIPIO);

decl_test_case_11(MMRM_TEST_WAIPIO);
decl_test_case_12(MMRM_TEST_WAIPIO);
decl_test_case_13(MMRM_TEST_WAIPIO);
decl_test_case_14(MMRM_TEST_WAIPIO);
decl_test_case_15(MMRM_TEST_WAIPIO);
decl_test_case_16(MMRM_TEST_WAIPIO);
decl_test_case_17(MMRM_TEST_WAIPIO);
decl_test_case_18(MMRM_TEST_WAIPIO);
decl_test_case_19(MMRM_TEST_WAIPIO);
decl_test_case_20(MMRM_TEST_WAIPIO);
decl_test_case_21(MMRM_TEST_WAIPIO);

struct  mmrm_test_desc  *waipio_all_testcases[] = {
	test_case_1_MMRM_TEST_WAIPIO,
	test_case_2_MMRM_TEST_WAIPIO,
	test_case_3_MMRM_TEST_WAIPIO,
	test_case_4_MMRM_TEST_WAIPIO,
	test_case_5_MMRM_TEST_WAIPIO,
	test_case_6_MMRM_TEST_WAIPIO,
	test_case_7_MMRM_TEST_WAIPIO,
	test_case_8_MMRM_TEST_WAIPIO,
	test_case_9_MMRM_TEST_WAIPIO,
	test_case_10_MMRM_TEST_WAIPIO,
	test_case_11_MMRM_TEST_WAIPIO,
	test_case_12_MMRM_TEST_WAIPIO,
	test_case_13_MMRM_TEST_WAIPIO,
	test_case_14_MMRM_TEST_WAIPIO,
	test_case_15_MMRM_TEST_WAIPIO,
	test_case_16_MMRM_TEST_WAIPIO,
	test_case_17_MMRM_TEST_WAIPIO,
	test_case_18_MMRM_TEST_WAIPIO,
	test_case_19_MMRM_TEST_WAIPIO,
	test_case_20_MMRM_TEST_WAIPIO,
	test_case_21_MMRM_TEST_WAIPIO,
};

int waipio_all_testcases_count = sizeof(waipio_all_testcases)/sizeof(waipio_all_testcases[0]);

int test_mmrm_testcase_register(struct platform_device *pdev, struct mmrm_test_desc *pcase)
{
	struct clk *clk;
	int rc = TEST_MMRM_SUCCESS;
	// Create client descriptor
	struct mmrm_client_desc desc = {
		MMRM_CLIENT_CLOCK,          // client type
		{},                         // clock client descriptor
		MMRM_CLIENT_PRIOR_HIGH,     // client priority
		NULL,                       // pvt_data
		test_mmrm_client_callback   // callback fn
	};
	struct mmrm_clk_client_desc *p_clk_client_desc = &(pcase->clk_client->clk_client_desc);

	desc.client_info.desc.client_domain = p_clk_client_desc->client_domain;
	desc.client_info.desc.client_id = p_clk_client_desc->client_id;
	strlcpy((char *)(desc.client_info.desc.name), p_clk_client_desc->name,
								MMRM_CLK_CLIENT_NAME_SIZE);

	// Get clk
	clk = clk_get(&pdev->dev, (const char *)&desc.client_info.desc.name);
	if (IS_ERR_OR_NULL(clk)) {
		pr_info("%s: domain(%d) client_id(%d) name(%s) Failed clk_get\n",
				__func__, desc.client_info.desc.client_domain,
		desc.client_info.desc.client_id, desc.client_info.desc.name);
		rc = -TEST_MMRM_FAIL_CLKGET;
		goto err_clk;
	}
	desc.client_info.desc.clk = clk;

	// Register client
	pcase->clk_client->client = test_mmrm_client_register(&desc);

err_clk:
	return rc;
}

int test_mmrm_get_num_hw_block(struct mmrm_test_desc *pcase)
{
	struct mmrm_test_desc *p = pcase;
	int   count = 0;

	while (!IS_ERR_OR_NULL(p->clk_client)) {
		if (p->clk_client->clk_client_desc.client_id == get_ife0_id(MMRM_TEST_WAIPIO) ||
			p->clk_client->clk_client_desc.client_id == get_ife1_id(MMRM_TEST_WAIPIO) ||
			p->clk_client->clk_client_desc.client_id == get_ife2_id(MMRM_TEST_WAIPIO))
			count++;
		p++;
	}
	return count;
}

int test_mmrm_run_one_case(struct platform_device *pdev,
	struct mmrm_test_desc *pcase)
{
	struct mmrm_client_data    client_data;
	unsigned long val;
	struct mmrm_test_desc *p = pcase;
	int rc = TEST_MMRM_SUCCESS;

	client_data = (struct mmrm_client_data){0, MMRM_CLIENT_DATA_FLAG_RESERVE_ONLY};

	while (!IS_ERR_OR_NULL(p->clk_client)) {
		val = p->clk_client->clk_rate[p->clk_rate_id];

		rc = test_mmrm_testcase_register(pdev, p);
		if ((rc != TEST_MMRM_SUCCESS) || (IS_ERR_OR_NULL(p->clk_client->client))) {
			pr_info("%s: client(%s) fail register\n", __func__,
						p->clk_client->clk_client_desc.name);
			rc = -TEST_MMRM_FAIL_REGISTER;
			break;
		}

		client_data.num_hw_blocks = 1;
		if (p->clk_client->clk_client_desc.client_id == get_csid_id(MMRM_TEST_WAIPIO))
			client_data.num_hw_blocks = test_mmrm_get_num_hw_block(pcase);

		pr_info("%s: domain:%d  csid:%d num_hw_block:%d\n",
					__func__,
					p->clk_client->clk_client_desc.client_domain,
					p->clk_client->clk_client_desc.client_id,
					client_data.num_hw_blocks);

		if (test_mmrm_client_set_value(p->clk_client->client, &client_data, val) != 0) {
			rc = -TEST_MMRM_FAIL_SETVALUE;
			break;
		}

		p++;
	}

	p = pcase;
	while (!IS_ERR_OR_NULL(p->clk_client)) {
		if (!IS_ERR_OR_NULL(p->clk_client->client)) {
			test_mmrm_client_set_value(p->clk_client->client, &client_data, 0);
			test_mmrm_client_deregister(p->clk_client->client);
		}
		p++;
	}

	return rc;
}

void test_mmrm_concurrent_client_cases(struct platform_device *pdev,
							struct mmrm_test_desc **testcases, int count)
{
	struct mmrm_test_desc **p = testcases;
	int i;
	int size, rc, pass = 0;
	int *result_ptr;

	pr_info("%s: Started\n", __func__);

	size = sizeof(int) * count;

	result_ptr = kzalloc(size, GFP_KERNEL);
	if (IS_ERR_OR_NULL(result_ptr)) {
		pr_info("%s: failed to allocate memory for concurrent client test\n",
			__func__);
		goto err_fail_alloc_result_ptr;
	}

	p = testcases;
	for (i = 0; i < count; i++, p++) {
		pr_info("%s: testcase: %d -----\n", __func__, i);
		rc = test_mmrm_run_one_case(pdev, *p);
		result_ptr[i] = rc;
		if (rc == TEST_MMRM_SUCCESS)
			pass++;
	}

	pr_info("%s: Finish concurrent client tests (pass / total): (%d / %d)\n",
			__func__, pass, count);

	for (i = 0; i < count; i++) {
		if (result_ptr[i] != TEST_MMRM_SUCCESS)
			pr_info("%s: Failed client test# %d reason %d\n",
				__func__, i, result_ptr[i]);
	}
	kfree(result_ptr);

err_fail_alloc_result_ptr:
	;

}

