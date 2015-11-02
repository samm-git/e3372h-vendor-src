#!/bin/bash
set -e
BALONG_TOPDIR=$1
CFG_OS_ANDROID_PRODUCT_NAME=$2
OBB_ANDROID_DIR=$3

#get CFG_PLATFORM_CHIP_TYPE
echo "wifi BALONG_TOPDIR = ${BALONG_TOPDIR}"
echo "wifi CFG_OS_ANDROID_PRODUCT_NAME = ${CFG_OS_ANDROID_PRODUCT_NAME}"
echo "wifi OBB_ANDROID_DIR = ${OBB_ANDROID_DIR}"

#old wifi build clean
WIFI_OUT_SYSTEM_BIN=${OBB_ANDROID_DIR}/out/target/product/${CFG_OS_ANDROID_PRODUCT_NAME}/system/bin
rm -rf ${WIFI_OUT_SYSTEM_BIN}/wifi\_*

#get WIFI_DIR_NAME
WIFI_DIR_NAME=${MBB_WIFI_CHIP1}

echo "wifi chip1 = ${WIFI_DIR_NAME}"
#编译第一个芯片对应的驱动
if [ "${WIFI_DIR_NAME}" != "FEATURE_OFF" ]; then
    #build wifi
    cd ${OBB_ANDROID_DIR}/kernel/drivers/wifi/${WIFI_DIR_NAME}
	chmod 777 build_wifi_v7.sh
    ./build_wifi_v7.sh
    if [ ! "$?" = "0" ]; then
        echo "Error11: build wifi driver wifi ${WIFI_DIR_NAME} failed"
        exit 255
    fi
fi

#get WIFI_DIR_NAME
WIFI_DIR_NAME=${MBB_WIFI_CHIP2}
echo "wifi chip2 = ${WIFI_DIR_NAME}"
#编译第二个芯片对应的驱动
if [ "${WIFI_DIR_NAME}" != "FEATURE_OFF" ]; then
    #build wifi
    cd ${OBB_ANDROID_DIR}/kernel/drivers/wifi/${WIFI_DIR_NAME}
    chmod 777 build_wifi_v7.sh
    ./build_wifi_v7.sh
    if [ ! "$?" = "0" ]; then
        echo "Error22: build wifi driver wifi ${WIFI_DIR_NAME} failed"
        exit 255
    fi
fi
