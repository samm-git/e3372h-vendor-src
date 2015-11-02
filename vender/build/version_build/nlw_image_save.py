#!/bin/python
#-*- coding: UTF-8 -*-

import os
import sys
import string
import hashlib
from xml.dom.minidom import parseString
       
# 读取一个完整的文件内容,返回字符串
def readFile(fileName) :
    if not os.path.isfile(fileName) :
        print "Error: \"%s\" file isn't exist." % fileName
        return None
    content = ""
    fd = open(fileName, 'r')
    content = fd.read()
    fd.close()
    return content

# 将字符串内容写入一个文件
def writeFile(fileName, data) :
    fd = open(fileName, 'w')
    fd.write(data)
    fd.close

class ManifestXml :
    def __init__(self) :
        self.nameTag = "name"
        self.pathTag = "path"
        self.projTag = "project"
        self.reviTag = "revision"

    # 解析xml文件,返回project项对应的path和revision
    def parseXml(self, xmlFile) :
        data = readFile(xmlFile)
        if None == data:
            return None
        prjDict = {}
        dom = parseString(data)
        projectList = dom.getElementsByTagName(self.projTag)
        for project in projectList :
            prjPath = project.getAttribute(self.pathTag)
            prjName = project.getAttribute(self.nameTag)
            prjRevi = project.getAttribute(self.reviTag)
            if "" == prjPath :
                prjPath = prjName
            prjDict[prjName] = [prjPath, prjRevi]
        return prjDict

class ManifestHash :
    def __init__(self, refXmlFile, currXmlFile) :
        self.refXmlFile = refXmlFile
        self.currXmlFile = currXmlFile

    # 解析参照xml文件的revision集合 
    def getHashContent(self) :
        manifestXml = ManifestXml()
        refPrj = manifestXml.parseXml(self.refXmlFile)
        currPrj = manifestXml.parseXml(self.currXmlFile)
        if (None == refPrj) or (None == currPrj) :
            return ""
        for project in refPrj :
            if not project in currPrj :
                print "Error: \"%s\" not in the current manifest." % project
                return ""
            if refPrj[project][0] != currPrj[project][0] :
                print "Error: \"%s\" name and path are mismathed." % project
                return ""
            refPrj[project][1] = currPrj[project][1]
        manifestItems = refPrj.items()
        manifestItems.sort()
        return str(manifestItems)

    # 读取一个文件，并用文件内容更新Hash值
    def updateMd5(self, imageFile, imageHash) :
        fd = open(imageFile, 'r')
        while(True) :
            str = fd.read(4096)
            if (str == ""):
                break
            imageHash.update(str)
        fd.close()

    # 搜查产品目录下的lib目录，计算镜像的MD5值
    def updateMd5Rec(self, srcPath, imageHash) :
        if os.path.isfile(srcPath) :
            self.updateMd5(srcPath, imageHash)
        elif os.path.isdir(srcPath) :
            fileList = os.listdir(srcPath)
            fileList.sort()
            for eachFile in fileList :
                subSrcPath = srcPath + "/" + eachFile
                self.updateMd5Rec(subSrcPath, imageHash)

    # 搜查产品目录下的buildinfo.txt和lib目录，计算镜像的MD5值，拷贝文件如果需要
    def copyImagesLevel1(self, srcPath, dstPath, imageHash, copyFlg) :
        fileList = os.listdir(srcPath)
        fileList.sort()
        for eachFile in fileList :
            subSrcPath = srcPath + "/" + eachFile
            subDstPath = dstPath + "/" + eachFile
            if os.path.isdir(subSrcPath) :
                if copyFlg :
                    os.system("mkdir -p " + subDstPath)
                infoSrcPath = subSrcPath + "/" + "buildinfo.txt"
                infoDstPath = subDstPath + "/" + "buildinfo.txt"
                libsSrcPath = subSrcPath + "/" + "lib"
                libsDstPath = subDstPath + "/" + "lib"
                if os.path.isfile(infoSrcPath) :
                    if copyFlg :
                        os.system("rsync -q " + infoSrcPath + " " + infoDstPath)
                    self.updateMd5Rec(infoSrcPath, imageHash)
                if os.path.isdir(libsSrcPath) :
                    if copyFlg :
                        os.system("rsync -q -r " + libsSrcPath + " " + subDstPath)
                    self.updateMd5Rec(libsSrcPath, imageHash)

    # 将指定配置路径下的文件，存放到指定的存储区
    def store(self, bankPath, groupInfo, deliveryPath) :
        hashContent = self.getHashContent()
        if "" == hashContent :
            return -1
        manifestHash = hashlib.md5()
        manifestHash.update(groupInfo)
        manifestHash.update(hashContent)
        hashValue = manifestHash.hexdigest()
        bankSpecifyDir = bankPath + "/" + hashValue
        bankHashFile = bankSpecifyDir + "/" + "HashContent.txt"
        if os.path.isfile(bankHashFile) :
            data = readFile(bankHashFile)
            if (data == groupInfo + "\n" + hashContent) :
                return 0
            else :
                print "Error: \"%s\" and \"%s\" are same Hash." % (data, hashContent)
                return -1
        os.system("mkdir -p " + bankSpecifyDir)
        writeFile(bankHashFile, groupInfo + "\n" + hashContent)
        imageHash = hashlib.md5()
        self.copyImagesLevel1(deliveryPath, bankSpecifyDir, imageHash, True)
        imageHashValue = imageHash.hexdigest()
        writeFile(bankSpecifyDir + "/" + "md5.txt", imageHashValue)
        print "Info : \"%s\" is generated." % (bankSpecifyDir + "/" + "md5.txt")
        return 0

    # 返回指定位置的缓存区路径，供获取镜像的拷贝
    def retract(self, bankPath, groupInfo) :
        hashContent = self.getHashContent()
        if "" == hashContent :
            print "Error: get Hash content fail." 
            return ""
        manifestHash = hashlib.md5()
        manifestHash.update(groupInfo)
        manifestHash.update(hashContent)
        hashValue = manifestHash.hexdigest()
        bankSpecifyDir = bankPath + "/" + hashValue
        if not os.path.isfile(bankSpecifyDir + "/" + "md5.txt") :
            print "Error: \"%s\" is not exist." % (bankSpecifyDir + "/" + "md5.txt")
            return ""
        bankHashFile = bankSpecifyDir + "/" + "HashContent.txt"
        if not os.path.isfile(bankHashFile) :
            print "Error: \"%s\" is not exist." % bankHashFile
            return ""
        data = readFile(bankHashFile)
        if (data != groupInfo + "\n" + hashContent) :
            print "Error: \"%s\" and \"%s\" are same Hash." % (data, hashContent)
            return ""
        imageHash = hashlib.md5()
        self.copyImagesLevel1(bankSpecifyDir, ".", imageHash, False)
        imageHashValue = imageHash.hexdigest()
        data = readFile(bankSpecifyDir + "/" + "md5.txt")
        if data != imageHashValue :
            print "Error: \"%s\" and \"%s\" are not equal." % (data, imageHashValue)
            return ""
        return bankSpecifyDir

# 存储镜像
def storeImage(refXmlFile, bankPath, groupInfo, deliveryPath) :
    currXmlFile = ".repo_curr_revision.xml"
    os.system("repo manifest -r -o " + currXmlFile + " 1> /dev/null 2>&1 ") 
    manifestHash = ManifestHash(refXmlFile, currXmlFile)
    result = manifestHash.store(bankPath, groupInfo, deliveryPath)
    os.system("rm -f " + currXmlFile)
    return result

# 返回存储镜像的路径
def retractPath(refXmlFile, bankPath, groupInfo) :
    currXmlFile = ".repo_curr_revision.xml"
    os.system("repo manifest -r -o " + currXmlFile + " 1> /dev/null 2>&1 ") 
    manifestHash = ManifestHash(refXmlFile, currXmlFile)
    result = manifestHash.retract(bankPath, groupInfo)
    os.system("rm -f " + currXmlFile)
    return result

# 测试函数，测试存储镜像函数
def testStoreImage() :
    groupName = "hi6620oem_gutl_dsda"
    groupInfo = "<buildgroup name=hi6620oem_gutl_dsda weight=5 >"
    refXmlFile = "BalongV9R1SFT_nlw_manifest.xml"
    bankPath = "/home/w67364/mount_test" + "/" + groupName
    deliveryPath = "../delivery"
    storeImage(refXmlFile, bankPath, groupInfo, deliveryPath)

# 测试函数，测试读取存储镜像路径
def testretractImage() :
    groupName = "hi6620oem_gutl_dsda"
    groupInfo = "<buildgroup name=hi6620oem_gutl_dsda weight=5 >"
    refXmlFile = "BalongV9R1SFT_nlw_manifest.xml"
    bankPath = "/home/w67364/mount_test" + "/" + groupName
    print retractPath(refXmlFile, bankPath, groupInfo)
    
def main() :
    testStoreImage()
    testretractImage()

if __name__ == "__main__" :
    main()
  
