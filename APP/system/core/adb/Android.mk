# Copyright 2005 The Android Open Source Project
#
# Android.mk for adb
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := adbd
LOCAL_SRC_FILES := $(LOCAL_MODULE)
#LOCAL_MODULE_TAGS := debug
LOCAL_MODULE_CLASS := SBIN
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
include $(BUILD_PREBUILT)

