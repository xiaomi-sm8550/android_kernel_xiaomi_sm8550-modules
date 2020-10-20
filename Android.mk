ifneq (,$(filter $(QCOM_BOARD_PLATFORMS),$(TARGET_BOARD_PLATFORM)))
ifneq (,$(filter aarch64 arm64, $(TARGET_ARCH)))

# Supported targets
MMRM_SUPPORT_TARGETS := lahaina
ifeq ($(call is-board-platform-in-list, $(MMRM_SUPPORT_TARGETS)),true)
LOCAL_PATH := $(call my-dir)

# DLKM
ifeq ($(TARGET_SUPPORTS_WEARABLES),true)
DLKM_DIR   := $(BOARD_DLKM_DIR)
else
DLKM_DIR   := $(TOP)/device/qcom/common/dlkm
endif

# Test module
include $(CLEAR_VARS)
LOCAL_MODULE      := mmrm_test_module.ko
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/kernel-tests/modules
include $(DLKM_DIR)/AndroidKernelModule.mk

# Test script
include $(CLEAR_VARS)
LOCAL_SRC_FILES    := mmrm_test.sh
LOCAL_MODULE       := mmrm_test.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_TAGS  := optional
LOCAL_MODULE_PATH  := $(TARGET_OUT_DATA)/kernel-tests
include $(BUILD_PREBUILT)

endif
endif
endif
