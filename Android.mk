# Android makefile for nfc kernel modules

# Path to DLKM make scripts
DLKM_DIR          :=  $(TOP)/device/qcom/common/dlkm

LOCAL_PATH        := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE      := nxp-nci.ko
LOCAL_MODULE_PATH := $(KERNEL_MODULES_OUT)
LOCAL_SRC_FILES   := $(wildcard $(LOCAL_PATH)/**/*) $(wildcard $(LOCAL_PATH)/*)

include $(DLKM_DIR)/Build_external_kernelmodule.mk
