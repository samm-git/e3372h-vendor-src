#
# Makefile for ddr test drivers
#
include $(BALONG_TOPDIR)/build/scripts/make_base.mk
EXTRA_CFLAGS += -fno-pic 
LINUXROOT =$(BALONG_TOPDIR)/modem/system/android/android_4.2_r1/out/target/product/balongv7r2/obj/KERNEL_OBJ/
ifeq ($(strip $(MBB_HWTEST)),FEATURE_ON)
obj-m		+= ddrtest.o 
ddrtest-objs += ddr_tester.o \
				ddr_alg.o \
				ddr_cache.o 
all:
	#@echo "ARCH:$(ARCH)"
	#@echo "CROSS_COMPILE:$(CROSS_COMPILE)"
	$(MAKE) -C $(LINUXROOT) M=$(shell pwd) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
	#$(CROSS_COMPILE)strip *.ko

clean:
	rm -rf   *.o *.ko .*cmd *.mod.c *.order .tmp_versions Module.symvers

tmp_clr:
	rm -rf   *.o .*cmd *.mod.c *.order .tmp_versions Module.symvers
endif

