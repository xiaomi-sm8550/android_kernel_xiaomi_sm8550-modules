/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
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

#include <linux/clk.h>

#include "mmrm_test_internal.h"

#define MMRM_TEST_MAX_CLK_CLIENTS 30
#define MMRM_TEST_NUM_CASES 3

enum mmrm_test_vdd_level {
	MMRM_TEST_VDD_LEVEL_SVS_L1,
	MMRM_TEST_VDD_LEVEL_NOM,
	MMRM_TEST_VDD_LEVEL_TURBO,
	MMRM_TEST_VDD_LEVEL_MAX
};

struct mmrm_test_param {
	bool enable;
	int level;
};

struct mmrm_test_clk_client {
	struct mmrm_clk_client_desc clk_client_desc;
	unsigned long clk_rate[MMRM_TEST_VDD_LEVEL_MAX];
	struct mmrm_client *client;
	struct mmrm_test_param test_params[MMRM_TEST_NUM_CASES];
};

static struct mmrm_test_clk_client mmrm_test_clk_client_list[][MMRM_TEST_MAX_CLK_CLIENTS] = {
	/* LAHAINA */
	{
		{{MMRM_CLIENT_DOMAIN_CAMERA, 42, "cam_cc_ife_0_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 51, "cam_cc_ife_1_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 60, "cam_cc_ife_2_clk_src", NULL},
			{600000000, 720000000, 720000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 4, "cam_cc_bps_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 67, "cam_cc_ife_lite_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 77, "cam_cc_jpeg_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 6, "cam_cc_camnoc_axi_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 70, "cam_cc_ife_lite_csid_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 37, "cam_cc_icp_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 14, "cam_cc_cphy_rx_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 16, "cam_cc_csi0phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 18, "cam_cc_csi1phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 20, "cam_cc_csi2phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 22, "cam_cc_csi3phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 24, "cam_cc_csi4phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 26, "cam_cc_csi5phytimer_clk_src", NULL},
			{300000000, 300000000, 300000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 9, "cam_cc_cci_0_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 11, "cam_cc_cci_1_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 118, "cam_cc_slow_ahb_clk_src", NULL},
			{80000000, 80000000, 80000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 33, "cam_cc_fast_ahb_clk_src", NULL},
			{300000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CVP, 8, "video_cc_mvs1_clk_src", NULL},
			{1098000000, 1332000000, 1332000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 41, "disp_cc_mdss_mdp_clk_src", NULL},
			{345000000, 460000000, 460000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_VIDEO, 3, "video_cc_mvs0_clk_src", NULL},
			{1098000000, 1332000000, 1332000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
	},
	/* WAIPIO */
	{
		{{MMRM_CLIENT_DOMAIN_CAMERA, 51, "cam_cc_ife_0_clk_src", NULL},
			{675000000, 785000000, 785000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 55, "cam_cc_ife_1_clk_src", NULL},
			{675000000, 785000000, 785000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 59, "cam_cc_ife_2_clk_src", NULL},
			{675000000, 785000000, 785000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 37, "cam_cc_csid_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 116, "cam_cc_sfe_0_clk_src", NULL},
			{675000000, 785000000, 785000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 119, "cam_cc_sfe_1_clk_src", NULL},
			{675000000, 785000000, 785000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 70, "cam_cc_ipe_nps_clk_src", NULL},
			{600000000, 700000000, 700000000},
			NULL,
			{{true, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_SVS_L1},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 2, "cam_cc_bps_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 64, "cam_cc_ife_lite_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 75, "cam_cc_jpeg_clk_src", NULL},
			{480000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 5, "cam_cc_camnoc_axi_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 67, "cam_cc_ife_lite_csid_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 49, "cam_cc_icp_clk_src", NULL},
			{600000000, 600000000, 600000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 23, "cam_cc_cphy_rx_clk_src", NULL},
			{480000000, 480000000, 480000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 25, "cam_cc_csi0phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 27, "cam_cc_csi1phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 29, "cam_cc_csi2phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 31, "cam_cc_csi3phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 33, "cam_cc_csi4phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 35, "cam_cc_csi5phytimer_clk_src", NULL},
			{400000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 8, "cam_cc_cci_0_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 10, "cam_cc_cci_1_clk_src", NULL},
			{37500000, 37500000, 37500000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 123, "cam_cc_slow_ahb_clk_src", NULL},
			{80000000, 80000000, 80000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CAMERA, 45, "cam_cc_fast_ahb_clk_src", NULL},
			{300000000, 400000000, 400000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_CVP, 8, "video_cc_mvs1_clk_src", NULL},
			{1500000000, 1650000000, 1650000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 61, "disp_cc_mdss_mdp_clk_src", NULL},
			{375000000, 500000000, 500000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_DISPLAY, 15, "disp_cc_mdss_dptx0_link_clk_src", NULL},
			{540000, 810000, 810000}, // kHz
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
		{{MMRM_CLIENT_DOMAIN_VIDEO, 3, "video_cc_mvs0_clk_src", NULL},
			{1098000000, 1332000000, 1332000000},
			NULL,
			{{false, MMRM_TEST_VDD_LEVEL_NOM},{false, MMRM_TEST_VDD_LEVEL_NOM},{true, MMRM_TEST_VDD_LEVEL_NOM}}},
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
		client_data = (struct mmrm_client_data){0, MMRM_CLIENT_DATA_FLAG_RESERVE_ONLY};
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

void test_mmrm_single_case(struct platform_device *pdev, int index, int count, int test)
{
	struct clk *clk; // clk struct
	struct mmrm_client *client; // mmrm client
	struct mmrm_client_data client_data; // mmrm client data
	unsigned long val; // mmrm set_val

	int i;
	int rc = 0;

	pr_info("%s: Running test case %d\n", __func__, test);

	for (i = 0; i < count; i++) {
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

		// Get test params
		struct mmrm_test_param param = mmrm_test_clk_client_list[index][i].test_params[test];
		if (!param.enable)
			continue;

		// Get clk
		clk = clk_get(&pdev->dev, (const char *)&desc.client_info.desc.name);
		if (IS_ERR_OR_NULL(clk)) {
			pr_info("%s: Failed clk_get for %s\n",
				__func__,
				desc.client_info.desc.name);
			goto err_clk;
		}
		desc.client_info.desc.clk = clk;
		pr_info("%s: clk(%pK)\n", __func__, clk);

		// Register client
		client = test_mmrm_client_register(&desc);
		if (client == NULL) {
			goto err_register;
		}

		// Set value for test case (Use reserve only)
		client_data = (struct mmrm_client_data){0, MMRM_CLIENT_DATA_FLAG_RESERVE_ONLY};
		val = mmrm_test_clk_client_list[index][i].clk_rate[param.level];
		rc = test_mmrm_client_set_value(client, &client_data, val);
		if (rc != 0)
			goto err_setval;

		mmrm_test_clk_client_list[index][i].clk_client_desc.clk = clk;
		mmrm_test_clk_client_list[index][i].client = client;
		continue;

	err_setval:
		test_mmrm_client_set_value(client, &client_data, 0);
		test_mmrm_client_deregister(client);
	err_clk:
	err_register:
		if (clk)
			clk_put(clk);
	}

	// Deregister all clients
	for (i = 0; i < count; i++) {
		if (mmrm_test_clk_client_list[index][i].client) {
			client = mmrm_test_clk_client_list[index][i].client;
			test_mmrm_client_set_value(client, &client_data, 0);
			test_mmrm_client_deregister(client);
			mmrm_test_clk_client_list[index][i].client = NULL;
		}
		if (mmrm_test_clk_client_list[index][i].clk_client_desc.clk) {
			clk_put(mmrm_test_clk_client_list[index][i].clk_client_desc.clk);
			mmrm_test_clk_client_list[index][i].clk_client_desc.clk = NULL;
		}
	}

	pr_info("%s: Finish test case %d\n", __func__, test);
}

void test_mmrm_client_cases(struct platform_device *pdev, int index, int count)
{
	int i;
	for (i = 0; i < MMRM_TEST_NUM_CASES; i++)
		test_mmrm_single_case(pdev, index, count, i);
}
