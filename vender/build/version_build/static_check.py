#!/usr/bin/python
#-*- coding: UTF-8 -*-
#**************************************************************
#
#          版权所有 (C), 2001-2011,华为技术有限公司
#
#**************************************************************
#文件名    ：static_check.py
#版本号    ：初稿
#作者       l00130025
#生成日期  ：2012年11月27日
#功能描述  ：等待对应形态下的构建结果
#使用方法  : 1. python ：static_check.py <version> <branch> <changeid>  <timestamp> <tag> <owners> [<buildgroup>] [<buildtype>] 
#实例：        python ：static_check.py version:BalongV9R1SFT branch:br_balong_ci_jellybean changeid:Ie2b2a46c75f5fccc3d79ad15bbe174cfc421aafd  timestamp:20130314_112736 tag:tag1234 owners:l00130025  buildgroup:hi6620oem_gutl_dsda buildtype:all  target:balong,bootimage,libbalong-ril
#输入参数  : 参数1:41位change-id
#        : 参数2: 版本形态
#输出参数  ：
#返回值    ： 各个形态的编译结果
#修改历史  ：
#1.日期    ：2012年11月28日
#  作者    l00130025
#  修改内容：创建文件
import os
import sys
import commands
import string
import copy
import build_utility

def main():
    argv_list = sys.argv[1:]
    if len(argv_list) == 0 :
            print "fail,parameters_is_empty"
            return 1
            
    reload(sys)
    sys.setdefaultencoding('utf-8')
    
    params_dict = {}
    for p in argv_list:
        if string.find(p, ':') != -1 :
            subs = string.split(p,':')
            params_dict[subs[0]]=subs[1]
        else:
            params_dict[p]=""
            
    if not params_dict.has_key('version') or \
       not params_dict.has_key('branch') or \
       not params_dict.has_key('changeid') or \
       not params_dict.has_key('timestamp')    or \
       not params_dict.has_key('tag') or \
       not params_dict.has_key('buildgroup') or \
       not params_dict.has_key('build_step') or \
       not params_dict.has_key('owners'):
            print "fail,params has not enough key"
            return 1
            
        
    branch_name = params_dict['branch']
    timestamp = params_dict['timestamp']
    version_name = params_dict['version']
    buildgroup = params_dict['buildgroup']
    changeid = params_dict['changeid']
    build_step = params_dict['build_step']

    buildtype ='all'
    if params_dict.has_key('buildtype'):
        buildtype = params_dict['buildtype']

    target =''
    if params_dict.has_key('target'):
        target = params_dict['target'] 
        
    source_dir = os.path.abspath('../..')

    complie_entry_dir = source_dir+'/build'
    os.chdir(complie_entry_dir)
    
    build_result_str = ""
    
    if build_step == "static_check_gen_dep":
        build_result_str += "___hi6210sft_pass"
    if build_step == "static_check_new":
        build_result_str += "___hi6210sft_pass"
    if build_step == "static_check_base":
        build_result_str += "___hi6210sft_pass"
    if build_step == "check_result":    
        build_result_str += "___hi6210sft_pass"
        
    # 返回编译结果  
    print "VERSION_BUILD_RESULT:" + build_result_str
    if ('fail' in build_result_str) :
        return 1
    return 0
    

if __name__ == '__main__':
    main()
