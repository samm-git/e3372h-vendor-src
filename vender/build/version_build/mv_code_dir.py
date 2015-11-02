#!/usr/bin/python

import os
import sys
import xml.etree.ElementTree as ET
import pwd

def readXml(in_path):
    tree = ET.ElementTree()
    tree.parse(in_path)
    return tree

def findNodes(tree, path):
    return tree.findall(path)

def getNodes(nodes):
    nodelist = []
    for node in nodes:
        nodelist.append(node.attrib["path"])
    return nodelist

def getProjectList(lw_nodeList, hw_nodeList, targetType):
    if(targetType == "hw"):
        return list(set(hw_nodeList).difference(set(lw_nodeList)))
    elif(targetType == "lw"):
        return list(set(lw_nodeList).difference(set(hw_nodeList)))
    else:
        return list(set(lw_nodeList).intersection(set(hw_nodeList)))

def move_dir(nodeList, source_dir, target_dir):
    nodeList = sorted(nodeList)
    for item in nodeList:
        gitListPath = os.path.join(source_dir, item) 
        if (not os.path.exists(gitListPath)):
            continue

        itemdir = os.path.join(target_dir, item)
        itemparentdir = os.path.split(itemdir)[0]
        cmd = "mkdir -p " + itemparentdir
        os.system(cmd)
        
        
        cmd = "mv " + gitListPath +" " + itemparentdir
        os.system(cmd)
        
        
def main(my_argv, source_dir, target_dir,lwXmlPath,hwXmlPath):
    if(not os.path.exists(lwXmlPath) or not os.path.exists(hwXmlPath)):
        print("lw_manifest or hw_manifest not exist in the dir version_build")
        return -1

    lw_tree = readXml(lwXmlPath)
    hw_tree = readXml(hwXmlPath)

    lw_nodes = findNodes(lw_tree, "project")
    hw_nodes = findNodes(hw_tree, "project")

    lw_nodeList = getNodes(lw_nodes)
    hw_nodeList = getNodes(hw_nodes)

    lwList = getProjectList(lw_nodeList, hw_nodeList, "lw")
    hwList = getProjectList(lw_nodeList, hw_nodeList, "hw")
    
    if(my_argv == 'lwrom'):
        move_dir(lwList, source_dir, target_dir)
    else:
        move_dir(hwList, source_dir, target_dir)
    
    return 0


if __name__ == "__main__":
    if(len(sys.argv) != 2):
        print("usage: mv_code_dir.py hwrom \n       mv_code_dir.py lwrom ")
        sys.exit(-1)
    my_argv = sys.argv[1]
    lwXmlPath = "BalongV9R1SFT_lw_manifest.xml"
    hwXmlPath = "BalongV9R1SFT_nlw_manifest.xml"
	
    #source_dir = "./../../../../"
    #target_dir = "./../../../../../code_back"
	
    source_dir =  "./../../../../../code_back"
    target_dir = "./../../../../"
    sys.exit(main(my_argv, source_dir, target_dir,lwXmlPath,hwXmlPath))

