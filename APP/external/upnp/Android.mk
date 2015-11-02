ifneq ($(findstring FEATURE_HUAWEI_MBB_UPNP, $(APP_DEFINE)),)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := eng

include $(LOCAL_PATH)/upnp_proj.min

#APP_DEFINE += -DFEATURE_HUAWEI_MBB_PNPX
#APP_DEFINE += -DVODAFONE_FEATURE
LOCAL_CFLAGS := $(APP_DEFINE)

LOCAL_SRC_FILES:= $(addprefix src/,$(WEBSDK_UPNP_SOURCES))

ifneq ($(findstring MBB_FEATURE_EUAP, $(APP_DEFINE)),)
LOCAL_C_INCLUDES := $(TARGET_OUT_HEADERS)/include
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/netplatforminc
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/netplatform/netplatform/include
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/netplatform/lib/libxml/xml
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/notify
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/config
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/log
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/os_adapter
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../approot/src/linux/syswatch_api
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/router
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/statistic
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/upnp_lib
ifneq ($(findstring FEATURE_HUAWEI_MBB_PNPX, $(APP_DEFINE)),)
    LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/ats
endif
ifneq ($(findstring MBB_FEATURE_PCP, $(APP_DEFINE)),)
    LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/pcp/inc
endif
else
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../netplatforminc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../netplatform/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lib/libxml/xml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../notify
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../config
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../log
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../os_adapter
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../approot/src/linux/syswatch_api
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../router
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../statistic
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../upnp_lib
ifneq ($(findstring FEATURE_HUAWEI_MBB_PNPX, $(APP_DEFINE)),)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../ats
endif
ifneq ($(findstring MBB_FEATURE_PCP, $(APP_DEFINE)),)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../pcp/inc
endif
endif

LOCAL_SHARED_LIBRARIES := libwebsdk libpublicapi libwebsdk_upnp
LOCAL_SHARED_LIBRARIES += libnetplatform libcl_xml

LOCAL_MODULE:= miniupnpd
include $(BUILD_EXECUTABLE)
else
endif