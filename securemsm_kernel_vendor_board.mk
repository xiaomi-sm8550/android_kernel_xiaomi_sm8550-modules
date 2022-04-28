BOARD_VENDOR_KERNEL_MODULES += $(KERNEL_MODULES_OUT)/tz_log_dlkm.ko \
      $(KERNEL_MODULES_OUT)/qcedev-mod_dlkm.ko \
      $(KERNEL_MODULES_OUT)/qcrypto-msm_dlkm.ko \
      $(KERNEL_MODULES_OUT)/qce50_dlkm.ko \
      $(KERNEL_MODULES_OUT)/hdcp_qseecom_dlkm.ko \
      $(KERNEL_MODULES_OUT)/qrng_dlkm.ko \

ifeq ($(TARGET_BOARD_AUTO),true)
BOARD_VENDOR_KERNEL_MODULES += $(KERNEL_MODULES_OUT)/qseecom_dlkm.ko
else
BOARD_VENDOR_KERNEL_MODULES += $(KERNEL_MODULES_OUT)/smcinvoke_dlkm.ko
endif
