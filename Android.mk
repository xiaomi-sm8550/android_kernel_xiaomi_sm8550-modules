# Android makefile for audio kernel modules

LOCAL_PATH := $(call my-dir)
DLKM_DIR := $(TOP)/device/qcom/common/dlkm

SSG_SRC_FILES := \
	$(wildcard $(LOCAL_PATH)/*) \
 	$(wildcard $(LOCAL_PATH)/*/*) \
 	$(wildcard $(LOCAL_PATH)/*/*/*) \
 	$(wildcard $(LOCAL_PATH)/*/*/*/*)

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
##################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := tz_log_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := tz_log_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################
##################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qce50_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qce50_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################
##################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qcedev-mod_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qcedev-mod_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################
##################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qcrypto-msm_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qcrypto-msm_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################
#################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := hdcp_qseecom_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := hdcp_qseecom_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################
#################################################
include $(CLEAR_VARS)
LOCAL_SRC_FILES           := $(SSG_SRC_FILES)
LOCAL_MODULE              := qrng_dlkm.ko
LOCAL_MODULE_KBUILD_NAME  := qrng_dlkm.ko
LOCAL_MODULE_TAGS         := optional
LOCAL_MODULE_DEBUG_ENABLE := true
LOCAL_MODULE_PATH         := $(KERNEL_MODULES_OUT)
include $(DLKM_DIR)/Build_external_kernelmodule.mk
#################################################