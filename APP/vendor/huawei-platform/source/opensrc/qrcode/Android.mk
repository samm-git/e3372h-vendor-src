# Copyright 2006 The Android Open Source Project


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := eng

LOCAL_PRELINK_MODULE := false

include $(LOCAL_PATH)/libqrencode.min
LOCAL_SRC_FILES:= $(addprefix src/, $(WEBSDK_LIBQRENCODE_SOURCES)) 

LOCAL_CFLAGS += $(APP_DEFINE) -Werror


LOCAL_MODULE:= libqrencode
include $(BUILD_SHARED_LIBRARY)