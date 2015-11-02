#
# Android.mk for memtester tools, it is memtester.c
#
LOCAL_PATH:= $(call my-dir)

# memtester
# =========================================================
include $(CLEAR_VARS)

LOCAL_MODULE			:= memtester
LOCAL_SRC_FILES			:= memtester.c tests.c freq.c add_ota_test.c log.c

LOCAL_MODULE_PATH 		:= $(TARGET_ROOT_OUT_SBIN)
LOCAL_STATIC_LIBRARIES 		:=libcutils libc
LOCAL_CFLAGS 			:= -O2 -lpthread -DPOSIX -D_POSIX_C_SOURCE=200809L -D_FILE_OFFSET_BITS=64
LOCAL_MODULE_TAGS := eng
LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)

#$(call dist-for-goals,dist_files sdk,$(LOCAL_BUILT_MODULE))