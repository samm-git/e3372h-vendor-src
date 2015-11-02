LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -Wall -g

LOCAL_SRC_FILES := \
	libbridge_devif.c \
	libbridge_if.c \
	libbridge_init.c \
	libbridge_misc.c  


LOCAL_SHARED_LIBRARIES := 
LOCAL_MODULE_TAGS := debug

LOCAL_MODULE:= libbridge

include $(BUILD_STATIC_LIBRARY)