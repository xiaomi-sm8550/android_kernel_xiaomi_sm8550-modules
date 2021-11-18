MM_DRIVER_PATH := $(call my-dir)
include $(MM_DRIVER_PATH)/msm_ext_display/Android.mk
include $(MM_DRIVER_PATH)/hw_fence/Android.mk
ifneq ($(TARGET_BOARD_PLATFORM), taro)
include $(MM_DRIVER_PATH)/sync_fence/Android.mk
endif

