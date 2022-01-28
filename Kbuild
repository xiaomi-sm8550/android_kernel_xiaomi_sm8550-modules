include $(SSG_MODULE_ROOT)/config/ssg_smcinvoke.conf

LINUXINCLUDE += -I$(SSG_MODULE_ROOT)/ \
                -I$(SSG_MODULE_ROOT)/linux/

KBUILD_CPPFLAGS += -DCONFIG_HDCP_QSEECOM

obj-$(CONFIG_QCOM_SMCINVOKE) += smcinvoke_dlkm.o
smcinvoke_dlkm-objs := smcinvoke/smcinvoke_kernel.o smcinvoke/smcinvoke.o

obj-$(CONFIG_QTI_TZ_LOG) += tz_log_dlkm.o
tz_log_dlkm-objs := tz_log/tz_log.o

obj-$(CONFIG_CRYPTO_DEV_QCEDEV) += qce50_dlkm.o
qce50_dlkm-objs := crypto-qti/qce50.o

obj-$(CONFIG_CRYPTO_DEV_QCOM_MSM_QCE) += qcedev-mod_dlkm.o
qcedev-mod_dlkm-objs := crypto-qti/qcedev.o crypto-qti/qcedev_smmu.o crypto-qti/compat_qcedev.o

obj-$(CONFIG_CRYPTO_DEV_QCRYPTO) += qcrypto-msm_dlkm.o
qcrypto-msm_dlkm-objs := crypto-qti/qcrypto.o 

obj-$(CONFIG_HDCP_QSEECOM) += hdcp_qseecom_dlkm.o
hdcp_qseecom_dlkm-objs := hdcp/hdcp_qseecom.o

obj-$(CONFIG_MSM_TMECOM_QMP) := tmecom-intf_dlkm.o
tmecom-intf_dlkm-objs := tmecom/tmecom.o tmecom/tme_hwkm_master.o
