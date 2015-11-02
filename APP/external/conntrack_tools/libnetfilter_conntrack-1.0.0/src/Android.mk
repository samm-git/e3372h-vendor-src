#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)

version = 1.0

INC_APP_DIR = ./
L_CFLAGS := -g -O2 -Wall -DPACKAGE_NAME=\"libnetfilter_conntrack\" -DPACKAGE_TARNAME=\"libnetfilter_conntrack\" -DPACKAGE_VERSION=\"1.0.0\" -DPACKAGE_STRING=\"libnetfilter_conntrack\ 1.0.0\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"libnetfilter_conntrack\" -DVERSION=\"1.0.0\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -DHAVE_ARPA_INET_H=1 -DHAVE_INET_NTOP=1 -DHAVE_INET_NTOP_IPV6=1
			
L_CFLAGS += -Wl,--allow-shlib-undefined
#############################################################
# Build radvd 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES :=main.c callback.c \
				conntrack/api.c \
			    conntrack/getter.c conntrack/setter.c \
			    conntrack/parse.c conntrack/build.c \
			    conntrack/snprintf.c \
			    conntrack/snprintf_default.c conntrack/snprintf_xml.c \
			    conntrack/objopt.c \
			    conntrack/compare.c \
			    conntrack/copy.c \
			    conntrack/filter.c conntrack/bsf.c \
			    conntrack/grp.c conntrack/grp_getter.c conntrack/grp_setter.c \
			    conntrack/stack.c expect/api.c \
				expect/compare.c \
				expect/getter.c expect/setter.c \
				expect/parse.c expect/build.c \
				expect/snprintf.c \
				expect/snprintf_default.c

LOCAL_LDLIBS += -lc -lgplutil

LOCAL_MODULE:= libnetfilter_conntrack

LOCAL_MODULE_TAGS := eng

include $(BUILD_STATIC_LIBRARY)

