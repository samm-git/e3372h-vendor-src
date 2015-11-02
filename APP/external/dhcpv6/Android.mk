#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)
BIONIC_PATH := $(call my-dir)/../../

#############################################################
# Build dhcp6c utils 
#

ifneq ($(findstring MBB_FEATURE_EUAP, $(APP_DEFINE)),)
NETPLATFORM_INC := $(TARGET_OUT_HEADERS)
else
NETPLATFORM_INC := $(LOCAL_PATH)/../../..
endif

sysconfdir= /var/dhcp6
localdbdir= /var/db
#BEGIN 2110706738 y00188255 2012-11-7 modified
L_CFLAGS := $(APP_DEFINE)
#END 2110706738 y00188255 2012-11-7 modified
L_CFLAGS += -DSYSLOG
L_CFLAGS += -DHGW_SOFTVERSION=\"$(HGW_SOFTVERSION)\"
L_CFLAGS += -g -O2 -I$(srcdir)  \
				-DPACKAGE_NAME=\"\"  \
				-DPACKAGE_TARNAME=\"\" \
				-DPACKAGE_VERSION=\"\" \
				-DPACKAGE_STRING=\"\" \
				-DPACKAGE_BUGREPORT=\"\" \
				-DYYTEXT_POINTER=1 \
				-DHAVE_GETADDRINFO=1 \
				-DHAVE_GETNAMEINFO=1 \
				-DHAVE_GETIFADDRS=1 \
				-DHAVE_IF_NAMETOINDEX=1 \
				-DHAVE_DAEMON=1 \
				-DHAVE_WARNX=1 \
				-DSTDC_HEADERS=1 \
				-DHAVE_SYS_TYPES_H=1 \
				-DHAVE_SYS_STAT_H=1 \
				-DHAVE_STDLIB_H=1 \
				-DHAVE_STRING_H=1 \
				-DHAVE_MEMORY_H=1 \
				-DHAVE_STRINGS_H=1 \
				-DHAVE_INTTYPES_H=1 \
				-DHAVE_STDINT_H=1 \
				-DHAVE_UNISTD_H=1 \
				-DHAVE_FCNTL_H=1 \
				-DHAVE_SYS_IOCTL_H=1 \
				-DHAVE_SYS_TIME_H=1 \
				-DHAVE_SYSLOG_H=1 \
				-DHAVE_UNISTD_H=1 \
				-DHAVE_IFADDRS_H=1 \
				-DTIME_WITH_SYS_TIME=1 \
				-DHAVE_STRUCT_TM_TM_ZONE=1 \
				-DHAVE_TM_ZONE=1 \
				-DHAVE_SIG_ATOMIC_T=1 \
				-DGETPGRP_VOID=1 \
				-DSETPGRP_VOID=1 \
				-DRETSIGTYPE=void \
				-DHAVE_MKTIME=1 \
				-DHAVE_SELECT=1 \
				-DHAVE_SOCKET=1 \
				-DHAVE_ANSI_FUNC=1 \
				-DHAVE_TAILQ_FOREACH_REVERSE_OLD=1 \
				-DHAVE_STDARG_H=1  \
				-DSYSCONFDIR=\"${sysconfdir}\" \
				-DLOCALDBDIR=\"${localdbdir}\" \
				-Ddprintf \
				-I $(BIONIC_PATH)/bionic/libc/include \
				-I $(NETPLATFORM_INC)/netplatform/netplatform/include \
				-I $(NETPLATFORM_INC)/include/ \
             -I $(NETPLATFORM_INC)/netplatforminc/ \
				-include $(NETPLATFORM_INC)/netplatforminc/netprotoapi.h \
				-include missing/sys/queue.h \
				-include missing/err.h
				
#############################################################
# Build dhcp6c utils 
#
include $(CLEAR_VARS)
LOCAL_MODULE:= dhcp6c

LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/missing
LOCAL_C_INCLUDES += $(NETPLATFORM_INC)/netplatform/offload/include	
LOCAL_SRC_FILES := dhcp6c.c common.c config.c prefixconf.c dhcp6c_ia.c timer.c \
	dhcp6c_script.c if.c base64.c auth.c dhcp6_ctl.c addrconf.c lease.c misc.c \
	dhcp6cinfo.c sockutils.c ra.c raopt.c ratlv.c addrstate.c missing/warnx.c \
	missing/arc4random.c missing/strlcat.c  \
	missing/strlcpy.c cfparse.c cftoken.c missing/getifaddrs.c

LOCAL_LDLIBS := -lcutils -lc 
LOCAL_SHARED_LIBRARIES := libpublicapi libnetplatform

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

#############################################################
# Build dhcp6s utils 
#
include $(CLEAR_VARS)
LOCAL_MODULE:= dhcp6s
LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/missing
LOCAL_C_INCLUDES += $(NETPLATFORM_INC)/netplatform/offload/include	
LOCAL_SRC_FILES := dhcp6s.c common.c if.c config.c timer.c lease.c misc.c \
	base64.c auth.c dhcp6_ctl.c missing/warnx.c missing/arc4random.c  \
	 missing/strlcat.c  \
	missing/strlcpy.c cfparse.c cftoken.c missing/getifaddrs.c

LOCAL_LDLIBS := -lcutils -lc 
LOCAL_SHARED_LIBRARIES := libpublicapi
	
LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

#############################################################
# Build dhcp6relay utils 
#
include $(CLEAR_VARS)
LOCAL_MODULE:= dhcp6relay
LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/missing
LOCAL_C_INCLUDES += $(NETPLATFORM_INC)/netplatform/offload/include	
LOCAL_SRC_FILES := dhcp6relay.c dhcp6relay_script.c common.c timer.c misc.c missing/warnx.c \
	missing/arc4random.c  missing/strlcat.c  \
	missing/strlcpy.c missing/getifaddrs.c

LOCAL_LDLIBS := -lcutils -lc
LOCAL_SHARED_LIBRARIES := libpublicapi
LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

#############################################################
# Build dhcp6ctl utils 
#
include $(CLEAR_VARS)
LOCAL_MODULE:= dhcp6ctl
LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/missing
LOCAL_C_INCLUDES += $(NETPLATFORM_INC)/netplatform/offload/include	
LOCAL_SRC_FILES := dhcp6_ctlclient.c base64.c auth.c missing/warnx.c 

LOCAL_LDLIBS := -lcutils -lc 
LOCAL_SHARED_LIBRARIES := libpublicapi
LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

