#!/bin/bash

chmod -Rf 755 ./*

CC=/opt/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-linux-gnueabi-gcc
export CC

make -f Makefile clean
make -f Makefile static
cp -f ./static $(pwd)/../release/bin/ebtables
make -f Makefile clean
