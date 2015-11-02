#
# Makefile for USB Class drivers
# (one step up from the misc category)
#

-include $(BALONG_TOPDIR)/config/product/$(OBB_PRODUCT_NAME)/config/balong_product_config.mk

ifeq ($(MBB_USB),FEATURE_ON)
KBUILD_CPPFLAGS += -Idrivers/usb/mbb_usb_unitary
endif

obj-$(CONFIG_USB_ACM)		+= cdc-acm.o
obj-$(CONFIG_USB_PRINTER)	+= usblp.o
obj-$(CONFIG_USB_WDM)		+= cdc-wdm.o
obj-$(CONFIG_USB_TMC)		+= usbtmc.o
