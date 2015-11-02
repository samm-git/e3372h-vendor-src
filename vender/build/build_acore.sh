#!/bin/sh
while true
do
echo "####################################################"
echo "###  Can I help you?                             ###"
echo "###  1: Clean                                    ###"
echo "###----------------------------------------------###"
echo "###  a: Build                                    ###"
echo "###----------------------------------------------###"
echo "###  q: exit                                     ###"
echo "####################################################"
echo "Please input your choose:"
    read LINE
    case $LINE in
		1)
		    echo "Now going to Clean, ready go!!!"
			./obuild.py product=hi6921_v711_e5   distclean
			if [ $? -ne 0 ]; then echo "#########   compile ERROR    #########"; exit 255; fi
		;;
		a)
		    echo "Now going to build, ready go!!!"
                        ./obuild.py product=hi6921_v711_e5 publish=true core=acore
			if [ $? -ne 0 ]; then echo "#########   compile ERROR    #########"; exit 255; fi
		;;
		q)
			echo "Thank You, Bye Bye!"
			exit 0
		;;
    esac
done
