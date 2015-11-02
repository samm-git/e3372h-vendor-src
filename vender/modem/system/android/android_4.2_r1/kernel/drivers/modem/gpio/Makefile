# generic gpio support: platform drivers, dedicated expander chips, etc
-include $(BALONG_TOPDIR)/config/product/$(OBB_PRODUCT_NAME)/config/balong_product_config.mk

obj-$(CONFIG_GPIO_BALONG)	       += gpio_balong.o

ifeq ($(strip $(CFG_CONFIG_GPIO_EXPANDER)),YES)
obj-y += gpio_expander_balong.o
endif

obj-$(CONFIG_ENABLE_TEST_CODE)     += gpio_balong_test.o

