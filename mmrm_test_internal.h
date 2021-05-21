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

#ifndef TEST_MMRM_TEST_INTERNAL_H_
#define TEST_MMRM_TEST_INTERNAL_H_

#include <linux/platform_device.h>
#include <linux/soc/qcom/msm_mmrm.h>

#define MMRM_TEST_LAHAINA 0
#define MMRM_TEST_LAHAINA_NUM_CLK_CLIENTS 23
#define MMRM_TEST_WAIPIO 1
#define MMRM_TEST_WAIPIO_NUM_CLK_CLIENTS 28

struct mmrm_test_desc {
	struct mmrm_test_clk_client  *clk_client;
	u32 clk_rate_id;
};

extern struct  mmrm_test  *all_lahaina_testcases[];
extern struct  mmrm_test_desc  *waipio_all_testcases[];

void test_mmrm_client(struct platform_device *pdev, int index, int count);
void test_mmrm_single_client_cases(struct platform_device *pdev,
					int index, int count);
void test_mmrm_concurrent_client_cases(struct platform_device *pdev,
					struct mmrm_test_desc **testcases);


#endif  // TEST_MMRM_TEST_INTERNAL_H_
