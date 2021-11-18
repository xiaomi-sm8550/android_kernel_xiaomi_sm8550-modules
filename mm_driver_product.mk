# SPDX-License-Identifier: GPL-2.0-only

PRODUCT_PACKAGES += msm_ext_display.ko msm_hw_fence.ko

ifneq ($(TARGET_BOARD_PLATFORM), taro)
PRODUCT_PACKAGES += sync_fence.ko
endif
