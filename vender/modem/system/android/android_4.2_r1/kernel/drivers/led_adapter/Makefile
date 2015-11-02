#
# Makefile for the kernel LED device drivers.
#

-include $(BALONG_TOPDIR)/config/product/$(OBB_PRODUCT_NAME)/config/balong_product_config.mk

obj-y	+= led_comm.o

ifeq ($(CFG_BSP_CONFIG_BOARD_MCPE),YES)
obj-y   += led_cpe.o
endif

