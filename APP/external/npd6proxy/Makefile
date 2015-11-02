# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../..

LOCAL_CFLAGS := $(APP_DEFINE) 
LOCAL_CFLAGS += -D_GNU_SOURCE
LOCAL_CFLAGS += -s -Os -fomit-frame-pointer

LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../offload/include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../include
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../netplatforminc 


LOCAL_SHARED_LIBS_INCLUDES :=-lpthread 

LOCAL_SRC_FILES := main.c icmp6.c util.c ip6.c config.c getifaddrs.c tsearch.c 

LOCAL_MODULE := npd6proxy


all:
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	mv $(LOCAL_MODULE) $(TARGETDIR)
install:
	mv $(LOCAL_MODULE) $(TARGETDIR)
.PHONY:clean
clean:
	rm -f *.a *.o $(LOCAL_MODULE)  $(TARGETDIR)/$(LOCAL_MODULE)

