# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../..

LOCAL_CFLAGS := $(APP_DEFINE) 
LOCAL_CFLAGS += -D_GNU_SOURCE -DBUILDSTR="\"`cat src/.buildno`\""
LOCAL_CFLAGS += -s -Os -fomit-frame-pointer

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../libosip2/include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../libosip2/include/osip2/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../libosip2/include/osipparser2/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../libosip2/include/osipparser2/headers/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../netplatforminc 


LOCAL_SHARED_LIBS_INCLUDES :=-lpthread -L$(TARGETDIR) -losip2 -pthread 

LOCAL_SRC_FILES := src/siproxd.c src/proxy.c src/register.c src/sock.c src/utils.c src/sip_utils.c src/log.c src/readconf.c src/rtpproxy.c 
LOCAL_SRC_FILES += src/rtpproxy_relay.c src/accessctl.c src/route_processing.c src/security.c src/auth.c src/fwapi.c  src/custom_fw_module.c

LOCAL_MODULE := siproxd

all:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	mv $(LOCAL_MODULE) $(TARGETDIR)

.PHONY:clean
clean:
	rm -f *.a src/*.o $(LOCAL_MODULE) $(TARGETDIR)/$(LOCAL_MODULE)

