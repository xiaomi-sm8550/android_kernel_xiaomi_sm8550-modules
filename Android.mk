# Android makefile for audio kernel modules

#Target based of taro does not need these DLKM's as they are present as kernel drivers
#But the project is mapped for DEV SP due to dependency on smcinvoke_kernel_headers
#Hence preventing the DLKM's to be part of the taro based DEV SP
ifneq ($(TARGET_BOARD_PLATFORM), taro)
LOCAL_PATH := $(call my-dir)
DLKM_DIR := $(TOP)/device/qcom/common/dlkm

SEC_KERNEL_DIR := $(TOP)/vendor/qcom/opensource/securemsm-kernel

SSG_SRC_FILES := \
	$(wildcard $(LOCAL_PATH)/*) \
 	$(wildcard $(LOCAL_PATH)/*/*) \
 	$(wildcard $(LOCAL_PATH)/*/*/*) \
 	$(wildcard $(LOCAL_PATH)/*/*/*/*)

# This is set once per LOCAL_PATH, not per (kernel) module
KBUILD_OPTIONS := SSG_ROOT=$(SEC_KERNEL_DIR)
KBUILD_OPTIONS += BOARD_PLATFORM=$(TARGET_BOARD_PLATFORM)

###################################################
include $(CLEAR_VARS)
# For incremental compilation
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := sec-module-symvers
LOCAL_MODULE_STEM         := Module.symvers
LOCAL_MODULE_KBUILD_NAME  := Module.symvers
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
#$(error $(SSG_SRC_FILES))
include $(CLEAR_VARS)
#LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := smcinvoke_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := smcinvoke_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_HEADER_LIBRARIES    := smcinvoke_kernel_headers
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := tz_log_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := tz_log_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qce50_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qce50_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qcedev-mod_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qcedev-mod_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qcrypto-msm_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qcrypto-msm_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := hdcp_qseecom_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := hdcp_qseecom_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
###################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qrng_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qrng_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
###################################################
endif
