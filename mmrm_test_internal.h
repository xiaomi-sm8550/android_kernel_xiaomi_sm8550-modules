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

#ifndef _MMRM_TEST_INTERNAL_H_
#define _MMRM_TEST_INTERNAL_H_

#include <linux/platform_device.h>
#include <linux/soc/qcom/msm_mmrm.h>

#define MMRM_TEST_LAHAINA 0
#define MMRM_TEST_LAHAINA_NUM_CLK_CLIENTS 23
#define MMRM_TEST_WAIPIO 1
#define MMRM_TEST_WAIPIO_NUM_CLK_CLIENTS 28

void test_mmrm_client(struct platform_device *pdev, int index, int count);
void test_mmrm_client_cases(struct platform_device *pdev, int index, int count);

#endif //_MMRM_TEST_INTERNAL_H_