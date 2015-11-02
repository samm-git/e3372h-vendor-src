#!/bin/sh

make clean

make

cp -f busybox ../release/bin/

make clean

