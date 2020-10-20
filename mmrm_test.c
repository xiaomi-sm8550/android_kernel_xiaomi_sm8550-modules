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
#include <linux/platform_device.h>

#include <soc/qcom/socinfo.h>

#include "mmrm_test_internal.h"

#define MODULE_NAME "mmrm_test"

static int mmrm_test_probe(struct platform_device *pdev)
{
  int soc_id;

  pr_info("%s: Validating mmrm on target: %s\n", __func__, socinfo_get_id_string());

  // Get socid to get known mmrm configurations
  soc_id = socinfo_get_id();
  switch (soc_id)
  {
  case 415:
    test_mmrm_client(pdev, MMRM_TEST_LAHAINA, MMRM_TEST_LAHAINA_NUM_CLK_CLIENTS);
    break;
  default:
    pr_info("%s: Not supported for soc_id %d [Target %s]\n", __func__, soc_id, socinfo_get_id_string());
    return -ENODEV;
  }

  return 0;
}

int mmrm_test_remove(struct platform_device *pdev)
{
  return 0;
}

static struct platform_driver mmrm_test_driver = {
    .probe = mmrm_test_probe,
    .remove = mmrm_test_remove,
    .driver = {
        .name = MODULE_NAME,
        .owner = THIS_MODULE,
    },
};

static void mmrm_test_release(struct device *dev)
{
  return;
}

static struct platform_device mmrm_test_device = {
    .name = MODULE_NAME,
    .id = -1,
    .dev = {
        .release = mmrm_test_release,
    },
};

static int __init mmrm_test_init(void)
{
  platform_device_register(&mmrm_test_device);
  return platform_driver_register(&mmrm_test_driver);
}
module_init(mmrm_test_init);

static void __exit mmrm_test_exit(void)
{
  platform_driver_unregister(&mmrm_test_driver);
  platform_device_unregister(&mmrm_test_device);
}
module_exit(mmrm_test_exit);

MODULE_DESCRIPTION("MMRM TEST");
MODULE_LICENSE("GPL v2");
