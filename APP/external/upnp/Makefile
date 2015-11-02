# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../..

LOCAL_CFLAGS := $(APP_DEFINE) 
LOCAL_CFLAGS += -D_GNU_SOURCE
LOCAL_CFLAGS += -s -Os -fomit-frame-pointer -Wall -Wl,-allow-shlib-undefined 

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../netplatform/include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../lib/libipc/inc
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../netplatforminc 
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../upnp_lib
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../netplatform/lib/libxml/xml


LOCAL_SHARED_LIBS_INCLUDES :=-lpthread -L$(TARGETDIR) -lpublicapi -lnetplatform -lcl_xml -lipc -lwebsdk -lpublicapi -lwebsdk_upnp

LOCAL_SRC_FILES := $(wildcard ./src/*.c)

LOCAL_MODULE := miniupnpd


all:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	mv $(LOCAL_MODULE) $(TARGETDIR)

.PHONY:clean
clean:
	rm -f *.a *.o $(LOCAL_MODULE) $(TARGETDIR)/miniupnpd
