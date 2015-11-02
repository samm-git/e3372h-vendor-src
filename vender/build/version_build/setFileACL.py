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
    

def setACL(nodeList, source_dir, aclUser):
    for item in nodeList:
        path = os.path.join(source_dir, item)
        cmd = "hi_setfacl -R -m u:"+ aclUser +":--x " + path
        os.system(cmd)

def setDefaultACL(source_dir, aclUser):
    for item in os.listdir(source_dir):
        if(item == ".repo"):
            pass
        else:
            path = os.path.join(source_dir, item)
            cmd = "hi_setfacl -R -m u:"+ aclUser +":rwx " + path
            os.system(cmd)

def removeACL(source_dir):
    cmd = "hi_setfacl -R -b " + source_dir
    os.system(cmd)

def checkAclUserWhetherExist(aclUser):
    cmd = '''cat /etc/passwd | awk '{FS=":"} {print $1}' | grep ''' + aclUser
    users = os.popen(cmd).readlines()
    if(len(users) == 0):
		print("user %s not exist"%aclUser)
		return False
    else:
        user = users[0].strip()
        if(user == aclUser):
            return True
        else:
			print("user %s not exist"%aclUser)
			return False

def main(my_argv, aclUser, source_dir, lwXmlPath, hwXmlPath):
    if(not os.path.exists(lwXmlPath) or not os.path.exists(hwXmlPath)):
        print("lw_manifest or hw_manifest not exist in the dir version_build")
        return -1
    if(checkAclUserWhetherExist(aclUser) == False):
        return 0
    lw_tree = readXml(lwXmlPath)
    hw_tree = readXml(hwXmlPath)

    lw_nodes = findNodes(lw_tree, "project")
    hw_nodes = findNodes(hw_tree, "project")

    lw_nodeList = getNodes(lw_nodes)
    hw_nodeList = getNodes(hw_nodes)

    lwList = getProjectList(lw_nodeList, hw_nodeList, "lw")
    hwList = getProjectList(lw_nodeList, hw_nodeList, "hw")
	
    setDefaultACL(source_dir, aclUser)
    if(my_argv == "hwrom"):
        setACL(lwList, source_dir, aclUser)
    elif(my_argv == "lwrom"):
        setACL(hwList, source_dir, aclUser)
    else:
        print("error, usage: setFileACL.py xxx\n")
        return -1

    return 0


if __name__ == "__main__":
    if(len(sys.argv) != 2):
        print("error, usage: setFileACL.py xxx")
        sys.exit(-1)
    my_argv = sys.argv[1]
    lwXmlPath = "BalongV9R1SFT_lw_manifest.xml"
    hwXmlPath = "BalongV9R1SFT_nlw_manifest.xml"
    productDir = "./../../../../"
    aclUser = "hibuild1"
    sys.exit(main(my_argv, aclUser, productDir, lwXmlPath, hwXmlPath))

