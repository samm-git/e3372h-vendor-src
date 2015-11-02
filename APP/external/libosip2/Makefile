# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../..

LOCAL_CFLAGS := $(APP_DEFINE) -DHAVE_CONFIG_H
LOCAL_CFLAGS += -DLINUX_OS -DNEW_TIMER -Wall -Wcast-align -Wchar-subscripts -Wformat -Winline -Wmissing-declarations -Wmissing-prototypes -Wnested-externs -Wpointer-arith
LOCAL_CFLAGS += -s -Os -fomit-frame-pointer -Wall

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/src/osipparser2/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/src/osip2/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/include/osip2/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/include/osipparser2/


LOCAL_SHARED_LIBS_INCLUDES := 
#-L$(TARGETDIR) -lpublicapi -lnetplatform -lcl_xml -lipc

LOCAL_SRC_FILES := src/osipparser2/osip_content_encoding.c src/osipparser2/osip_proxy_authorization.c 
LOCAL_SRC_FILES += src/osipparser2/osip_accept_encoding.c src/osipparser2/osip_cseq.c  src/osipparser2/osip_record_route.c
LOCAL_SRC_FILES += src/osipparser2/osip_accept_language.c src/osipparser2/osip_accept.c src/osipparser2/osip_route.c  
LOCAL_SRC_FILES += src/osipparser2/osip_alert_info.c src/osipparser2/osip_error_info.c src/osipparser2/osip_to.c   
LOCAL_SRC_FILES += src/osipparser2/osip_allow.c src/osipparser2/osip_from.c src/osipparser2/osip_uri.c 
LOCAL_SRC_FILES += src/osipparser2/osip_authorization.c src/osipparser2/osip_header.c src/osipparser2/osip_via.c 
LOCAL_SRC_FILES += src/osipparser2/osip_body.c src/osipparser2/osip_md5c.c src/osipparser2/osip_www_authenticate.c
LOCAL_SRC_FILES += src/osipparser2/osip_call_info.c src/osipparser2/osip_message.c src/osipparser2/osip_list.c 
LOCAL_SRC_FILES += src/osipparser2/osip_call_id.c src/osipparser2/osip_message_parse.c   
LOCAL_SRC_FILES += src/osipparser2/osip_contact.c src/osipparser2/osip_message_to_str.c src/osipparser2/osip_port.c 
LOCAL_SRC_FILES += src/osipparser2/osip_content_disposition.c src/osipparser2/osip_mime_version.c src/osipparser2/sdp_message.c 
LOCAL_SRC_FILES += src/osipparser2/osip_content_length.c src/osipparser2/osip_parser_cfg.c src/osipparser2/sdp_accessor.c
LOCAL_SRC_FILES += src/osipparser2/osip_content_type.c src/osipparser2/osip_proxy_authenticate.c src/osipparser2/osip_rfc3264.c 
LOCAL_SRC_FILES += src/osip2/ict_fsm.c src/osip2/ist_fsm.c src/osip2/nict_fsm.c src/osip2/nist_fsm.c src/osip2/ict.c src/osip2/ist.c src/osip2/nict.c src/osip2/nist.c
LOCAL_SRC_FILES += src/osip2/fsm_misc.c src/osip2/osip.c src/osip2/osip_transaction.c src/osip2/osip_event.c src/osip2/port_sema.c src/osip2/port_fifo.c
LOCAL_SRC_FILES += src/osip2/port_thread.c src/osip2/osip_dialog.c src/osip2/osip_negotiation.c src/osip2/port_condv.c src/osip2/osip_time.c

LOCAL_MODULE := libosip2.so

all:
	$(CC) $(LOCAL_CFLAGS) -fPIC -shared $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	mv $(LOCAL_MODULE) $(TARGETDIR)

.PHONY:clean
clean:
	rm -f *.a src/osip2/*.o src/osipparser2/*.o $(LOCAL_MODULE) $(TARGETDIR)/$(LOCAL_MODULE)

