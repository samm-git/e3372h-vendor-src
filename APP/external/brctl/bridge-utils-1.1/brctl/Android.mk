# Copyright 2006 The Android Open Source Project

# Setting LOCAL_PATH will mess up all-subdir-makefiles, so do it beforehand.

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := libbridge

LOCAL_CFLAGS := -Wall
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/../libbridge 

ifneq ($(findstring MBB_FEATURE_EUAP, $(APP_DEFINE)),)
    LOCAL_C_INCLUDES+= $(TARGET_OUT_HEADERS)/netplatforminc
else
    LOCAL_C_INCLUDES+= $(LOCAL_PATH)/../../../../../netplatforminc
endif

LOCAL_SRC_FILES := brctl.c brctl_cmd.c brctl_disp.c

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := debug

LOCAL_MODULE:= brctl

include $(BUILD_EXECUTABLE)
