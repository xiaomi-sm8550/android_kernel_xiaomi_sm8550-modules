include  $(SSG_MODULE_ROOT)/config/ssg_smcinvoke.conf

obj-m += smcinvoke_dlkm.o
smcinvoke_dlkm-objs := smcinvoke/smcinvoke_kernel.o smcinvoke/smcinvoke.o

obj-m += tz_log_dlkm.o
tz_log_dlkm-objs := tz_log/tz_log.o

obj-m += qce50_dlkm.o
qce50_dlkm-objs := crypto-qti/qce50.o

obj-m += qcedev-mod_dlkm.o
qcedev-mod_dlkm-objs := crypto-qti/qcedev.o crypto-qti/qcedev_smmu.o crypto-qti/compat_qcedev.o

obj-m += qcrypto-msm_dlkm.o
qcrypto-msm_dlkm-objs := crypto-qti/qcrypto.o 

