# SPDX-License-Identifier: GPL-2.0-only

PRODUCT_PACKAGES += msm_ext_display.ko

ifneq ($(TARGET_BOARD_PLATFORM), taro)
PRODUCT_PACKAGES += sync_fence.ko
endif
