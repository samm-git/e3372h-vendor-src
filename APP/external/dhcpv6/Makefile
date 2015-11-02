#
#3062604657  w00208043  2013/08/02        清理路由归一化代码屏幕打印和无用代码 
#

# CC = arm-none-linux-gnueabi-gcc

LOCAL_PATH := $(shell pwd)
SOURCE_ROOT = $(LOCAL_PATH)/../..

LOCAL_CFLAGS := $(APP_DEFINE)
#BEGIN 3080204628 , w00208043, 2013-08-02 modified
LOCAL_CFLAGS += -DSYSLOG -DNOCONFIG_DEBUG
#BEGIN 3080204628 , w00208043, 2013-08-02 modified
LOCAL_CFLAGS += -DHGW_SOFTVERSION=\"$(HGW_SOFTVERSION)\"
LOCAL_CFLAGS += -g -O2 	-g -O2 -I$(srcdir) $(INC_DIR) -DPACKAGE_NAME  -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DYYTEXT_POINTER=1 -DHAVE_GETADDRINFO=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GETIFADDRS=1 -DHAVE_IF_NAMETOINDEX=1 -DHAVE_DAEMON=1 -DHAVE_WARNX=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_FCNTL_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYSLOG_H=1 -DHAVE_UNISTD_H=1 -DHAVE_IFADDRS_H=1 -DTIME_WITH_SYS_TIME=1 -DHAVE_STRUCT_TM_TM_ZONE=1 -DHAVE_TM_ZONE=1 -DHAVE_SIG_ATOMIC_T=1 -DGETPGRP_VOID=1 -DSETPGRP_VOID=1 -DRETSIGTYPE=void -DHAVE_MKTIME=1 -DHAVE_SELECT=1 -DHAVE_SOCKET=1 -DHAVE_ANSI_FUNC=1 -DHAVE_TAILQ_FOREACH_REVERSE_OLD=1 -DHAVE_STDARG_H=1  -DSYSCONFDIR=\"${sysconfdir}\" \
	-DLOCALDBDIR=\"${localdbdir}\"

				
LOCAL_C_INCLUDES := -I$(LOCAL_PATH)/
ifeq ($(findstring FEATURE_HUAWEI_MBB_9615, $(APP_DEFINE)),)
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/missing
endif
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../netplatform/include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../include/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../../netplatforminc/
LOCAL_C_INCLUDES += -I$(LOCAL_PATH)/../../offload/include
#############################################################
# Build dhcp6c utils 
#	

LOCAL_SHARED_LIBS_INCLUDES := -lpthread -L$(TARGETDIR) -lpublicapi -lnetplatform -lcl_xml -lipc -lc 

ifneq ($(findstring FEATURE_HUAWEI_MBB_9615, $(APP_DEFINE)),)
LOCAL_SRC_FILES:= dhcp6c.c common.c config.c prefixconf.c dhcp6c_ia.c timer.c \
	dhcp6c_script.c if.c base64.c auth.c dhcp6_ctl.c addrconf.c lease.c misc.c \
	dhcp6cinfo.c sockutils.c ra.c raopt.c ratlv.c addrstate.c \
	missing/arc4random.c missing/strlcat.c  \
	missing/strlcpy.c cfparse.c cftoken.c

LOCAL_SRV_SRC_FILES:= dhcp6s.c common.c if.c config.c timer.c lease.c misc.c \
	base64.c auth.c dhcp6_ctl.c missing/arc4random.c \
	missing/strlcat.c missing/strlcpy.c cfparse.c cftoken.c
else
LOCAL_SRC_FILES:= dhcp6c.c common.c config.c prefixconf.c dhcp6c_ia.c timer.c \
	dhcp6c_script.c if.c base64.c auth.c dhcp6_ctl.c addrconf.c lease.c misc.c \
	dhcp6cinfo.c sockutils.c ra.c raopt.c ratlv.c addrstate.c missing/warnx.c \
	missing/arc4random.c missing/strlcat.c  \
	missing/strlcpy.c cfparse.c cftoken.c missing/getifaddrs.c

LOCAL_SRV_SRC_FILES:= dhcp6s.c common.c if.c config.c timer.c lease.c misc.c \
	base64.c auth.c dhcp6_ctl.c missing/warnx.c missing/arc4random.c \
	missing/strlcat.c missing/strlcpy.c cfparse.c cftoken.c missing/getifaddrs.c
endif	
LOCAL_OBJS_FILES := $(patsubst %.c,%.o,$(LOCAL_SRC_FILES))

LOCAL_SRV_OBJS_FILES := $(patsubst %.c,%.o,$(LOCAL_SRV_SRC_FILES))

	
LOCAL_MODULE := dhcp6c

LOCAL_SRV_MODULE := dhcp6s

all:	$(LOCAL_MODULE) $(LOCAL_SRV_MODULE)
dhcp6c: 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_MODULE) $(LOCAL_SRC_FILES)
	mv $(LOCAL_MODULE) $(TARGETDIR)
dhcp6s: 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_C_INCLUDES) $(LOCAL_SHARED_LIBS_INCLUDES) $(LOCAL_SHARED_LIBRARIES) \
	-o $(LOCAL_SRV_MODULE) $(LOCAL_SRV_SRC_FILES)
	mv $(LOCAL_SRV_MODULE) $(TARGETDIR)
install:
	mv $(LOCAL_MODULE) $(TARGETDIR)
	mv $(LOCAL_SRV_MODULE) $(TARGETDIR)

.PHONY:clean
clean:
	rm -f *.a *.o $(LOCAL_MODULE) $(LOCAL_SRV_MODULE) $(TARGETDIR)/$(LOCAL_MODULE) $(TARGETDIR)/$(LOCAL_SRV_MODULE)

