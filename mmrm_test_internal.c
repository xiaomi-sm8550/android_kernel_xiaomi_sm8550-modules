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

#define MMRM_TEST_MAX_CLK_CLIENTS 5

struct mmrm_test_clk_client
{
  struct mmrm_clk_client_desc clk_client_desc;
  unsigned long svsl1;
  unsigned long nom;
};

static struct mmrm_test_clk_client mmrm_test_clk_client_list[][MMRM_TEST_MAX_CLK_CLIENTS] = {
    /* LAHAINA */
    {
        {{MMRM_CLIENT_DOMAIN_CAMERA, 42, "CAM_CC_IFE_0_CLK_SRC", NULL}, 675, 785},
        {{MMRM_CLIENT_DOMAIN_CAMERA, 51, "CAM_CC_IFE_1_CLK_SRC", NULL}, 675, 785},
        {{MMRM_CLIENT_DOMAIN_CVP, 8, "VIDEO_CC_MVS1_CLK_SRC", NULL}, 1500, 1650},
        {{MMRM_CLIENT_DOMAIN_DISPLAY, 41, "DISP_CC_MDSS_MDP_CLK_SRC", NULL}, 375, 500},
        {{MMRM_CLIENT_DOMAIN_VIDEO, 3, "VIDEO_CC_MVS0_CLK_SRC", NULL}, 1098, 1332},
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

  if (!desc)
  {
    pr_info("%s: Invalid input\n", __func__);
    return NULL;
  }

  pr_info("%s: domain(%d) cid(%d) name(%s) type(%d) pri(%d) pvt(%p) notifier(%p)\n",
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
  if (client == NULL)
  {
    pr_info("%s: Failed to register mmrm client %s\n", __func__, desc->client_info.desc.name);
    return NULL;
  }
  pr_info("%s: cuid(%d)\n", __func__, client->client_uid);
  return client;
}

static int test_mmrm_client_deregister(struct mmrm_client *client)
{
  int rc;

  if (!client)
  {
    pr_info("%s: Invalid input\n", __func__);
    return -EINVAL;
  }

  pr_info("%s: cuid(%d) Deregistering mmrm client\n", __func__, client->client_uid);
  rc = mmrm_client_deregister(client);
  if (rc != 0)
  {
    pr_info("%s: cuid(%d) Failed to deregister mmrm client with %d\n", __func__, client->client_uid, rc);
    return rc;
  }
  return rc;
}

static int test_mmrm_client_set_value(struct mmrm_client *client, struct mmrm_client_data *client_data, unsigned long val)
{
  int rc;

  if (!client || !client_data)
  {
    pr_info("%s: Invalid input\n", __func__);
    return -EINVAL;
  }

  pr_info("%s: cuid(%d) Setting value for mmrm client\n", __func__, client->client_uid);
  rc = mmrm_client_set_value(client, client_data, val);
  if (rc != 0)
  {
    pr_info("%s: cuid(%d) Failed to set value for mmrm client with %d\n", __func__, client->client_uid, rc);
    return rc;
  }
  return rc;
}

static int test_mmrm_client_get_value(struct mmrm_client *client, struct mmrm_client_res_value *val)
{
  int rc;

  if (!client || !val)
  {
    pr_info("%s: Invalid input\n", __func__);
    return -EINVAL;
  }

  pr_info("%s: cuid(%d) Getting value for mmrm client\n", __func__, client->client_uid);
  rc = mmrm_client_get_value(client, val);
  if (rc != 0)
  {
    pr_info("%s: cuid(%d) Failed to get value for mmrm client with %d\n", __func__, client->client_uid, rc);
    return rc;
  }
  pr_info("%s: cuid(%d) min(%d) cur(%d) max(%d)\n", __func__, client->client_uid, val->min, val->cur, val->max);
  return rc;
}

void test_mmrm_client(struct platform_device *pdev, int index, int count)
{
  struct clk *clk;                      // clk struct
  struct mmrm_client *client;           // mmrm client
  struct mmrm_client_data client_data;  // mmrm client data
  struct mmrm_client_res_value res_val; // mmrm get_val
  unsigned long val;                    // mmrm set_val

  int i, pass_count;
  int rc = 0;

  pr_info("%s: Running client tests\n", __func__);

  // Run nominal test for each individual clock source
  for (i = 0, pass_count = 0; i < count; i++)
  {
    // Create callback used to pass resource data to client
    struct mmrm_client_notifier_data notifier_data = {
        MMRM_CLIENT_RESOURCE_VALUE_CHANGE, // cb_type
        {{0, 0}},                          // cb_data (old_val, new_val)
        NULL};                             // pvt_data

    // Create client descriptor
    struct mmrm_client_desc desc = {
        MMRM_CLIENT_CLOCK,                                     // client type
        {mmrm_test_clk_client_list[index][i].clk_client_desc}, // clock client descriptor
        MMRM_CLIENT_PRIOR_HIGH,                                // client priority
        notifier_data.pvt_data,                                // pvt_data
        test_mmrm_client_callback};                            // callback fn

    // Get dummy clk
    clk = clk_get(&pdev->dev, "dummy_clk");
    desc.client_info.desc.clk = clk;

    // Register client
    client = test_mmrm_client_register(&desc);
    if (client == NULL)
    {
      rc = -EINVAL;
      goto err_register;
    }

    // Set values (Use reserve only)
    client_data = (struct mmrm_client_data){0, MMRM_CLIENT_DATA_FLAG_RESERVE_ONLY};

    val = mmrm_test_clk_client_list[index][i].svsl1;
    rc = test_mmrm_client_set_value(client, &client_data, val);
    if (rc != 0)
      goto err_setval;

    val = mmrm_test_clk_client_list[index][i].nom;
    rc = test_mmrm_client_set_value(client, &client_data, val);
    if (rc != 0)
      goto err_setval;

    // Get value
    rc = test_mmrm_client_get_value(client, &res_val);
    if (rc != 0)
      goto err_getval;

  err_setval:
  err_getval:

    // Deregister client
    rc = test_mmrm_client_deregister(client);
    if (rc != 0)
      goto err_deregister;

  err_register:
  err_deregister:

    if (rc == 0)
      pass_count++;

    // Reset clk
    desc.client_info.desc.clk = NULL;
  }

  pr_info("%s: Finish client tests (pass / total): (%d / %d)\n", __func__, pass_count, count);
}
