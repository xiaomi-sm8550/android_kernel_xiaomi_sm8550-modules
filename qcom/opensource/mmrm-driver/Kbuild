
include $(MMRM_ROOT)/config/waipiommrm.conf
LINUXINCLUDE += -include $(MMRM_ROOT)/config/waipiommrmconf.h

ifneq ($(CONFIG_ARCH_QTI_VM), y)

obj-m += driver/

ifeq ($(CONFIG_MSM_MMRM_VM),y)
LINUXINCLUDE += -I$(MMRM_ROOT)/vm/common/inc/
obj-m += vm/be/
endif

else

LINUXINCLUDE += -I$(MMRM_ROOT)/vm/common/inc/

obj-m += vm/fe/

endif
