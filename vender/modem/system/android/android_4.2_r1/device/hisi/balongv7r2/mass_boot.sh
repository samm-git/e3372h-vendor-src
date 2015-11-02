#!/system/bin/busybox sh
cdrom=$(cat /proc/mtd|grep cdromiso|cut -d: -f1|sed 's/mtd/mtdblock/')
sdpath=/dev/block/platform/hi_mci.1
gadget=/sys/devices/platform/dwc3/gadget
mount -t yaffs2 /dev/block/$cdrom /root/


for i in  $(ls $gadget|grep lun)
do 
	if [ "$(cat $gadget/$i/ro)" = "1" ];then
        cdromlun=$gadget/$i/file
	echo /root/ISO > $cdromlun
    elif 
        [ "$(cat $gadget/$i/mode)" = "1" ];then
		sdlun=$gadget/$i/file
	if [ $(ls $sdpath|grep mmcblk[0-9]) ];then
		echo $sdpath/$(ls $sdpath|grep mmcblk[0-9]$) > $sdlun
	elif 
	[ $(ls $sdpath|grep mmcblk[0-9]p1$) ];then
		echo $sdpath/$(ls $sdpath|grep mmcblk[0-9]p1$) > $sdlun 
	fi
    fi

done
