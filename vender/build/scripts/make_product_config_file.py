#!/usr/bin/python
# Filename: using_file.py

import string
import os
import sys
from hashlib import md5
# Description: 
# Author y00184236/2012-03-22 @org
def add_product_config_h_file_head(file):
    str = '''#if !defined(__PRODUCT_CONFIG_H__)\n#define __PRODUCT_CONFIG_H__\n\n'''
    file.write(str)
    
def add_product_config_h_file_tail(file):
    str = '''#endif /*__PRODUCT_CONFIG_H__*/ \n'''
    file.write(str)
#BEGIN DTS2013061702233 daizhicheng 2013-06-17 added
def read_argv_from_file(file_obuild_cmd):
	my_argv = []
	if os.path.exists(file_obuild_cmd):
		fil = open(file_obuild_cmd, 'r')
		for v in fil.readlines() :
			my_argv.append(string.rstrip(v,'\n'))
		fil.close()
	return my_argv

#include "mbb_config.h" in product_config.h
def add_product_config_h_mbb_file(file):
    file_obuild_cmd = 'obuild.cmd'
    my_argv = read_argv_from_file(file_obuild_cmd)

    # scan parameters to dict
    params_dict = {}
    params_string = ""
    for p in my_argv:
        params_string += " " + p
        if string.find(p, '=') != -1 :
            subs = string.split(p,'=')
            params_dict[subs[0]]=subs[1]
        else:
            params_dict[p]=""
            
    if params_dict.has_key('MBB_DT'):
        os.environ['MBB_DT_USBLOADER'] = 'FEATURE_ON'
        mbb_str = '''#ifndef MBB_DT_USBLOADER\n#define MBB_DT_USBLOADER FEATURE_ON\n#endif\n\n'''
        print "[ NOTE ] MBB_DT_USBLOADER = FEATURE_ON"
    else:
        mbb_str = '''#ifndef MBB_DT_USBLOADER\n#define MBB_DT_USBLOADER FEATURE_OFF\n#endif\n\n'''
        os.environ['MBB_DT_USBLOADER'] = 'FEATURE_OFF'
        print "[ NOTE ] MBB_DT_USBLOADER = FEATURE_OFF"
    file.write(mbb_str)
	
    if params_dict.has_key('NV_BURN'):
        os.environ['MBB_FACTORY'] = 'FEATURE_ON'
        mbb_str = '''#ifndef MBB_FACTORY\n#define MBB_FACTORY FEATURE_ON\n#endif\n\n'''
        print "[ NOTE ] MBB_FACTORY = FEATURE_ON"
    else:
        mbb_str = '''#ifndef MBB_FACTORY\n#define MBB_FACTORY FEATURE_OFF\n#endif\n\n'''
        os.environ['MBB_FACTORY'] = 'FEATURE_OFF'
        print "[ NOTE ] MBB_FACTORY = FEATURE_OFF"
    file.write(mbb_str)
    mbb_str = '''#include "mbb_config.h" \n#include "mbb_adapt.h" \n'''
    file.write(mbb_str)
#END DTS2013061702233 daizhicheng 2013-06-17 added

# Description: str1 = str2  ==>  #define str1 1 or #define str1 str2
# Author y00184236/2012-03-221 @org
def trans_string(str1, str2):
    if str2 == 'YES' :
#        return '#define ' + str1 + ' 1\n'
        return '#define ' + str1 + ' \n'
    elif str2 == 'NO':
        return ''
    elif str2 != '' and str2 != ' ' and str2 != '\n':
        return '#define ' + str1 + ' '+ str2 + ' \n'

# Description: 
# Author y00184236/2012-03-21 @org
def get_string_from_idx(str, idx=0):
    result =''
    result = str[idx:]
    return result

# Description: 
# Author y00184236/2012-03-21 @org
def remove_invalid_ch(str):
    result =''
    str = string.strip(str, ' ')
    str = string.strip(str, '\n')
    str = string.strip(str, '\r')
    str = string.strip(str, '\*')
    str = string.strip(str, '\/')
    result = str
    return result

# Description: 
# Author y00184236/2012-03-22 @org
def format_singlecore_string(core_str, valid_str):
    head_str = '#ifndef ' + core_str + '\n'
    mid_str  = valid_str
#   tail_str = '#endif // ' + core_str + '\n\n'
    tail_str = '#endif ' + '\n\n'    
    result = head_str + mid_str + tail_str
    return result
	
# Description: 
# Author y00184236/2012-03-22 @org
def format_multicore_string(core_str, valid_str):
    head_str = '#ifdef ' + core_str + '\n'
    mid_str  = valid_str
    tail_str = '#endif ' + '\n\n'
#   tail_str = '#endif // ' + core_str + '\n\n'    
    result = head_str + mid_str + tail_str
    return result

# Description: 
# Author y00184236/2012-03-22 @org
def get_multicore_valid_string(str1, str2):
    remove_invalid_ch(str1)
    remove_invalid_ch(str2)
    if str2 == '':
        return str1
    else:
        return str1 + str2 + '\n'

# Description: 
# Author y00184236/2012-03-22 @org
def process_string_start_with_cfg(str):
    fields = string.split(str, '=')
    #print fields
    left_slice = remove_invalid_ch(fields[0])
    left_slice = get_string_from_idx(left_slice, 4)
    right_slice = remove_invalid_ch(fields[1])
    #print left_slice
    #print right_slice
    result = trans_string(left_slice, right_slice)
    result = format_singlecore_string(left_slice, result)
    return result

# Description: config multicore 
# original string: x =A;B;C;D 
# transferred string: define x 1 / define x 1()
# Author y00184236/2012-03-21 @org
def process_string_start_with_env(str):
    str = remove_invalid_ch(str)
    
    mcore_result = ''
    acore_result = ''
    bootrom_result = ''
    bootloader_result = ''
    
    fields = string.split(str, '=')
    left_str = remove_invalid_ch(fields[0])
    left_str = get_string_from_idx(left_str, 4)
    right_str = remove_invalid_ch(fields[1])
    
    fields = string.split(fields[1], ';')
    mcore_str = remove_invalid_ch(fields[0])
    acore_str = remove_invalid_ch(fields[1])
    bootrom_str = remove_invalid_ch(fields[2])
    bootloader_str = remove_invalid_ch(fields[3])
    
    mcore_result = mcore_result + trans_string(left_str, mcore_str)
    #print 'left_str  = ' , left_str, 'mcore_result = ', mcore_result
    acore_result = acore_result + trans_string(left_str, acore_str)
    bootrom_result = bootrom_result + trans_string(left_str, bootrom_str)
    bootloader_result = bootloader_result + trans_string(left_str, bootloader_str)
    
    mcore_result = remove_invalid_ch(mcore_result)
    acore_result = remove_invalid_ch(acore_result)
    bootrom_result = remove_invalid_ch(bootrom_result)
    bootloader_result = remove_invalid_ch(bootloader_result)
    
    result = (mcore_result, acore_result, bootrom_result, bootloader_result)
    return result

# Description: generate product_config.h and product.config.mk
# Input :
# Output:
# Author y00184236/2012-03-24 @org
def gen_config_flie(org_file, gen_file):
    
    #open file 
    config_file = file(org_file, 'r')
    product_config_file = file(gen_file, 'a')

    mcore_result =''
    acore_result =''
    bootrom_result =''
    bootloader_result =''
    mk_line = ''
    line = ''
    
    if string.find(gen_file, '.h') > 0:
        add_product_config_h_file_head(product_config_file)

    while True:
        line = config_file.readline()
        if len(line) == 0: # Zero length indicates EOF
            break
        line = remove_invalid_ch(line)
        #print line

        if string.find(gen_file, '.h') > 0:
            if line.startswith('CFG_'):
                result = process_string_start_with_cfg(line)        
                if result != '':
                    product_config_file.write(result)
            elif line.startswith('ENV_'):
                multicore_result = process_string_start_with_env(line)
                #print multicore_result
                mcore_result = get_multicore_valid_string(mcore_result, multicore_result[0])
                #print 'mcore_result = ', mcore_result
                acore_result = get_multicore_valid_string(acore_result, multicore_result[1])
                bootrom_result = get_multicore_valid_string(bootrom_result, multicore_result[2])
                bootloader_result = get_multicore_valid_string(bootloader_result, multicore_result[3])
        
        elif string.find(gen_file, '.mk') > 0:
            if line.split() and not line.startswith('#'):
                mk_line = string.replace(line, '=', ':=')
                mk_line = remove_invalid_ch(mk_line)
                product_config_file.write(mk_line + '\n')
    
    if string.find(gen_file, '.h') > 0:
        #print 'multicore_result[0]=', multicore_result[0]
        #print 'multicore_result[1]=', multicore_result[1]
        #print 'multicore_result[2]=', multicore_result[2]
        #print 'multicore_result[3]=', multicore_result[3]
        #print 'mcore_result=' , mcore_result
        #print 'acore_result=' , acore_result
        #print 'bootrom_result=' ,  bootrom_result
        #print 'bootloader_result=', bootloader_result
        result1 = format_multicore_string('OBC_IMAGE_MODEM', mcore_result)
        result2 = format_multicore_string('OBC_IMAGE_APP', acore_result)
        result3 = format_multicore_string('OBC_IMAGE_BOOTROM', bootrom_result)
        result4 = format_multicore_string('OBC_IMAGE_BOOTLOADER', bootloader_result)
        product_config_file.write(result1)
        product_config_file.write(result2)
        product_config_file.write(result3)
        product_config_file.write(result4)
        #print result1
        #print result2
        #print result3
        #print result4
        #BEGIN DTS2013061702233 daizhicheng 2013-06-17 added
        #include "mbb_config.h" in product_config.h
        add_product_config_h_mbb_file(product_config_file)
        #END DTS2013061702233 daizhicheng 2013-06-17 added
        add_product_config_h_file_tail(product_config_file)
    
    config_file.close() # close the file 
    product_config_file.close() # close the file 

def get_file_md5(file_name):
    m = md5()
    a_file = open(file_name, 'rb')
    m.update(a_file.read())
    a_file.close()
    return m.hexdigest()

def split_str_with_separator(str, sep):
    str = remove_invalid_ch(str)
    fields =''
    
    if string.find(str, sep) > 0:
        fields = string.split(str, sep)
        fields[0] = remove_invalid_ch(fields[0])
        fields[1] = remove_invalid_ch(fields[1])
    return fields
    #print fields
    result = (fields[0], fields[1])
    #print result
    return result

#format: 'MD5: xxxx'
def gen_product_config_file(org_file, gen_file):
    org_md5 = get_file_md5(org_file)
    local_file = ''
    stored_md5 =''
    type =''
    if string.find(gen_file, '.h') > -1:
        type_head = r'/*'
        type_tail = r'*/'
        first_line = type_head + ' MD5: ' + org_md5 + type_tail + '\n'
    elif string.find(gen_file, '.mk') > -1:
        type = r'#'
        first_line = type + ' MD5: ' + org_md5 + '\n'
    else:
        print r'==> Error:"', gen_file, r'"Invalid parameter!'
        return
    
    #first_line = type + ' MD5: ' + org_md5 + '\n'
    #print first_line
    if not os.path.isfile(gen_file):
        local_file = file(gen_file, 'w')
        local_file.write(first_line)
        local_file.close()
        #print 'enterance'
        #print org_file
        #print gen_file
        gen_config_flie(org_file, gen_file)
    else:
        local_file = file(gen_file, 'r')
        line = local_file.readline() #read first line from file
        line = remove_invalid_ch(line)
        local_file.close()
        #print line
        # deleted by xuchao, do not use md5
        #if string.find(line, ':') > -1 :
        #stored_md5 = split_str_with_separator(line, ':')[1]
        #if stored_md5 != org_md5:
        local_file = file(gen_file, 'w')
        local_file.write(first_line);
        local_file.close()
        gen_config_flie(org_file, gen_file)
        #else:
         #   print os.path.basename(org_file),'file has no change, no need to generate', os.path.basename(gen_file)

