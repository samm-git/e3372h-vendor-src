#!/bin/sh
###############################################################
# Copyright (C), 2005-2007, HISILICON Tech. Co., Ltd.
# File name: MBB_HEAD2MK.sh 
# Description: convert mbb_config.h to balong_product_config.mk
# Editer:      2013.07.02
# Version:     v1.0
# Date:
# Warning:
#         1.Not support a define after commont, eg:"/* XXXXXX */ #define A B"
#         2.Not support "#if (defined(A))"
#         3.Not support "&&" and "||", please use "#ifdef" or "#ifndef" instead
#         4.Not support once define in two lines or more.
###############################################################
HEAD_FILE="$1"
MK_HEAD_FILE="$2"

###############################################################
#         Par check
###############################################################
if [ ! -f ${HEAD_FILE} ] || [ ! -f ${HEAD_FILE} ]; then
	echo "#########  HEAD_FILE=$1,  MK_HEAD_FILE=$2 is not exist!!"
	exit 255
fi

temp=`grep -c '[|&]' ${HEAD_FILE}`
if [ ${temp} -ne 0 ]; then
	echo "#########  Not support && and || !!"
	exit 255
fi

temp=`grep -c ^'#if(' ${HEAD_FILE}`
if [ ${temp} -ne 0 ]; then
	echo "#########  Syntax error in line: `grep -n ^'#if(' ${HEAD_FILE}`"
	echo 
	exit 255
fi

temp=`grep -c 'defined' ${HEAD_FILE}`
if [ ${temp} -ne 0 ]; then
	echo "#########  Not support #if (defined(A)) !!"
	exit 255
fi

echo ">> HEAD_FILE=$1,  MK_HEAD_FILE=$2"
echo ">> 终端MBB宏定义将被自动转成makefile，在mbb_config.h中定义的宏可以对makefile生效>>>>"
echo ">> 终端MBB宏定义转换开始，如有错误或遗漏，请检查mbb_config.h >>>>>>>>>>>>>>>>>>>>>>>"
temp=`grep -n "__MBB_CONFIG_H__" ${MK_HEAD_FILE}|sed 's/:/ /g'|awk '{print $1}'|head -1`
if [ ! -z ${temp} ]; then
	head -$((${temp} -1)) ${MK_HEAD_FILE} > ${MK_HEAD_FILE}.bk
	mv ${MK_HEAD_FILE}.bk ${MK_HEAD_FILE}
fi


TEMP_HEAD_FILE_A="MBB_MK_A.tmp"

isBlank="0"
isComment="0"  #Comment like //
isComment2="0" #Comment like /* xxxxx
isComment3="0" #Comment like xxxxx */

numComment=0

if [ -f ${TEMP_HEAD_FILE_A} ]; then
	rm -fr ${TEMP_HEAD_FILE_A}
fi

###############################################################
#         Filter Comment and blank line
###############################################################
while read LINE
do
	# Blank lines check
	isBlank=`echo "${LINE}"|grep -c ^[[:space:]]*$`

	isComment=`echo "${LINE}"|grep -c ^'//'`

	isComment2=`echo "${LINE}"|grep -c ^'/\*'`
	if [ "${isComment2}" -ne 0 ] ; then
		numComment=$((${numComment}+1))
	fi
	
	isComment3=`echo "${LINE}"|grep -c '\*/'`
	
	if [ "${isBlank}" -eq 0 ] && [ "${isComment}" -eq 0 ] && [ "${isComment2}" -eq 0 ] && [ "${numComment}" -eq 0 ]; then
		echo "${LINE}" >> "${TEMP_HEAD_FILE_A}"
	fi
	
	# have /* in before lines ?
	if [ ${numComment} -gt 0 ]; then
		
		if [ "${isComment3}" -ne 0 ] ; then
			numComment=$((${numComment}-1))
		fi
	fi
done < ${HEAD_FILE}

###############################################################
#         Convert head HEAD_FILE to make file
###############################################################
isDefine=0    # like #define xxxx yyyy
isIfdef=0     # like #ifdef xxxx
isIfndef=0    # like #ifndef xxxx
isIfDefined=0 # like #if (defined(xxxx) && defined(yyyy))
isEndif=0     # like #endif
isUsefull=0

numIf=0

definedPar=""
definedVal=""
definedFinal=""

while read LINE
do
	LINE=`echo "${LINE}"|sed 's/\/\/.*/ #/g'`
	LINE=`echo "${LINE}"|sed 's/\/\*.*/ #/g'`
	LINE=`echo "${LINE}"|sed 's/\r//g'`
	
	isDefine=`echo "${LINE}"|grep -c ^'#define'`
	isIfdef=`echo "${LINE}"|grep -c ^'#ifdef'`
	isIfndef=`echo "${LINE}"|grep -c ^'#ifndef'`
	isIfDefined=`echo "${LINE}"|grep -c ^'#if '`
	isEndif=`echo "${LINE}"|grep -c ^'#endif'`

	if [ ${isDefine} -ne 0 ] && [ ${isUsefull} -eq 0 ]; then
		definedPartmp=`echo "${LINE}"|awk '{print $2}'`
		definedValtmp=`echo "${LINE}"|awk '{print $3}'`
		temp=`echo "${definedValtmp}"|grep -c '^#'`
		if [ -z "${definedValtmp}" ] || [ ${temp} -ne 0 ]; then
			definedValtmp="YES"
		fi
		
		definedPar="${definedPar} ${definedPartmp}"
		definedVal="${definedVal} ${definedValtmp}"
		definedFinal="${definedFinal} ${definedPartmp}==${definedValtmp}"
		echo "${definedPartmp}    :=    ${definedValtmp}" >> "${MK_HEAD_FILE}"
		echo "${definedPartmp}    :=    ${definedValtmp}"
	fi
	
	if [ ${isIfdef} -ne 0 ]; then
		numIf=$((${numIf}+1))
		temp1=`echo "${LINE}"|awk '{print $2}'`
		temp2=`echo "${definedPar}"|grep -wc ${temp1}`
		if [ ${temp2} -eq 0 ]; then
			isUsefull=${numIf}
		fi
	fi
	
	if [ ${isIfndef} -ne 0 ]; then
		numIf=$((${numIf}+1))
		temp1=`echo "${LINE}"|awk '{print $2}'`
		temp2=`echo "${definedPar}"|grep -wc ${temp1}`
		if [ ${temp2} -ne 0 ]; then
			isUsefull=${numIf}
		fi
	fi
	
	if [ ${isIfDefined} -ne 0 ]; then
		numIf=$((${numIf}+1))
		temp1=`echo "${LINE}"|sed 's/[()=!]/ /g'|awk '{print $2}'`
		temp2=`echo "${LINE}"|sed 's/[()=!]/ /g'|awk '{print $3}'`
		temp3=`echo "${LINE}"|grep -c "=="`

		temp4="${temp1}==${temp2}"
		temp5="${temp2}==${temp1}"
		temp6=`echo "${definedFinal}"|grep -wc ${temp4}`
		temp7=`echo "${definedFinal}"|grep -wc ${temp5}`
		
		if [ ${temp3} -ne 0 ]; then
			#A==B
			if [ ${temp6} -eq 0 ] && [ ${temp7} -eq 0 ]; then
				isUsefull=${numIf}
			fi
		else
			#A!=B
			if [ ${temp6} -ne 0 ] || [ ${temp7} -ne 0 ]; then
				isUsefull=${numIf}
			fi
		fi
	fi
	
	if [ ${isEndif} -ne 0 ]; then
		if [ ${numIf} -eq ${isUsefull} ]; then
			isUsefull=0
		fi
		numIf=$((${numIf}-1))
	fi
	
done < "${TEMP_HEAD_FILE_A}"

if [ -f ${TEMP_HEAD_FILE_A} ]; then
	rm -fr ${TEMP_HEAD_FILE_A}
fi

echo ">> 请检查以上终端MBB宏定义，如有错误或遗漏，请检查mbb_config.h >>>>>>>>>>>>>>>>>>>>>>>"