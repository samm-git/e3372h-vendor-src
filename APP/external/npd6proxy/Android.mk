#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)
BIONIC_PATH := $(call my-dir)/../../

#############################################################
# Build npd6proxy utils 
#

sysconfdir= /var/npd6proxy
localdbdir= /var/db

L_CFLAGS := -g -O2 -Wpointer-arith  \
	        -Wcast-qual \
			-Wcast-align \
			-Wstrict-prototypes \
			-Wmissing-prototypes \
			-Wmissing-declarations \
			-I $(srcdir)  \
		    -I $(LOCAL_PATH)/
#############################################################
# Build npd6proxy utils 
#
include $(CLEAR_VARS)
LOCAL_MODULE:= npd6proxy

LOCAL_CFLAGS := $(APP_DEFINE)
LOCAL_CFLAGS += $(L_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/missing 
ifneq ($(findstring MBB_FEATURE_EUAP, $(APP_DEFINE)),)
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/netplatforminc
else  
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../netplatforminc
endif
LOCAL_SRC_FILES := main.c icmp6.c util.c ip6.c config.c getifaddrs.c tsearch.c 


LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)
