#!/usr/bin/python
#-*- coding: UTF-8 -*-

import  os
import  sys

def dsp_asc_to_bin(input_file_name, output_file_name):
    with open(input_file_name) as input_file_fd, open(output_file_name, mode='wb') as output_file_fd :
        
        for input_file_oneline in input_file_fd:
            
            input_file_oneline = input_file_oneline.strip()

            input_file_oneline_value = input_file_oneline[6:8] + input_file_oneline[4:6] + input_file_oneline[2:4] + input_file_oneline[0:2]

            input_file_oneline_bin = bytearray.fromhex(input_file_oneline_value)
            output_file_fd.write(input_file_oneline_bin)





if __name__ == '__main__':

    if sys.argv.__len__() < 3:
        print("please  inputname  outputname")
        
    dsp_asc_to_bin(sys.argv[1], sys.argv[2])

