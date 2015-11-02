#! /bin/sh
# $1: webui source code path
# $2: out path 
# $3: produce type: e5 ,hilink, cpe
# webui_install_bak
#if [ $# -ne 3 ]; then kkx 
#    echo "Error: wrong number of arguments in cmd: $0 $* "
#    exit 1
#fi
echo "[info] install webui arguments is: $@ "

CURPATH=$PWD
if [ "$1" = "" ];then
    SRC_PATH=$TOP/vendor/huawei-platform/webui
else
    SRC_PATH=$1
fi
echo "[info] SRC_PATH = $SRC_PATH"

if [ "$2" = "" ];then
    OUT_PATH=$CURPATH
else
    OUT_PATH=$2
fi
echo "[info] OUT_PATH = $OUT_PATH"

if [ "$3" = "" ];then
    PRODUCT_TYPE="e5"
else
    PRODUCT_TYPE=$3
fi
echo "[info] PRODUCT_TYPE = $PRODUCT_TYPE "

if [ "$4" = "gzip" ];then
    COMPRESS_TYPE="gzip"
else
    COMPRESS_TYPE="null"
fi
echo "[info] COMPRESS_TYPE = $COMPRESS_TYPE "


#WEBUI_CONFIG="http://10.82.19.157:6801/svn/TC_ServiceAndSolutions_CODE_SVN/005.UnifiedRoutingApp/bugfix/trunk/webui_trunk_R004/"

#****************************************************************************************************************
rm -fr $OUT_PATH/obj/webui_temp $OUT_PATH/webui
OUT_PATH_COMMON=$OUT_PATH/obj/webui_temp/WebApp/common
USERMUAL_PATH=$SRC_PATH"/WebApp/common/usermanual_e5"
mkdir -p $OUT_PATH_COMMON/config

cp -rf $SRC_PATH/WebApp/common/config/*  $OUT_PATH_COMMON/config/
cp -rf $SRC_PATH/WebApp/common/version/version.xml $OUT_PATH_COMMON/config/
cp -rf $SRC_PATH/WebApp/common/js  $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/html  $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/css  $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/language  $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/lib  $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/res  $OUT_PATH_COMMON/

cp -rf $SRC_PATH/WebApp/common/factory $OUT_PATH_COMMON/
cp -rf $SRC_PATH/WebApp/common/favicon.ico $OUT_PATH_COMMON/


if [ "$PRODUCT_TYPE" = "e5" -o "$PRODUCT_TYPE" = "wingle" ];then
    echo "[info] start install e5 webui..."
    USERMUAL_PATH=$SRC_PATH"/WebApp/common/usermanual_e5"
    cp -rf $SRC_PATH/WebApp/common/small $OUT_PATH_COMMON/
    cp -rf $SRC_PATH/upnp $OUT_PATH/obj/webui_temp
    
    rm -fr $OUT_PATH_COMMON/config/global_hilink
    rm -fr $OUT_PATH_COMMON/config/lan_hilink
    rm -fr $OUT_PATH_COMMON/config/deviceinformation_multimode
    rm -fr $OUT_PATH_COMMON/config/network_multimode
    rm -fr $OUT_PATH_COMMON/config/network_vxworks
    rm -fr $OUT_PATH_COMMON/config/oled_vxworks
    rm -fr $OUT_PATH_COMMON/config/wifi_vxworks
    rm -fr $OUT_PATH_COMMON/config/webserver_vxworks    
    
elif  [ "$PRODUCT_TYPE" = "cpe" ];then
    echo "[info] start install cpe webui..."
    USERMUAL_PATH=$SRC_PATH"/WebApp/common/usermanual_cpe"
    cp -rf $SRC_PATH/WebApp/common/small $OUT_PATH_COMMON/
    cp -rf $SRC_PATH/upnp $OUT_PATH/obj/webui_temp

    rm -fr $OUT_PATH_COMMON/config/global_hilink
    rm -fr $OUT_PATH_COMMON/config/lan_hilink
    rm -fr $OUT_PATH_COMMON/config/deviceinformation_multimode
    rm -fr $OUT_PATH_COMMON/config/network_multimode
    rm -fr $OUT_PATH_COMMON/config/network_vxworks
    rm -fr $OUT_PATH_COMMON/config/oled_vxworks
    rm -fr $OUT_PATH_COMMON/config/wifi_vxworks
    rm -fr $OUT_PATH_COMMON/config/webserver_vxworks    
elif  [ "$PRODUCT_TYPE" = "hilink" ];then
    echo "[info] start install hilink webui..."
    USERMUAL_PATH=$SRC_PATH"/WebApp/common/usermanual_HiLink"
    rm -fr $OUT_PATH_COMMON/config/led
    rm -fr $OUT_PATH_COMMON/config/fastboot
    rm -fr $OUT_PATH_COMMON/config/firewall
    rm -fr $OUT_PATH_COMMON/config/dlna
    rm -fr $OUT_PATH_COMMON/config/help
    rm -fr $OUT_PATH_COMMON/config/offload
    rm -fr $OUT_PATH_COMMON/config/oled
    rm -fr $OUT_PATH_COMMON/config/oled_128x64
    rm -fr $OUT_PATH_COMMON/config/oled_vxworks
    rm -fr $OUT_PATH_COMMON/config/websd
    rm -fr $OUT_PATH_COMMON/config/wifi
    rm -fr $OUT_PATH_COMMON/config/wifi_vxworks
    rm -fr $OUT_PATH_COMMON/config/wifistation
    rm -fr $OUT_PATH_COMMON/config/global
    rm -fr $OUT_PATH_COMMON/config/lan
    rm -fr $OUT_PATH_COMMON/config/deviceinformation_multimode
    rm -fr $OUT_PATH_COMMON/config/network_multimode
    rm -fr $OUT_PATH_COMMON/config/network_vxworks
    rm -fr $OUT_PATH_COMMON/config/webserver_vxworks    
    cp -rf $OUT_PATH_COMMON/config/global_hilink $OUT_PATH_COMMON/config/global
    cp -rf $OUT_PATH_COMMON/config/lan_hilink $OUT_PATH_COMMON/config/lan
    rm -fr $OUT_PATH_COMMON/config/global_hilink
    rm -fr $OUT_PATH_COMMON/config/lan_hilink    
else
    echo "[info] install webui product is wrong..."
    exit -1    
fi

mkdir $OUT_PATH_COMMON/usermanual
cp -rf $USERMUAL_PATH/public_sys-resources $OUT_PATH_COMMON/usermanual/public_sys-resources
cp -rf $USERMUAL_PATH/en-us $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/zh-cn $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/fr-fr $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/de-de $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/es-es $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/it-it $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/ru-ru $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/pt-pt $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/pt-br $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/es-ar $OUT_PATH_COMMON/usermanual/
cp -rf $USERMUAL_PATH/ar-sa $OUT_PATH_COMMON/usermanual/
find  $OUT_PATH/obj/webui_temp -name "*.svn" | xargs rm -rf

if [ "$COMPRESS_TYPE" = "gzip" ]; then
    echo "[info] start compress webui js, html, css file ..."
    find  $OUT_PATH/obj/webui_temp -name "*.js" | xargs gzip -f 
    find  $OUT_PATH/obj/webui_temp -name "*.html" | xargs gzip -f 
    find  $OUT_PATH/obj/webui_temp -name "*.css" | xargs gzip -f 
fi

cp -rf $OUT_PATH/obj/webui_temp $OUT_PATH/webui

echo "[info] install webui done ..."





