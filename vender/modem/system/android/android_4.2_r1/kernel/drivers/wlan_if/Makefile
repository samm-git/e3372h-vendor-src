#
# Makefile for wlan_if
#

include $(BALONG_TOPDIR)/build/scripts/make_base.mk

obj-y	+= wlan_utils.o
obj-y	+= wlan_at_api.o
obj-y	+= wlan_if.o


WLAN_AT_API_CHIP1_OBJECT := wlan_at_api_stub

# mutil wifi chips support
ifneq ($(MBB_WIFI_CHIP1),FEATURE_OFF)
        WLAN_AT_API_CHIP1_OBJECT := wlan_at_api_$(MBB_WIFI_CHIP1)
endif

#reg one wifi chip or stub
obj-y	+= $(WLAN_AT_API_CHIP1_OBJECT).o

ifneq ($(MBB_WIFI_CHIP2),FEATURE_OFF)
        WLAN_AT_API_CHIP2_OBJECT := wlan_at_api_$(MBB_WIFI_CHIP2)
        obj-y	+= $(WLAN_AT_API_CHIP2_OBJECT).o
endif


