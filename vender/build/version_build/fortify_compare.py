#!/usr/bin/python
#-*- coding: UTF-8 -*-
#**************************************************************
#
#          版权所有 (C), 2001-2012,华为技术有限公司
#
#**************************************************************
#文件名    ：fortify_compare.py
#版本号    ：初稿
#作者      ：y00187525
#生成日期  ：2014年03月06日
#功能描述  ：静态检查之Fortify两次报告对比
#使用方法  :
#输入参数  : 
#输出参数  ：
#返回值    ：
#修改历史  ：
#1.日期    ：2014年03月06日
#  作者    ：y00187525
#  修改内容：创建文件

import xml.etree.ElementTree as ET
import os
import sys
import string


def count_critical_and_high(fortifyXmlFileList):
    iid_list = []
    Category_list = []
    Friority_list = []
    Abstract_list = []
    FileName_list = []
    LineStart_list = []
    Item_list = []
            
    for fortifyXmlFile in fortifyXmlFileList:
        tree_container_config = ET.parse(fortifyXmlFile)
        container_config = tree_container_config.getroot()
        ReportSection  = container_config.findall("ReportSection")[2]
        SubSections = ReportSection.findall("SubSection")
        total_count = 0

        for SubSection in SubSections:
            IssueListing = SubSection.find("IssueListing")
            Chart = IssueListing.find("Chart")
            GroupingSections = Chart.findall("GroupingSection")
            if None == GroupingSections:
                continue
            for GroupingSection in GroupingSections:
                issues = GroupingSection.findall("Issue")
                if None == issues:
                    continue
                for issue in issues:
                    iid = issue.attrib['iid']
                    Category = issue.find("Category").text
                    Abstract = issue.find("Abstract").text
                    Friority = issue.find("Friority").text
                    Primary = issue.find("Primary")
                    FileName = Primary.find("FileName").text
                    FilePath = Primary.find("FilePath").text
                    LineStart = Primary.find("LineStart").text
                    Snippet = Primary.find("Snippet")
                    if (Category.strip() == "Dead Code") or (Category.strip() == "Poor Style: Value Never Read") or (Category.strip() == "Poor Style: Variable Never Used"):
                        continue
                    AbstractFilter = Abstract
                    for i in range(0,9):
                        AbstractFilter = AbstractFilter.replace(str(i),"")
                    # Item = Category + ":" + AbstractFilter + ":" + Friority + ":" + FilePath
                    if None == Snippet or \
                    not "[false alarm]" in Snippet.text:
                        #if Friority == "Critical" or Friority == "High":
                        total_count += 1
                        iid_list.append(iid)
                        Category_list.append(Category)
                        Friority_list.append(Friority)
                        Abstract_list.append(Abstract)
                        FileName_list.append(FileName)
                        LineStart_list.append(LineStart)
                        SnippetFilter = Snippet.text
                        SnippetFilter = SnippetFilter.replace("\n","")
                        Item = Category + ":" + AbstractFilter + ":" + Friority + ":" + FilePath + ":" + SnippetFilter
                        Item_list.append(Item)
                        
    return total_count,iid_list,Category_list,Friority_list,Abstract_list,FileName_list,LineStart_list,Item_list

def genReport(fortifyXmlFileBaseDir, fortifyXmlFileNewDir, fortifyRptOutputDir):

    if not os.path.exists(fortifyXmlFileBaseDir) and not os.path.exists(fortifyXmlFileNewDir):
        print "Warning: fortifyXmlFileBaseDir and fortifyXmlFileNewDir not exist."
        # return 1

    if not os.path.exists(fortifyRptOutputDir):
        os.system("mkdir -p " + fortifyRptOutputDir)
    
    reload(sys)
    sys.setdefaultencoding('utf-8')
    fobj = open(fortifyRptOutputDir + '/report.html','w')
    
    total_count_base = 0
    iid_list_base = []
    Item_list_base = []
    if os.path.exists(fortifyXmlFileBaseDir):
        fortifyXmlFileBaseList = []
        basefilelist = os.listdir(fortifyXmlFileBaseDir)
        for basefile in basefilelist:
            if "fortify" in basefile:
                fortifyXmlFileBaseList.append(fortifyXmlFileBaseDir + "/" + basefile)
        if len(fortifyXmlFileBaseList) > 0:
            total_count_base,iid_list_base,Category_list_base,Friority_list_base,Abstract_list_base,FileName_list_base,LineStart_list_base,Item_list_base = count_critical_and_high(fortifyXmlFileBaseList)

    total_count_new = 0
    iid_list_new = []
    Item_list_new = []
    if os.path.exists(fortifyXmlFileNewDir):
        fortifyXmlFileNewList = []
        newfilelist = os.listdir(fortifyXmlFileNewDir)
        for newfile in newfilelist:
            if "fortify" in newfile:
                fortifyXmlFileNewList.append(fortifyXmlFileNewDir + "/" + newfile)
        if len(fortifyXmlFileNewList) > 0:
            total_count_new,iid_list_new,Category_list_new,Friority_list_new,Abstract_list_new,FileName_list_new,LineStart_list_new,Item_list_new = count_critical_and_high(fortifyXmlFileNewList)
    
    report = "<html>" + "\n" + "<head>" + "\n" + "<meta charset=\"UTF-8\"> </meta>" + "<title>" +"static_check_fortify" + "</title>" + "\n" + "</head>" + "\n" + "<body><center>" + "\n"
    report += "<h2>Fortify Report</h2>" + "\n"
    # report += "<p>Base Defects: " + str(total_count_base) + "&nbsp&nbsp&nbsp" + "Current Defects: " + str(total_count_new) + "</p>" + "\n"
    tr_str = "<tr bgcolor=\"#F8F8F2\">"
    tr_str += "<td>No.</td>"
    tr_str += "<td>Type</td>"
    tr_str += "<td>Priority</td>"
    tr_str += "<td>Description</td>"
    tr_str += "<td>File</td>"
    tr_str += "<td>Line</td>"
    tr_str += "</tr>" + "\n"

    No = 0
        
    for iid in iid_list_new:
        if not iid in iid_list_base:
            Index = iid_list_new.index(iid)
            Category = Category_list_new[Index]
            Friority = Friority_list_new[Index]
            Abstract = Abstract_list_new[Index]
            FileName = FileName_list_new[Index]
            LineStart = LineStart_list_new[Index]
            Item = Item_list_new[Index]
            if Item in Item_list_base:
                continue
            #gen report
            No = No + 1
            tr_str += "<tr bgcolor=\"#F8F8F2\">"
            tr_str += "<td>" + str(No) + "</td>"
            tr_str += "<td>" + str(Category) + "</td>"
            tr_str += "<td>" + str(Friority) + "</td>"
            tr_str += "<td>" + str(Abstract) + "</td>"
            tr_str += "<td>" + str(FileName) + "</td>"
            tr_str += "<td>" + str(LineStart) + "</td>"
            tr_str += "</tr>" + "\n"
            
    if No > 0:
        report += "<p>" + "<font color=\"blue\">New Defects : </font>" + "<font color=\"red\">" + str(No) + "</font></p>" + "\n"
        report += "<table cellPadding=\"8\">" + "\n"
        report += tr_str
        report += "</table>" + "\n"
        report += "</center></body>" + "\n" + "</html>"   
        fobj.write(report)
        fobj.close()
    else:
        report += "<p><font color=\"blue\">" + "No New Defects" + "</font></p>" + "\n"
        report += "</center></body>" + "\n" + "</html>"   
        fobj.write(report)
        fobj.close()
                
    return No

def main():

    if 4 > len(sys.argv):
        print "This .py needs 3 args except args[0]!"
        return 1
        
    fortifyXmlFileBaseDir  = sys.argv[1]
    fortifyXmlFileNewDir   = sys.argv[2]
    fortifyRptOutputDir   = sys.argv[3]

    return genReport(fortifyXmlFileBaseDir, fortifyXmlFileNewDir, fortifyRptOutputDir)
    

if __name__ == '__main__':
    sys.exit(main())




