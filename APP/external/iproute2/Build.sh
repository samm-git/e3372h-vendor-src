#!/bin/bash
chmod -Rf 755 ./*
sed -ig '/CC =/d' Makefile
sed -ig "/^ADDLIB+=ipx_ntop.o ipx_pton.o/a CC = $1" Makefile
sed -ig "/^CC = /a HOSTCC = $1" Makefile

make clean;make;



