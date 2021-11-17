#Makefile for qti nfc drivers

include $(NFC_ROOT)/config/gki_nfc.conf

LINUXINCLUDE    += -I$(NFC_ROOT)/include/uapi/linux/nfc

LINUXINCLUDE   += -include $(NFC_ROOT)/config/gki_nfc_conf.h

obj-$(CONFIG_NFC_QTI_I2C) += nfc_i2c.o

#source files
nfc_i2c-objs += qti/ese_cold_reset.o \
                qti/nfc_common.o \
                qti/nfc_i2c_drv.o

