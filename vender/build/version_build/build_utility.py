#!/usr/bin/python
#-*- coding: UTF-8 -*-
#**************************************************************
#
#          版权所有 (C), 2001-2011,华为技术有限公司
#
#**************************************************************
#文件名    ：build_utlity.py
#版本号    ：初稿
#作者       l00130025
#生成日期  ：2012年11月27日
#功能描述  ：等待对应形态下的构建结果
#使用方法  : 1. python version_compile.py <version> <branch> <changeid>  <timestamp> <tag> <owners> [<buildgroup>] [<buildtype>] 
#实例：        python version_compile.py version:BalongV9R1SFT branch:br_balong_ci_jellybean changeid:Ie2b2a46c75f5fccc3d79ad15bbe174cfc421aafd  timestamp:20130314_112736 tag:tag1234 owners:l00130025  buildgroup:hi6620oem_gutl_dsda buildtype:all  target:balong,bootimage,libbalong-ril
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
import multiprocessing
import platform
sys.path.append("../scripts")
import comm
import nlw_image_save
import xml.etree.ElementTree as ET
import platform  
import json
import mv_code_dir

#XML解析可调整的库
from xml.dom.minidom import parseString
from collections import namedtuple

THIS_FILE_NAME = __file__

BUILD_RULE=namedtuple('BUILD_RULE','productname added_condition buildtype addname target buildscript')
BUILD_SCRIPT=namedtuple('BUILD_SCRIPT','productname script')

def get_group_product_type(build_rule_file,version_name,group):
    """
    功能描述：获取version的构建策略
    参数：
    返回值： product_type
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
   
    #读取XML文件内容
    f = open(build_rule_file,'r')
    data = f.read()
    f.close()
    
    versionUpperString = version_name.upper()
    
    #获取Document类，根据XML结构获取Element和Attribute
    dom = parseString(data)
    versionNode     = dom.getElementsByTagName(versionUpperString)
    buildGrouplist  = versionNode[0].getElementsByTagName("buildgroup")
    #获取编译分组
    # 检查需要编译的产品形态列表
        
    product_type='mbb'
    for element in buildGrouplist:      
        # 需要编译的产品形态列表
        groupname  = element.getAttribute("name")
        producttype_val = element.getAttribute("producttype")        
        if groupname == group and \
            producttype_val !='':
            product_type = producttype_val
            break            
    
    # 检查需要编译的产品形态列表
    return product_type
    
def is_group_release_build(build_rule_file,version_name,group):
    """
    功能描述：判断对应 Group是否 按照Release方式构建
    参数：
    返回值： true/false
    异常描述：
    修改记录：1.日期    : 2013年6月28日
                作者    : l00130025
                修改内容: 创建函数
    """
   
    #读取XML文件内容
    f = open(build_rule_file,'r')
    data = f.read()
    f.close()
    
    versionUpperString = version_name.upper()
    #获取Document类，根据XML结构获取Element和Attribute
    dom = parseString(data)
    versionNode     = dom.getElementsByTagName(versionUpperString)
    buildGrouplist  = versionNode[0].getElementsByTagName("buildgroup")
    #获取编译分组
    # 检查需要编译的产品形态列表
        
    be_release_build='false'
    for element in buildGrouplist:      
        # 需要编译的产品形态列表
        groupname  = element.getAttribute("name")
        buildvalue = element.getAttribute("relbuild")        
        if groupname == group and \
            buildvalue !='':
            be_release_build = buildvalue
            break            

    return be_release_build
    
def get_rules_of_version(build_rule_file,version_name):
    """
    功能描述：获取不需要编译的产品形态
    参数：
    返回值：dict_rule
    异常描述：
    修改记录：1.日期    : 2012年11月27日
                作者    : l00130025
                修改内容: 创建函数
    """
   
    #读取XML文件内容
    f = open(build_rule_file,'r')
    data = f.read()
    f.close()
    
    versionUpperString = version_name.upper()
    #获取Document类，根据XML结构获取Element和Attribute
    dom = parseString(data)
    versionNode     = dom.getElementsByTagName(versionUpperString)
    buildGrouplist  = versionNode[0].getElementsByTagName("buildgroup")
    #获取编译分组
    xmlbuildgroupdic= {}
    # 检查需要编译的产品形态列表

    for element in buildGrouplist:      
        # 需要编译的产品形态列表
        groupname = element.getAttribute("name")
        rules = element.getElementsByTagName("rule")
        rulelist =[]
        for rule in rules:
            productname     =rule.getAttribute('productname')
            added_condition =rule.getAttribute('added_condition')
            addname         =rule.getAttribute('addname')
            
            build_type      =rule.getAttribute('buildtype')
            if not "release"==build_type:
                build_type="notrelease"
            
            build_target=rule.getAttribute('target')

            buildscript=rule.getAttribute('buildscript')
            if ''==buildscript:
                buildscript="./HiBuild.sh"
                #buildscript="python obuild.py"                     
        
            build_rule=BUILD_RULE(productname,added_condition,build_type,addname,build_target,buildscript)
            rulelist.append(build_rule)
            #print build_rule

        xmlbuildgroupdic[groupname]=rulelist

    return xmlbuildgroupdic
    
def CleanOldBuildFile(source_dir):
    android_out_dir    = source_dir+'/../../out'
    build_delivery_dir = source_dir+'/build/delivery'    
    
    if os.path.exists(android_out_dir) :
        errcode =os.system("rm -rf "+android_out_dir)        
        print "rm -rf "+android_out_dir
        
    if os.path.exists(build_delivery_dir) :
        errcode =os.system("rm -rf "+build_delivery_dir)        
        print "rm -rf "+build_delivery_dir
    return
    
def gen_build_scripts(source_dir,version,branch_name,buildgroup,buildtype,target,params_dict, cleanFlg=True):
    """
    功能描述：获取要执行的编译脚本
    参数：source_dir：顶层源代码路径
          branch_name：分支名
    返回值：需要执行的编译脚本列表
    异常描述：
    修改记录：1.日期    : 2012年11月1日
                作者    : l00130025
                修改内容: 创建函数
    """
    buildscriptsdic={}
    
    product_dir = source_dir + "/config/product"
    if not os.path.exists(product_dir) :
        print "process ,config/product not exist"        
        return buildscriptsdic   

    

    spec_products = os.listdir(product_dir)            
    clean_action = " clean "
    
    specbuildtype = buildtype.lower()
    
    if not specbuildtype=="release" and \
       not specbuildtype=="notrelease" :
            specbuildtype="all"

    orig_buildtarget=""        
    spectargets=target.strip()    
    if len(spectargets) != 0 :
        orig_buildtarget = spectargets.replace(',',' ' )
        
    spec_addname =''
    if params_dict.has_key('addname'):
        spec_addname = params_dict['addname']
        
    build_rule_file = source_dir + "/build/version_build/build_rule.xml"
    xmlbuildgroupdic = get_rules_of_version(build_rule_file,version)    

    if not xmlbuildgroupdic.has_key(buildgroup):
        return buildscriptsdic        
    
    #  xmlbuildgroupdic:       productnames in one group are different
    #     groupname BUILD_RULE (productname1 added_condition target buildtype)
    #               BUILD_RULE (productname2 added_condition target buildtype)
    # ========>    
    #     productname1   BUILD_SCRIPT(productname1 script_clean)
    #                    BUILD_SCRIPT(productname1 script_retrycomponent)
    #                    BUILD_SCRIPT(productname1 script_balong)    
    #     productname2   BUILD_SCRIPT(productname2 script_clean)
    #                    BUILD_SCRIPT(productname2 script_retrycomponent)
    #                    BUILD_SCRIPT(productname2 script_balong)
        
    rulelist=xmlbuildgroupdic[buildgroup]
        
    for i in range(len(rulelist)):
        if rulelist[i].productname not in spec_products:
            continue    
            
        if not specbuildtype=="all" and \
           not specbuildtype==rulelist[i].buildtype:
            continue    

        product_build_script = []        
        productname          = rulelist[i].productname
        added_condition      = rulelist[i].added_condition
        buildtype            = rulelist[i].buildtype
        addname              = rulelist[i].addname
        xmltarget            = rulelist[i].target
        buildscript          = rulelist[i].buildscript

        buildtarget=orig_buildtarget
        
        if orig_buildtarget=="":
            buildtarget=xmltarget

        clean_script=buildscript+ " product=" + productname   + clean_action
        balong_build=buildscript+ " product=" + productname  + " " + buildtarget 
        
        clean_script += added_condition
        balong_build += added_condition        
        # pass buildinfo to HiBuild.sh
        buildinfo = comm.GenerateCmdArgFromDict(params_dict)
        balong_build += " "+ buildinfo
        
        if buildtype=="release" or \
            buildtarget=="balong-libs":
            balong_build += " relversion=true"
            
        if buildtarget=="camera" or \
            buildtarget=="balong-libs" or \
            buildtarget=="lw_rom_modules" :
            addname += "_"+buildtarget

        if spec_addname != "": 
            addname += "_"+spec_addname

        if addname != "":            
            balong_build +=" addname="+addname+" "        
            
        if cleanFlg :
            build_script=BUILD_SCRIPT(productname,clean_script)
            product_build_script.append(build_script)
        
        build_script=BUILD_SCRIPT(productname,balong_build)    
        product_build_script.append(build_script)

        if  len(product_build_script) != 0:
            buildscriptsdic[productname]=product_build_script
        
    return buildscriptsdic

def build_spec_product(product,buildrules):
    """
    功能描述：顺序执行对应产品形态的编译规则
    参数：productRules：编译规则
    返回值：需要执行的编译结果
    异常描述：
    修改记录：1.日期    : 2013年4月20日
                作者    : l00130025
                修改内容: 创建函数
    """
    productresult =0
    result_str =""
    # buildrules
    #     productname1   BUILD_SCRIPT(productname1 script_clean)
    #                    BUILD_SCRIPT(productname1 script_retrycomponent)
    #                    BUILD_SCRIPT(productname1 script_balong)    
    # productRules 目前会先做clean ,再做编译;编译结果为每一次的编译结果
    
    #logf = open("log.txt", 'a')
    for rule in buildrules :
        #print rule        
        script = rule.script    
        
        print script
        # print "platform",platform.system()
        if "Linux" == platform.system():
            ret = commands.getstatusoutput(script)
            productresult+=ret[0]
        else:
            ret = os.system(script)
            productresult = ret
            
        print "%s's Compile Result is %d" %(product,productresult)

    if productresult:
        result_str+=product+"_fail"
    else:
        result_str+=product+"_pass"
    
    return result_str

def exec_sequ_build(buildscriptdic):
    """
    功能描述：执行所有组的编译
    参数：buildscriptdic：编译规则字典
    返回值： 执行的编译结果
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
    build_result_str =""
    #print buildscriptdic
    for product in buildscriptdic:
        # 发起对应产品形态的构建信息文件
        build_result_str+='___'        
        build_result_str += build_spec_product(product,buildscriptdic[product])    
    
    #logf.close()        
    return build_result_str    
def exec_group_build(buildscriptdic):
    """
    功能描述：执行所有组的编译
    参数：buildscriptdic：编译规则字典
    返回值： 执行的编译结果
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
    build_result_str =""

    buildpool = multiprocessing.Pool()     # Make a process pool,use the max cpu number        
    exresult=[]    
    
    #print buildscriptdic
    for product in buildscriptdic:
        # 发起对应产品形态的构建信息文件        
        exresult.append(buildpool.apply_async(build_spec_product, (product,buildscriptdic[product])))        
    
    buildpool.close()
    buildpool.join()
        
    for rslt in exresult:
        build_result_str+='___'
        build_result_str+=rslt.get()    
    
    #logf.close()        
    return build_result_str

def rm_confidential_code(source_dir):
    """
    功能描述：删除关键仓的代码
    参数： source_dir=vendor/hisi
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
    g2d_dir       = source_dir+'/confidential/g2d '
    g3d_dir       = source_dir+'/confidential/g3d '
    mcu_dir       = source_dir+'/confidential/mcu '
    onchiprom_dir = source_dir+'/confidential/onchiprom '
    videocodec_dir= source_dir+'/confidential/videocodec '
    med_build     = source_dir+'/modem/med/common ' 
    med_hip5xx    = source_dir+'/modem/med/hip5xx '
    med_hi6620    = source_dir+'/modem/med/hi6620 ' 
    phy_dir       = source_dir+'/modem/phy '
    ps_as         = source_dir+'/modem/ps/as '
    llt_dir       = source_dir+'/llt '
    
    tobedellist  = g2d_dir + g3d_dir + mcu_dir + onchiprom_dir +videocodec_dir
    tobedellist += med_build + med_hip5xx + med_hi6620 + phy_dir + ps_as + llt_dir
    
    if os.path.exists(os.path.dirname(phy_dir)) :
        errcode =os.system("rm -rf "+tobedellist)        
        
    #logf.close()        
    return 0

def restore_balong_libs(source_dir):
    """
    功能描述： 备份delivery/$(product)/balong_libs关键仓的二进制文件到hisi/libs下
    参数： source_dir=vendor/hisi
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
    delivery_dir  = source_dir+'/build/delivery/'
    #product_list  = os.listdir(delivery_dir)

    libs_dir   = delivery_dir+'/balong_libs/'
    dst_dir = source_dir +'/libs/'
    cp_cmd   = "cp -rf  "+ libs_dir +'* ' + dst_dir
    if os.path.exists(libs_dir) :
        errcode =os.system("mkdir -p "+dst_dir)        
        errcode =os.system(cp_cmd)        
        
    # for product in product_list: 
        # libs_dir   = delivery_dir+product+'/balong_libs/'
        # dst_dir = source_dir +'/libs/'+ product
        # cp_cmd   = "cp -rf  "+ libs_dir +'* ' + dst_dir
        # if os.path.exists(libs_dir) :
            # errcode =os.system("mkdir -p "+dst_dir)        
            # errcode =os.system(cp_cmd)        
            
    return 0        
    
def rm_camera_code(source_dir):
    """
    功能描述：删除camera的代码
    参数： source_dir=vendor/hisi
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2013年6月26日
                作者    : l00130025
                修改内容: 创建函数
    """
    camera_dir    = source_dir+'/confidential/camera'
    
    if os.path.exists(os.path.dirname(camera_dir)) :
        errcode =os.system("rm -rf "+camera_dir)        
        #print "rm -rf "+camera_dir
    
    #logf.close()        
    return 0    
    
