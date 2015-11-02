#!/bin/bash
export tmp_conntrack_path=/tmp/ct/$RANDOM
filepath=$(pwd)
chmod -R 777 ./*
mkdir -p $tmp_conntrack_path
chmod -R 777 /tmp/ct
chmod -R 777 $tmp_conntrack_path
cd ./libnfnetlink-1.0.0/
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc   AR=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ar  RANLIB=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ranlib STRIP=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip --enable-static --disable-shared 
make clean
make 
make install
cd -
cd ./libnetfilter_conntrack-1.0.0/
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc   AR=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ar  RANLIB=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ranlib STRIP=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip --enable-static --disable-shared PKG_CONFIG_PATH=$tmp_conntrack_path/lib/pkgconfig 
make clean
make
make install
cd -
mkdir -p ./../../../../../../../../../sysroots/9615-cdp/$tmp_conntrack_path/lib
cp -rf ./../../opensrc/conntrack_tools/libnetfilter_conntrack-1.0.0/src/.libs/* ./../../../../../../../../../sysroots/9615-cdp/$tmp_conntrack_path/lib
cp -rf ./../../opensrc/conntrack_tools/libnfnetlink-1.0.0/src/.libs/* ./../../../../../../../../../sysroots/9615-cdp/$tmp_conntrack_path/lib
cd ./conntrack-tools-1.0.0
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc   AR=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ar  RANLIB=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-ranlib STRIP=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip --enable-static --disable-shared PKG_CONFIG_PATH=$tmp_conntrack_path/lib/pkgconfig
cd ./src
cd ../
make clean
make 
make install
cd ..
/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-strip $tmp_conntrack_path/sbin/conntrack

cp $tmp_conntrack_path/sbin/conntrack ./ -f
rm -rf $tmp_conntrack_path
rm -rf $filepath/../../../../../../../../../sysroots/9615-cdp/tmp