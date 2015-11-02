#!/bin/bash
cd system
echo "Step1: delete some files in system/bin directory"
rm -rf bin/perf
rm -rf bin/lsz
rm -rf bin/wifi_brcm
rm -rf bin/lcdtest
rm -rf bin/wpa_supplicant
rm -rf bin/device
rm -rf bin/router
rm -rf bin/npdaemon
rm -rf bin/dhcp6c
rm -rf bin/webserver
rm -rf bin/agent
rm -rf bin/dhcp6s
rm -rf bin/stacall
rm -rf bin/siproxd
rm -rf bin/miniupnpd
rm -rf bin/radvd
rm -rf bin/dns
rm -rf bin/call
rm -rf bin/dhcp6relay
rm -rf bin/ats
rm -rf bin/dhcps
rm -rf bin/wifiwpa_cli
rm -rf bin/wpa_cli
rm -rf bin/syswatch
rm -rf bin/dhcpc
rm -rf bin/npd6proxy
rm -rf bin/brctl
rm -rf bin/debuggerrd
rm -rf bin/sntp
rm -rf bin/radvdump
rm -rf bin/notify_server
rm -rf bin/dhcp6ctl
rm -rf bin/msg_monitor
rm -rf bin/ipcheck
rm -rf bin/lcd_screenshot

echo "Step2: delete some files in system/lib directory"
rm -rf lib/libwl.so
rm -rf lib/libwebsdk.so
rm -rf lib/libwlwpscli.so
rm -rf lib/libhttpstack.so
rm -rf lib/libnetplatform.so
rm -rf lib/libqrencode.so
rm -rf lib/libcl_xml.so
rm -rf lib/libpublicapi.so
rm -rf lib/libplatform.so
rm -rf lib/libwebsdk_postevent.so
rm -rf lib/libipc.so
rm -rf lib/libwebsdk_syswatchapi.so
rm -rf lib/libwifiwpa_client.so
rm -rf lib/libwpa_client.so
rm -rf lib/libwebsdk_upnp.so
rm -rf lib/libatproxy.so
rm -rf lib/libpktwpscli.so

echo "Step3: delete some files in system/app directory"
rm -rf app

echo "setp4: delete some files in system/media directory"
rm -rf  media




