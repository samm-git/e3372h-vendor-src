-include $(BALONG_TOPDIR)/config/product/$(OBB_PRODUCT_NAME)/config/balong_product_config.mk

ifeq ($(strip $(MBB_MLOG)),FEATURE_ON)
obj-y	+= mlogdrv/
endif
ifeq ($(strip $(MBB_DLOAD)),FEATURE_ON)
obj-y     += dload_com/
endif



