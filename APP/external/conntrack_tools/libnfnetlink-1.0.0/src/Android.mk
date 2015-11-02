#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)

version = 1.0

INC_APP_DIR = ./
L_CFLAGS := -g -O2 -Wall \
			-DPACKAGE_NAME=\"libnfnetlink\" \
			-DPACKAGE_TARNAME=\"libnfnetlink\" \
			-DPACKAGE_BUGREPORT=\"\" \
			-DPACKAGE=\"libnfnetlink\" \
			-DSTDC_HEADERS=1 \
			-DHAVE_SYS_TYPES_H=1 \
			-DHAVE_SYS_STAT_H=1 \
			-DHAVE_STDLIB_H=1 \
			-DHAVE_STRING_H=1 \
			-DHAVE_MEMORY_H=1 \
			-DHAVE_STRINGS_H=1 \
			-DHAVE_INTTYPES_H=1 \
			-DHAVE_STDINT_H=1 \
			-DHAVE_UNISTD_H=1 \
			-DHAVE_DLFCN_H=1
			
L_CFLAGS += -Wl,--allow-shlib-undefined
#############################################################
# Build radvd 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES :=iftable.c libnfnetlink.c rtnl.c

LOCAL_LDLIBS += -lc -lgplutil

LOCAL_MODULE:= libnfnetlink

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

