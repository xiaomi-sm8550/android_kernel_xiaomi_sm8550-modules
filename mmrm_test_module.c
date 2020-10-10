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

#define pr_fmt(fmt) "mmrm-test: " fmt

#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <soc/qcom/socinfo.h>
#include <linux/soc/qcom/msm_mmrm.h>

#define MODULE_NAME "mmrm_test"

// MMRM_CLIENT_DOMAIN_CAMERA
#define CAM_CC_IFE_0_CLK_SRC 45
#define CAM_CC_IFE_1_CLK_SRC 54

// MMRM_CLIENT_DOMAIN_CVP
#define VIDEO_CC_MVS1_CLK_SRC 8

// MMRM_CLIENT_DOMAIN_DISPLAY
#define DISP_CC_MDSS_MDP_CLK_SRC 43

// MMRM_CLIENT_DOMAIN_VIDEO
#define VIDEO_CC_MVS0_CLK_SRC 3

static int mmrm_test_client_callback(struct mmrm_client_notifier_data *notifier_data)
{
  return 0;
}

static int mmrm_test_client_register(void)
{
  // Client
  struct mmrm_client *client;
  int result;

  // Create callback used to pass resource data to client
  struct mmrm_client_notifier_data client_notifier_data = {
      MMRM_CLIENT_RESOURCE_VALUE_CHANGE, // cb_type
      {{0, 0}},                          // cb_data (old_val, new_val)
      NULL};                             // pvt_data

  // Create clock client descriptor
  struct mmrm_clk_client_desc clk_desc = {
      MMRM_CLIENT_DOMAIN_VIDEO, // clk domain
      VIDEO_CC_MVS0_CLK_SRC,    // clk id
      "VIDEO_CC_MVS0_CLK_SRC",  // clk name
      NULL};                    // clk struct

  // Create client descriptor
  struct mmrm_client_desc desc = {MMRM_CLIENT_CLOCK,             // client type
                                  {clk_desc},                    // clk client descriptor
                                  MMRM_CLIENT_PRIOR_HIGH,        // client priority
                                  client_notifier_data.pvt_data, // pvt_data
                                  mmrm_test_client_callback};    // callback fn

  // Print client information
  pr_info("%s: domain(%d) csid(%d) name(%s) type(%d) pri(%d) pvt(%p) notifier(%p)\n",
          __func__,
          desc.client_info.desc.client_domain,
          desc.client_info.desc.client_id,
          desc.client_info.desc.name,
          desc.client_type,
          desc.priority,
          desc.pvt_data,
          desc.notifier_callback_fn);

  // Register client
  pr_info("Registering mmrm client %s\n", desc.client_info.desc.name);
  client = mmrm_client_register(&desc);

  // Check client status
  if (client == NULL)
  {
    pr_info("Failed to register mmrm client %s\n", desc.client_info.desc.name);
    return 1;
  }

  // Deregister client
  pr_info("Deregistering mmrm client %s\n", desc.client_info.desc.name);
  result = mmrm_client_deregister(client);

  // Check client status
  if (result != 0)
  {
    pr_info("Failed to deregister mmrm client %s with %d\n", desc.client_info.desc.name, result);
    return result;
  }

  return 0;
}

static int mmrm_test_probe(struct platform_device *pdev)
{
  int result;
  pr_info("Validating mmrm on target: %s\n", socinfo_get_id_string());
  result = mmrm_test_client_register();
  return result;
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
