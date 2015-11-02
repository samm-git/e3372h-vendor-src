#
#build dhcp executable bin file
#
LOCAL_PATH:= $(call my-dir)

version = 1.0

INC_APP_DIR = ./
L_CFLAGS := -g -O2 -std=gnu99 -W -Wall \
	-Wmissing-prototypes -Wwrite-strings -Wcast-qual -Wfloat-equal -Wshadow -Wpointer-arith -Wbad-function-cast -Wsign-compare -Waggregate-return -Wmissing-declarations -Wredundant-decls -Wnested-externs -Winline -Wstrict-prototypes -Wundef \
	-Wno-unused-parameter -DPACKAGE_NAME=\"conntrack-tools\" -DPACKAGE_TARNAME=\"conntrack-tools\" -DPACKAGE_VERSION=\"1.0.0\" -DPACKAGE_STRING=\"conntrack-tools\ 1.0.0\" -DPACKAGE_BUGREPORT=\"pablo@netfilter.org\" -DPACKAGE_URL=\"\" -DPACKAGE=\"conntrack-tools\" -DVERSION=\"1.0.0\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DLT_OBJDIR=\".libs/\" -DYYTEXT_POINTER=1 -DHAVE_LINUX_CAPABILITY_H=1 -DHAVE_ARPA_INET_H=1 -DHAVE_INET_PTON=1 -DHAVE_INET_PTON_IPV6=1
			
#############################################################
# Build radvd 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES :=	alarm.c main.c run.c hash.c queue.c rbtree.c \
		    local.c log.c mcast.c udp.c netlink.c vector.c \
		    filter.c fds.c event.c process.c origin.c date.c \
		    cache.c cache_iterators.c \
		    cache_timer.c \
		    sync-mode.c sync-alarm.c sync-ftfw.c sync-notrack.c \
		    traffic_stats.c stats-mode.c \
		    network.c cidr.c \
		    build.c parse.c \
		    channel.c multichannel.c channel_mcast.c channel_udp.c \
		    tcp.c channel_tcp.c \
		    external_cache.c external_inject.c \
		    internal_cache.c internal_bypass.c \
		    read_config_yy.c read_config_lex.c \
			../extensions/libct_proto_tcp.c \
			../extensions/libct_proto_udp.c \
			../extensions/libct_proto_udplite.c ../extensions/libct_proto_icmp.c ../extensions/libct_proto_icmpv6.c \
			../extensions/libct_proto_sctp.c ../extensions/libct_proto_dccp.c ../extensions/libct_proto_gre.c \
			../extensions/libct_proto_unknown.c
				

LOCAL_LDLIBS += -lc -lgplutil 
LOCAL_STATIC_LIBRARIES=libnetfilter_conntrack libnfnetlink
LOCAL_MODULE:= conntrackd

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

#############################################################
# Build radvd 
#
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_SRC_FILES :=	conntrack.c \
			../extensions/libct_proto_tcp.c \
			../extensions/libct_proto_udp.c \
			../extensions/libct_proto_udplite.c ../extensions/libct_proto_icmp.c ../extensions/libct_proto_icmpv6.c \
			../extensions/libct_proto_sctp.c ../extensions/libct_proto_dccp.c ../extensions/libct_proto_gre.c \
			../extensions/libct_proto_unknown.c
				

LOCAL_LDLIBS += -lc -lgplutil 
LOCAL_STATIC_LIBRARIES=libnetfilter_conntrack libnfnetlink
LOCAL_MODULE:= conntrack

LOCAL_MODULE_TAGS := eng

include $(BUILD_EXECUTABLE)

