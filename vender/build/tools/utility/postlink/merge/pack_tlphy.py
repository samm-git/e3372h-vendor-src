#!/usr/bin/env python
# created by y00187525, 08/02/2014
#
#pack_lphy.py for tlphy_onetrack


import os,sys,string
import shutil


def main():

	OBB_PRODUCT_DELIVERY_DIR = sys.argv[1]
	BALONG_TOPDIR = sys.argv[2]

	OBB_PRODUCT_LIB_DIR     =  OBB_PRODUCT_DELIVERY_DIR + "/lib"
	LDSP_OUTFILE_BIN_FILE   =  OBB_PRODUCT_LIB_DIR + "/lphy.bin"
	MERGE_TOOL_PATH         =  BALONG_TOPDIR + "/build/tools/utility/postlink/merge/merge_dsp.py"
	PACKHEAD_TOOL_PATH      =  BALONG_TOPDIR + "/build/tools/utility/postlink/packHead/packHead.py"
	TDS_DDR_BIN_PATH        =  BALONG_TOPDIR + "/modem/phy/lt"

	shutil.copyfile(TDS_DDR_BIN_PATH + "/tds_dsp_ddr_data.bin",OBB_PRODUCT_LIB_DIR + "/tds_dsp_ddr_data.bin")
	os.system('python ' + MERGE_TOOL_PATH + ' ' + OBB_PRODUCT_LIB_DIR + ' ' + OBB_PRODUCT_LIB_DIR + ' ' + LDSP_OUTFILE_BIN_FILE + '.forpack')
	os.system('python ' + PACKHEAD_TOOL_PATH + ' DSP ' + LDSP_OUTFILE_BIN_FILE + '.forpack ' + LDSP_OUTFILE_BIN_FILE + ' 0x0')


if __name__ == "__main__":
    main()
