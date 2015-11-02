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

import commands
import json

THIS_FILE_NAME = __file__

GERRIT_IP = "10.97.84.145"
GERRIT_PORT = "29418"
GERRIT_USER = "jenkins_ci"

def GetPatchSetFileList(files):
    fileList=[]
    for eachObj in files:
        if not eachObj['file'] == "/COMMIT_MSG":
            fileList.append(eachObj['file'])
    return fileList
        
def get_gerrit_info(branch,myChangeId,gerrit_ip=GERRIT_IP,gerrit_port=GERRIT_PORT, gerrit_user=GERRIT_USER):
    """
    功能描述:根据changeid从gerrit上获取change list信息
    参数： source_dir=vendor/hisi
    返回值： 无
    异常描述：
    修改记录：1.日期    : 2014年01月13日
                作者    : y00187525
                修改内容: 创建函数
    """
    myProjects = []
    myStatus = []
    myfiles = []
    myProjectsCount = 0
    branch = branch.replace("_PRE_COMPILE","")
    cmd = 'ssh -p %s %s@%s gerrit query --format=JSON --current-patch-set --files %s branch:%s'%(gerrit_port, gerrit_user, gerrit_ip, myChangeId, branch)
    ret = commands.getstatusoutput(cmd)
    if 0 != ret[0]:
        print "ChangeId is not exists!\n%s\n" %(ret[1])
    else:    
        projectInfoList = ret[1].split('\n')[:-1]
        myProjectsCount = len(projectInfoList)
        
    #获取project信息
    for projectIndex in range(0,myProjectsCount):
        myProjects.append(json.loads(projectInfoList[projectIndex])["project"])
        myStatus.append(json.loads(projectInfoList[projectIndex])["status"])
        myfiles.append(GetPatchSetFileList(json.loads(projectInfoList[projectIndex])["currentPatchSet"]["files"]))
        
    return myProjects,myStatus,myfiles
