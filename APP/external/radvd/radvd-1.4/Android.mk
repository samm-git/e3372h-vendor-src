#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)

sysconfdir= /var/dhcp6
localdbdir= /var/db
radvd_log = /var/log/radvd.log
radvd_pid = /var/run/radvd.pid
log_facility = LOG_DAEMON
version = 1.4

INC_APP_DIR = ./
L_CFLAGS := $(EC508_CFLAGS)
L_CFLAGS += $(APP_DEFINE)
L_CFLAGS += -g -O2 -Wall \
			-DSYSLOG  \
			-DVERSION=\"$(version)\" \
			-DLOG_FACILITY=$(log_facility) \
			-DPATH_RADVD_LOG=\"$(radvd_log)\" \
			-DPATH_RADVD_PID=\"$(radvd_pid)\" \
			-DHGW_SOFTVERSION=\"$(HGW_SOFTVERSION)\" \
			-DINET6=1 \
			-Wpointer-arith  \
			-Wcast-qual \
			-Wcast-align \
			-Wstrict-prototypes \
			-Wmissing-prototypes \
			-Wmissing-declarations \
			-I$(INC_APP_DIR) \
			-include config.h \
			-include include/linux/sysctl.h
				
L_CFLAGS += -Wl,--allow-shlib-undefined
#############################################################
# Build radvd 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
ifneq ($(findstring MBB_FEATURE_EUAP, $(APP_DEFINE)),)
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/netplatforminc 
else
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../netplatforminc
endif
LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES := radvd.c timer.c send.c process.c interface.c \
	device-common.c gram.c gram.h scanner.c \
	device-linux.c  privsep-linux.c \
	getifaddrs log.c socket.c recv.c util.c getifaddrs.c

LOCAL_LDLIBS += -lpthread -lc -lgplutil

LOCAL_MODULE:= radvd

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

#############################################################
# Build radvdump 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES := radvdump.c log.c socket.c recv.c util.c

LOCAL_LDLIBS += -lpthread -lc -lgplutil

LOCAL_MODULE:= radvdump

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)
