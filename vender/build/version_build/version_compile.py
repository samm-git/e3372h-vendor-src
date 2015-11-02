#!/usr/bin/python
#-*- coding: UTF-8 -*-
#**************************************************************
#
#          版权所有 (C), 2001-2011,华为技术有限公司
#
#**************************************************************
#文件名    ：version_complie.py
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
import copy
import platform
import datetime
sys.path.append("../scripts")
import comm
import nlw_image_save
import xml.etree.ElementTree as ET
import mv_code_dir
import build_utility
import GerritInfo

THIS_FILE_NAME = __file__

BUILD_TARGET_PRE_CB_LW ="PRE_CB_LW"
BUILD_TARGET_PRE_CB_NLW ="PRE_CB_NLW"
BUILD_TARGET_VB_ALL ="VB_ALL"
 
def is_in_manifest(project_list, manifestFile):
    """
    功能描述:判断指定的仓列表是否在指定的manifest中有交集
    参数： project_list，manifestFile 
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2014年01月13日
                作者    : y00187525
                修改内容: 创建函数
    """
    #stable_rom_manifest_file = "BalongV9R1SFT_nlw_manifest.xml"

    stable_rom_manifest = ET.parse(manifestFile)
    config = stable_rom_manifest.getroot()

    for projectItem in project_list:
        for project_cfg in config.findall('project'):
            if project_cfg.attrib['name'] == projectItem:
                return "true"
                
    return "false"

def is_merged_version_build(changeid_status):
    """
    功能描述:根据是否存在OPEN的仓判断是pre还是vb编译
    参数： changeid_status
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2014年01月13日
                作者    : y00187525
                修改内容: 创建函数
    """
    merged_version_build = "true"
    for status in changeid_status:
        if 'MERGED' != status:
            merged_version_build = "false"
            break
    return merged_version_build

def judge_build_target_type(params_dict,myChangeId,branch_name):
    """
    功能描述：根据changeid_git_list是否与stable_rom_manifest有交集，判断轻量级还是重量级编译;
    参数： Gerrit的端口号 gerrit_port; 用户名:gerrit_user; GerretIP:gerrit_ip ; myChangeId: changeid,branch_name:分支名
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2014年01月13日
                作者    : y00187525
                修改内容: 创建函数
    """
    # BUILD_TARGET_PRE_CB_LW:表示pre轻量级编译  BUILD_TARGET_PRE_CB_NLW:表示pre非轻量级编译  BUILD_TARGET_VB_ALL:表示vb编译    
    # Temp : add by zhangteng
    return BUILD_TARGET_VB_ALL
    # End
    if string.find(branch_name, '_PRE_COMPILE') == -1 :
        build_target = BUILD_TARGET_VB_ALL
    elif params_dict.has_key('lwbuild') and params_dict['lwbuild'] =='false' :
        build_target = BUILD_TARGET_VB_ALL
    else:
        build_target = BUILD_TARGET_PRE_CB_LW
        dict = GerritInfo.get_gerrit_info(branch_name,myChangeId)

        myProjects = dict[0]
        myStatus = dict[1]
        if len(myProjects) == 0:
            build_target = BUILD_TARGET_PRE_CB_NLW
        else:
            #是否需要VB编译
            if "true" == is_merged_version_build(myStatus):
                build_target = BUILD_TARGET_VB_ALL
            else:
                ret = is_in_manifest(myProjects, "version_build/BalongV9R1SFT_nlw_manifest.xml")
                if "true" == ret:
                    build_target = BUILD_TARGET_PRE_CB_NLW
    
    return build_target

def get_upated_system(myChangeId,branch_name):
    """
    功能描述：根据changeid_git_list是否与modem_manifest和ap_manifest有交集，判断更新的子模块;
    参数：  myChangeId: changeid,branch_name:分支名
    返回值： ap/gu/tl/x/ci/tool以及用，隔开的组合
    异常描述：
    修改记录：1.日期    : 2014年06月30日
                作者    : l00130025
                修改内容: 创建函数
    """
    # updated_system

    dict = GerritInfo.get_gerrit_info(branch_name,myChangeId)
    myProjects = dict[0]
    myStatus = dict[1]
    
    if len(myProjects) != 0:
    
        sub_system=''
        
        modem_check_ret = is_in_manifest(myProjects, "version_build/Balong_modem_manifest.xml")
        ap_check_ret = is_in_manifest(myProjects, "version_build/Balong_ap_manifest.xml")
        ci_check_ret = is_in_manifest(myProjects, "version_build/Balong_ci_manifest.xml")
        
        if "true" == ap_check_ret:
            sub_system+='ap,'
        if "true" == modem_check_ret:
            sub_system+='gu,'                    
        if "true" == ci_check_ret:
            sub_system+='ci,'
            
        if  '' == sub_system  :
            sub_system='ap,gu'             
    else:
        sub_system='ap,gu'
        
    return sub_system.strip(',')

def IsnlwSwitchToLw(isBuildTypeSwitched, buildgroup, xmlbuildgroupinfo, params_dict):
    if isBuildTypeSwitched == True:
        nlw_image_path = nlw_image_save.retractPath('version_build/BalongV9R1SFT_nlw_manifest.xml', '/home/jenkins_ci/vb_image/' + buildgroup, xmlbuildgroupinfo) 
        if nlw_image_path != "":
            params_dict["nlw_image_path"] = nlw_image_path
            return True
        else:
            return False
    else:
        return False
def main():
    curtime = datetime.datetime.now()
    print "version_compile start time:" +  curtime.strftime("%H:%M:%S")
    argv_list = sys.argv[1:]
    if len(argv_list) == 0 :
            print "fail,parameters_is_empty"
            return 1
        
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
       not params_dict.has_key('owners'):
            print "fail,params has not enough key"
            return 1
                   
    branch_name = params_dict['branch']
    timestamp = params_dict['timestamp']
    version_name = params_dict['version']
    buildgroup = params_dict['buildgroup']
    changeid = params_dict['changeid']
        
    buildtype ='all'
    if params_dict.has_key('buildtype'):
        buildtype = params_dict['buildtype']

    target =''
    if params_dict.has_key('target'):
        target = params_dict['target']

    if not params_dict.has_key('transfer'):
        params_dict['transfer'] = 'true'   
   

    source_dir = os.path.abspath('../..')

    complie_entry_dir = source_dir+'/build'
    os.chdir(complie_entry_dir)
    
    build_rule_file = source_dir + "/build/version_build/build_rule.xml"
    producttype = build_utility.get_group_product_type(build_rule_file,version_name,buildgroup) 
    print "producttype:" +  producttype
    root_dir = source_dir + "/../../"
    code_bak_dir = root_dir + "/../code_back"
    lw_Xml_Path = source_dir + "/build/version_build/BalongV9R1SFT_lw_manifest.xml"
    hw_Xml_path = source_dir + "/build/version_build/BalongV9R1SFT_nlw_manifest.xml"

    rm_bak_code_dir_cmd = "rm -rf " + code_bak_dir
    #清除上次的编译记录
    build_utility.CleanOldBuildFile(source_dir)
    
    build_result_str =''
    isBuildTypeSwitched = False

    # release 构建时,使用产品线发布方式构建
    # be_release_build = build_utility.is_group_release_build(build_rule_file,version_name,buildgroup)

    # if 'true' == be_release_build:
        # 编译发布的二进制文件
        #1)build balong-libs 2)构造产品线编译的环境 3)rm confidential 4)restor balong libs
        # if producttype == "mbb" or \
            # producttype == "phone" :        
            # relbuildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,"balong-libs")
            # build_result_str +=build_utility.exec_group_build(relbuildscriptdic)
            # rm_confidential_code(source_dir)
            # restore_balong_libs(source_dir)
            
        # 编译camera后,删除camera代码
        # if producttype == "phone" :
            # relbuildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,"camera",params_dict)
            # build_result_str +=build_utility.exec_group_build(relbuildscriptdic)            
            # rm_camera_code(source_dir)
            
    
    # 按产品线发布方式编译，传出版本，做内部验证
    if producttype == "tool":
        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict)
        build_result_str += build_utility.exec_sequ_build(buildscriptdic)
    elif producttype == "ut" :
        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict, False)
        build_result_str += build_utility.exec_group_build(buildscriptdic)        
    elif producttype == "inc_chk" :
        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict, False)
        build_result_str += build_utility.exec_group_build(buildscriptdic)        
    elif producttype != "phone" :
        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict)
        build_result_str += build_utility.exec_group_build(buildscriptdic)
    else:
        xmlbuildgroupinfo = buildgroup + "\n"
        xmlbuildgroupdic = build_utility.get_rules_of_version(build_rule_file, version_name)
        if xmlbuildgroupdic.has_key(buildgroup):
            xmlbuildgroupinfo += str(xmlbuildgroupdic[buildgroup])
        buildtargettype = judge_build_target_type(params_dict,changeid,branch_name)
        print 'buildtargettype:' + buildtargettype

        # Temp : add by zhangteng
        #params_dict['system_update'] = get_upated_system(changeid,branch_name)
        # End

        os.system(rm_bak_code_dir_cmd)

        if (target == '' or target == 'balong') :
            if buildtargettype == BUILD_TARGET_PRE_CB_LW:
                curtime = datetime.datetime.now()
                print "search vb_img path start time:" +  curtime.strftime("%H:%M:%S") 
                nlw_image_path = nlw_image_save.retractPath('version_build/BalongV9R1SFT_nlw_manifest.xml', '/home/jenkins_ci/vb_image/' + buildgroup, xmlbuildgroupinfo) 
                curtime = datetime.datetime.now()
                print "search vb_img path end time:" +  curtime.strftime("%H:%M:%S") 
                if ("" != nlw_image_path) :
                    target = "lw_rom"
                    params_dict["nlw_image_path"] = nlw_image_path
                    mv_code_dir.main("hwrom", root_dir, code_bak_dir, lw_Xml_Path, hw_Xml_path)                    
                    buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict)                 
                    build_result_str += build_utility.exec_group_build(buildscriptdic)
                    print 'lw_rom build result:' + build_result_str
                    mv_code_dir.main("hwrom", code_bak_dir, root_dir, lw_Xml_Path, hw_Xml_path)                    
                    os.system(rm_bak_code_dir_cmd)
                else :
                    buildtargettype = BUILD_TARGET_PRE_CB_NLW
                    isBuildTypeSwitched = True
                    print "LW -> NLW"

            if buildtargettype == BUILD_TARGET_PRE_CB_NLW:
                # 切换轻量级别视图 TBD                
                curtime = datetime.datetime.now()
                print "lw_rom_modules start time:" +  curtime.strftime("%H:%M:%S")
                mv_code_dir.main("hwrom", root_dir, code_bak_dir, lw_Xml_Path, hw_Xml_path)
                # 调用轻量级别编译 
                target_lw = "lw_rom_modules"
                lw_params_dict = copy.deepcopy(params_dict)
                if not lw_params_dict.has_key('trans_targets') or lw_params_dict["trans_targets"] == "":
                    lw_params_dict['trans_targets'] = "log"
                buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target_lw,lw_params_dict, True)                
                build_result_str += build_utility.exec_group_build(buildscriptdic)                               
                print 'lw_rom_modules build result:' + build_result_str
                curtime = datetime.datetime.now()
                print "lw_rom_modules end time:" +  curtime.strftime("%H:%M:%S")

                if (not 'fail' in build_result_str) :
                    # 切换重量级别视图 
                    mv_code_dir.main("hwrom", code_bak_dir, root_dir, lw_Xml_Path, hw_Xml_path)
                    if IsnlwSwitchToLw(isBuildTypeSwitched, buildgroup, xmlbuildgroupinfo, params_dict) == True:
                        print "NLW -> LW"
                        target_lw = "pkg_lw_rom"
                        curtime = datetime.datetime.now()
                        print "pkg_lw_rom start time:" +  curtime.strftime("%H:%M:%S")
                        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target_lw,params_dict, False)                
                        build_result_str += build_utility.exec_group_build(buildscriptdic)                               
                        curtime = datetime.datetime.now()
                        print "pkg_lw_rom end time:" +  curtime.strftime("%H:%M:%S")
                    else:
                        mv_code_dir.main("lwrom", root_dir, code_bak_dir, lw_Xml_Path, hw_Xml_path)
                    # 调用重量级别编译   传输
                        target_nlw = "pre_cb_rom"
                        curtime = datetime.datetime.now()
                        print "pre_cb_rom start time:" +  curtime.strftime("%H:%M:%S")
                        buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target_nlw,params_dict, False)                                    
                        build_result_str += build_utility.exec_group_build(buildscriptdic)                                                      
                        print 'pre_cb_rom result' + build_result_str
                        curtime = datetime.datetime.now()
                        print "pre_cb_rom end time:" +  curtime.strftime("%H:%M:%S")
                     # 切换重量级别视图 
                        mv_code_dir.main("lwrom", code_bak_dir, root_dir, lw_Xml_Path, hw_Xml_path)
                else :
                    mv_code_dir.main("hwrom", code_bak_dir, root_dir, lw_Xml_Path, hw_Xml_path)
                os.system(rm_bak_code_dir_cmd)

            if buildtargettype == BUILD_TARGET_VB_ALL:
                buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict)               
                build_result_str += build_utility.exec_group_build(buildscriptdic)                             
               # print 'vb result:' + build_result_str
               # if (not 'fail' in build_result_str) :
               #     vbBankDir = '/home/jenkins_ci/vb_image'
               #     if os.path.isdir(vbBankDir) :
               #         curtime = datetime.datetime.now()
               #         print "rsync to vb_img start time:" +  curtime.strftime("%H:%M:%S") 
               #         nlw_image_save_result = nlw_image_save.storeImage('version_build/BalongV9R1SFT_nlw_manifest.xml', vbBankDir + '/' + buildgroup, xmlbuildgroupinfo, "delivery") 
               #         curtime = datetime.datetime.now()
               #         print "rsync to vb_img end time:" +  curtime.strftime("%H:%M:%S") 
               #         if nlw_image_save_result:
               #             print "nlw_image_save excute failed!\n"
               #         else:
               #             print "nlw_image_save excute success!\n"
               #     else :
               #         print "vb bank is not exist, ignore nlw_image_save!\n"
        else :
            curtime = datetime.datetime.now()
            print "HIbuild start time:" +  curtime.strftime("%H:%M:%S")
            buildscriptdic = build_utility.gen_build_scripts(source_dir,version_name,branch_name,buildgroup,buildtype,target,params_dict)
            build_result_str += build_utility.exec_group_build(buildscriptdic)
            curtime = datetime.datetime.now()
            print "HIbuild end time:" +  curtime.strftime("%H:%M:%S")
    # 返回编译结果  
    print "VERSION_BUILD_RESULT:" + build_result_str
    return 0
    

if __name__ == '__main__':
    main()
