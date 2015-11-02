#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#########################################
#LOCAL_CFLAGS := $(DFLAGS)
#LOCAL_CFLAGS += $(EC508_CFLAGS)

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/

#LOCAL_SRC_FILES:= custom_fw_module.c

#LOCAL_MODULE:= libcustom_fw_module

#include $(BUILD_STATIC_LIBRARY)

#########################################
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(DFLAGS)
LOCAL_CFLAGS += $(EC508_CFLAGS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include/osip2/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include/osipparser2/

LOCAL_STATIC_LIBRARIES :=  

LOCAL_SRC_FILES := osip_content_encoding.c osip_proxy_authorization.c 
LOCAL_SRC_FILES += osip_accept_encoding.c osip_cseq.c  osip_record_route.c
LOCAL_SRC_FILES += osip_accept_language.c osip_accept.c osip_route.c  
LOCAL_SRC_FILES += osip_alert_info.c osip_error_info.c osip_to.c   
LOCAL_SRC_FILES += osip_allow.c osip_from.c osip_uri.c 
LOCAL_SRC_FILES += osip_authorization.c osip_header.c osip_via.c 
LOCAL_SRC_FILES += osip_body.c osip_md5c.c osip_www_authenticate.c
LOCAL_SRC_FILES += osip_call_info.c osip_message.c osip_list.c 
LOCAL_SRC_FILES += osip_call_id.c osip_message_parse.c   
LOCAL_SRC_FILES += osip_contact.c osip_message_to_str.c osip_port.c 
LOCAL_SRC_FILES += osip_content_disposition.c osip_mime_version.c sdp_message.c 
LOCAL_SRC_FILES += osip_content_length.c osip_parser_cfg.c sdp_accessor.c
LOCAL_SRC_FILES += osip_content_type.c osip_proxy_authenticate.c osip_rfc3264.c 

LOCAL_SHARED_LIBRARIES := 

LOCAL_MODULE_TAGS := eng
LOCAL_MODULE:= libosipparser2

include $(BUILD_STATIC_LIBRARY)
