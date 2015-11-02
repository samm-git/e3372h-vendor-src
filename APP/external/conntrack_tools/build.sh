#!/bin/bash
export tmp_conntrack_path=/tmp/ct/$RANDOM
rm -rf $tmp_conntrack_path
chmod -R 777 ./*
mkdir -p $tmp_conntrack_path
chmod -R 777 /tmp/ct
chmod -R 777 $tmp_conntrack_path
cd ./libnfnetlink-1.0.0/
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-gcc   AR=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ar  RANLIB=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ranlib STRIP=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-strip --enable-static --disable-shared 
make clean
make 
make install
cd -
cd ./libnetfilter_conntrack-1.0.0/
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-gcc   AR=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ar  RANLIB=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ranlib STRIP=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-strip --enable-static --disable-shared PKG_CONFIG_PATH=$tmp_conntrack_path/lib/pkgconfig 
make clean
make
make install
cd -
cd ./conntrack-tools-1.0.0
./configure --prefix=$tmp_conntrack_path --host=arm-linux  CC=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-gcc   AR=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ar  RANLIB=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-ranlib STRIP=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-strip --enable-static --disable-shared PKG_CONFIG_PATH=$tmp_conntrack_path/lib/pkgconfig
cd ./src
cd ../
make clean
make 
make install
cd ..
/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin/arm-uclibc-linux-2.6.36-strip $tmp_conntrack_path/sbin/conntrack
cp $tmp_conntrack_path/sbin/conntrack ./ -f
rm -rf $tmp_conntrack_path
