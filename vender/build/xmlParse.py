# -*- coding=utf-8 -*-
'''
Created on 2014-2-14

@author: l00258701
'''
#encoding=utf-8
import sys
from xml.dom import minidom as Xml
#import principal
import sys
reload(sys)
sys.setdefaultencoding('utf-8') 

issueTagList = ['Category','Folder','Kingdom','Abstract','Friority','Primary']
PrimaryTagList = ['FileName','FilePath','LineStart','Snippet']
principalList = {}
counterList = {'未确定' : 0,
'商绵友' : 0,
'李晓杰' : 0,
'刘文慧' : 0,
'张静远' : 0,
'翟继业' : 0,
'徐经翠' : 0,
'王献东' : 0,
'卢锁' : 0,
'徐文芳' : 0,
'朱鸿飞' : 0,
'敬勇' : 0,
'王忠顺' : 0,
'陈东岳' : 0,
'鲁婷' : 0,
'王心力' : 0,
'左凤华' : 0,
'姚国才' : 0,
'崔军强' : 0,
'付欣' : 0,
'于洋洋' : 0,
'王伟超' : 0,
'杨志' : 0,
'杨强' : 0,
'终端' : 0,
'郝然' : 0,
'毕小涛' : 0,
'刘永鹏' : 0
}
xmlPath = "D:/l00258701/workspace/fortify/src/hisi/drv.xml"
def getPrincipal(filePath):
    keyList = principalList.keys();
    for key in keyList:
        if(filePath.__contains__(key)):
            #print filePath + "   " + key
            #print principalList.get(key)
            return principalList.get(key)

       
        
    
def outHtmlHead(html):
    html.write('<html>\n')
    html.write('<title>Fortify扫描</title>\n')
    html.write('<head>')
    html.write('<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">\n')
    html.write('</head>\n')
    html.write('<body>\n')

def outHtmlTail(html):
    html.write('</body>\n')
    html.write('</html>\n')

def outHtmlTableTh(html):
    html.write('<table border="1">\n')
    html.write('<tr>\n')
    html.write('<th>Category</th>\n')
    html.write('<th>Folder</th>\n')
    html.write('<th>Kingdom</th>\n')
    html.write('<th>Abstract</th>\n')
    html.write('<th>Friority</th>\n')
    html.write('<th>FileName</th>\n')
    html.write('<th>FilePath</th>\n')
    html.write('<th>LineStart</th>\n')
    html.write('<th>Snippet</th>\n')
    html.write('<th>是否为误报    </th>\n')
    html.write('<th>误报分析备注   </th>\n')
    html.write('<th>责任人   </th>\n')
    html.write('</tr>\n')
    
def outHtmlTableTr(html,tableList):
    table = "   "
    global falseAlarm
    global hasowner
    if((falseAlarm == False)):
        html.write('<tr>\n')
        for table in tableList:
            html.write('<td>  ')
            html.write(str(table).encode("utf-8"))
            html.write('  </td>')
            html.write('\n')
        
        html.write('</tr>\n')
     
def outputHtml(filename,tableList):
    html = open(filename,'w')
    outHtmlHead(html,filename)
    outHtmlTableTh(html);
    #outHtmlTableTr(html,tableList);
    #outHtmlTail(html);

def outputCountTableTh(html):
    html.write('<table border="1">\n')
    html.write('<tr>\n')
    html.write('<th>责任人</th>\n')
    html.write('<th>错误数</th>\n')
    html.write('</tr>\n')

def outputCountTableTr(html):
    counterKeys = counterList.keys()
    for counterKey in counterKeys:
        html.write('<tr>\n')
        html.write('<td>  ')
        html.write(str(counterKey) + '\n')
        html.write('  </td>')
        html.write('<td>  ')
        html.write(str(counterList.get(counterKey)) + '\n')
        html.write('  </td>')
        html.write('\n')
        html.write('</tr>\n')
    
def getBasename():
    return sys.argv[1]
    
def outPutCounterHtml():
    basePath = getBasename()
    html = open(basePath + "_waring_counter.html",'w')
    outHtmlHead(html)
    outputCountTableTh(html);
    outputCountTableTr(html);
    html.write('</table>\n');
    outHtmlTail(html)
    
    
config = open("config.ini",'rb')
line = config.readline()
while(line !=  ''):
    list = line.split();
    lenList = len(list)
    principalList[list[0]] = list[lenList - 1]
    line = config.readline()
xmlPath = getBasename()
dom = Xml.parse(xmlPath)

classlist = dom.getElementsByTagName("ReportSection")

issueList = classlist[2].getElementsByTagName("Issue")

resultList = []
filePath = ''
owner = ''
falseAlarm = False
falseTag = "[false alarm]:"
reason = ''
html = open(getBasename() + "_result.html",'w')
outHtmlHead(html)
outHtmlTableTh(html);
hasowner = False

for issue in issueList:
    for issueTag in issueTagList:
        if issueTag != 'Primary':
            commList = issue.getElementsByTagName(issueTag)
            resultList.append( commList[0].childNodes[0].data)
        else :
            for PrimaryTag in PrimaryTagList:
                PrimaryList = issue.getElementsByTagName(PrimaryTag)
                for Primary in PrimaryList:
                    if(PrimaryTag == 'FilePath'):
                        owner = ''
                        filePath = ( Primary.childNodes[0].data)
                        owner = getPrincipal(filePath)
                    
                    resultList.append( Primary.childNodes[0].data)
                    if( PrimaryTag == 'Snippet'):
                        falseAlarm = False
                        Snippet = Primary.childNodes[0].data
                        if(falseTag in Snippet):
                            resultList.append(' ' + "是" + ' ')
                            reasonIndex = Snippet.find(falseTag)
                            reasonEnd = Snippet.find("*/",reasonIndex)
                            reasonIndex += len(falseTag)
                            reason = Snippet[reasonIndex : reasonEnd];
                            resultList.append(reason)
                            falseAlarm = True
                        else:
                            resultList.append(' ' + "否" + ' ')
                            resultList.append("非误报")
                        resultList.append(owner)
                        counterKeys = counterList.keys()
                        for counterKey in counterKeys:
                            if(counterKey == owner):
                                if(falseAlarm == False):
                                    value = counterList.get(counterKey)
                                    value += 1
                                    counterList[counterKey] = value
    if(resultList[len(resultList) - 1] != None):
        outHtmlTableTr(html,resultList)
    resultList = []
    filePath = ''
   
    
    
html.write('</table>\n');
outHtmlTail(html)

outPutCounterHtml()

