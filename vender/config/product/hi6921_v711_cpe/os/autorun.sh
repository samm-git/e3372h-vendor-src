#!/system/bin/busybox sh

mkdir bin
ln -s /system/bin/sh /bin/sh

/system/sbin/NwInquire &

busybox echo 0 > /proc/sys/net/netfilter/nf_conntrack_checksum
#BEGIN PN:DTS2013090908350 c00190522 add 20130910
/system/bin/upgsrvd &
#begin DTS2013110106009 zhoujianchun 2013.11.01 added
ecall bsp_get_factory_mode
dmesg | /system/bin/busybox grep "+=+=+==factory_mode+=+=+=="
if [ $? -eq 0 ]
then
    insmod /system/bin/si32178.ko
else
    if [ -e /system/bin/appautorun.sh ]
    then
        /system/bin/appautorun.sh
    fi
fi
#end DTS2013110106009 zhoujianchun 2013.11.01 added

#根据产线NV项，如果是产线版本，则只起wifi，否则起全应用，forgive me pls, no better method thought
#ecall bsp_get_factory_mode
#BEGIN DTS2013092201594 yaozhanwei 2013-05-25 modified for wifi factory mode
dmesg | /system/bin/busybox grep "+=+=+==factory_mode+=+=+=="
#END DTS2013092201594 yaozhanwei 2013-05-25 modified for wifi factory mode
#if [ $? -eq 0 ]
#then 
#	#BEGIN DTS2013092201594 yaozhanwei 2013-05-25 added for wifi factory mode
#	/system/bin/wifi_brcm/exe/wifi_poweron_factory_43241.sh
#	#END DTS2013092201594 yaozhanwei 2013-05-25 added for wifi factory mode
#else
#	/app/appautorun.sh
#fi
#END PN:DTS2013090908350 c00190522 add 20130910
