# Build audio kernel driver
ifneq ($(TARGET_USES_QMAA),true)
ifneq ($(TARGET_BOARD_AUTO),true)
ifeq ($(call is-board-platform-in-list,$(TARGET_BOARD_PLATFORM)),true)
BOARD_VENDOR_KERNEL_MODULES += $(KERNEL_MODULES_OUT)/btpower.ko\
	$(KERNEL_MODULES_OUT)/bt_fm_slim.ko \
	$(KERNEL_MODULES_OUT)/radio-i2c-rtc6226-qca.ko
endif
endif
endif
