#!/usr/bin/python

import os
import sys
import xml.etree.ElementTree as ET
import pwd

def readXml(inputFile):
    tree = ET.ElementTree()
    tree.parse(inputFile)
    return tree

def findNodes(tree, tagName):
    return tree.findall(tagName)

def getNodes(nodes):
    nodelist = []
    for node in nodes:
        nodelist.append(node.attrib["name"])
    return nodelist
        
def getDiffNodes(baseNodeList, curNodeList):
    nodeList = []
    for node in curNodeList:
        if node not in baseNodeList:
            nodeList.append(node)
    return nodeList

def getDiffNodeList(lwNodeList, hwNodeList, curNodeList):
    nodeList = []
    for item in curNodeList:
        if item not in lwNodeList and item not in hwNodeList:
            nodeList.append(item)
    return nodeList
        
def main(lwXmlPath, hwXmlPath, curXmlPath):

    lwTree = readXml(lwXmlPath)
    hwTree = readXml(hwXmlPath)
    curTree = readXml(curXmlPath)

    lwNodes = findNodes(lwTree, "project")
    hwNodes = findNodes(hwTree, "project")
    curNodes = findNodes(curTree, "project")

    lwNodeList = getNodes(lwNodes)
    hwNodeList = getNodes(hwNodes)
    curNodeList = getNodes(curNodes)

    print "new inc stores in manifest.xml:"
    diffNodeList = getDiffNodeList(lwNodeList, hwNodeList, curNodeList) 
    print diffNodeList
        
    print "\nbelow is in lw but not in lastest manifest:"
    diffNodeList = getDiffNodes(curNodeList, lwNodeList)
    print diffNodeList
    
    print "\nbelow is in hw but not in lastest manifest:"
    diffNodeList = getDiffNodes(curNodeList, hwNodeList)
    print diffNodeList
    return 0

if __name__ == "__main__":
    lwXmlPath = "BalongV9R1SFT_lw_manifest.xml"
    hwXmlPath = "BalongV9R1SFT_nlw_manifest.xml"
    curXmlPath = "./../../../../.repo/manifest.xml"
    
    if not os.path.exists(lwXmlPath) or not os.path.exists(hwXmlPath) or not os.path.exists(curXmlPath):
        print "manifest not exists"
        sys.exist(-1)
    sys.exit(main(lwXmlPath, hwXmlPath, curXmlPath))

