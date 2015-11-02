# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../../../..

LOCAL_CFLAGS := $(APP_DEFINE)

CC := /opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc

LOCAL_CFLAGS += -s -Os -fomit-frame-pointer -Wall

LOCAL_SHARED_LIBS_INCLUDES := -L$(TARGETDIR)

LOCAL_C_INCLUDES:= \
	-I$(LOCAL_PATH)/external/libnl-headers

LOCAL_SRC_FILES := \
        attr.c \
        cache.c \
        genl/genl.c \
        genl/family.c \
        handlers.c \
        msg.c \
        netlink.c \
        object.c \
        socket.c \
        dbg.c

LOCAL_OBJS_FILES := $(notdir $(LOCAL_SRC_FILES:.c=.o))

LOCAL_SHARED_LIBRARIES := -liblog

LOCAL_MODULE := libnl_2.so

all: 
	$(CC) $(LOCAL_CFLAGS) -fPIC -shared $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	
.PHONY:clean
clean:
	-rm -f *.a *.o $(LOCAL_MODULE)






		

		










