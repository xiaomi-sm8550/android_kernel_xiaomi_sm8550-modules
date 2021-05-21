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

#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>

#include <soc/qcom/socinfo.h>

#include "mmrm_test_internal.h"

#define MODULE_NAME "mmrm_test"

static int mmrm_test_probe(struct platform_device *pdev)
{
	int soc_id;

	// Check if of_node is found
	if (!of_device_is_compatible(pdev->dev.of_node, "qcom,msm-mmrm-test")) {
		dev_info(&pdev->dev, "No compatible device node\n");
		return 1;
	}

	dev_info(&pdev->dev, "%s: Validating mmrm on target: %s\n", __func__, socinfo_get_id_string());

	// Get socid to get known mmrm configurations
	soc_id = socinfo_get_id();
	switch (soc_id) {
	case 415: /* LAHAINA */
		test_mmrm_client(pdev, MMRM_TEST_LAHAINA, MMRM_TEST_LAHAINA_NUM_CLK_CLIENTS);
		test_mmrm_single_client_cases(pdev, MMRM_TEST_LAHAINA,
								MMRM_TEST_LAHAINA_NUM_CLK_CLIENTS);
//		test_mmrm_concurrent_client_cases(pdev, all_lahaina_testcases);
		break;
	case 457: /* WAIPIO */
		test_mmrm_client(pdev, MMRM_TEST_WAIPIO, MMRM_TEST_WAIPIO_NUM_CLK_CLIENTS);
		test_mmrm_single_client_cases(pdev, MMRM_TEST_WAIPIO,
								MMRM_TEST_WAIPIO_NUM_CLK_CLIENTS);
		test_mmrm_concurrent_client_cases(pdev, waipio_all_testcases);
		break;
	default:
		pr_info("%s: Not supported for soc_id %d [Target %s]\n",
			__func__,
			soc_id,
			socinfo_get_id_string());
		return -ENODEV;
	}

	return 0;
}

int mmrm_test_remove(struct platform_device *pdev) { return 0; }

static const struct of_device_id mmrm_test_dt_match[] = {
	{.compatible = "qcom,msm-mmrm-test"}, {} // empty
};

static struct platform_driver mmrm_test_driver = {
	.probe = mmrm_test_probe,
	.remove = mmrm_test_remove,
	.driver = {
			.name = MODULE_NAME,
			.owner = THIS_MODULE,
			.of_match_table = mmrm_test_dt_match,
		},
};

static int __init mmrm_test_init(void)
{
	int rc = 0;
	rc = platform_driver_register(&mmrm_test_driver);
	if (rc) {
		pr_info("%s: failed to register platform driver\n", __func__);
	}
	return rc;
}
module_init(mmrm_test_init);

static void __exit mmrm_test_exit(void)
{
	platform_driver_unregister(&mmrm_test_driver);
}
module_exit(mmrm_test_exit);

MODULE_DESCRIPTION("MMRM TEST");
MODULE_LICENSE("GPL v2");
