-include $(BALONG_TOPDIR)/config/product/$(OBB_PRODUCT_NAME)/config/balong_product_config.mk
obj-y	+= huawei-charger.o
ifeq ($(strip $(MBB_CHG_BQ27510)),FEATURE_ON)
obj-y	+=	bq27510/
endif

ifeq ($(strip $(MBB_CHG_COULOMETER)),FEATURE_ON)
#for coul
obj-y	+=	coul/
endif

obj-y	+=	chg_charge_task.o
obj-y	+=	chg_charge_stm.o
obj-y	+=	chg_charge_api.o
obj-y	+=	platform/
obj-y	+=	drv/
obj-y	+=	os_adapter/
obj-y	+=	hardware_data/
