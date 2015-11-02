#!/usr/bin/env python
# created by Zhang Jingyuan(z00227143), 11/23/2012
#
# merge DSP images

import os,sys

# Read Buffer
READ_BUFFER_SIZE = 10*1024

# Ordered BBE16 Images
BBEFileList = [
    "ten_pub_dtcm.bin",
    "ten_pub_itcm.bin",
    "ten_lte_dtcm.bin",
    "ten_lte_itcm.bin",
    "ten_tds_dtcm.bin",
    "ten_tds_itcm.bin",
#	"ten_uni_dtcm.bin",
#	"ten_uni_itcm.bin",
	"tds_dsp_ddr_data.bin",
    ]

# Ordered HiFi Images
HiFiFileList = [
    ]


def printusage():
    print("\nusage:")
    print("mergeDsp.py <directory BBE> <directory HiFi> <outfile>\n")
    print("  <directory BBE> -- the directory containing BBE16 images")
    print("  <directory HiFi> -- the directory containing HiFi2 images")
    print("\n")

def merge_for_file(argv):
    pFileOut = argv[3]
    FileOut = open(pFileOut,'wb')

    for i in range(1, len(argv)-1):
        FileIn = open(argv[i], 'rb')
        while True:
            chunk = FileIn.read(READ_BUFFER_SIZE)
            if not chunk:                
                break
            FileOut.write(chunk)
        FileIn.close()

    FileOut.close()



def merge_for_directory(argv):
    pFileOut = argv[3]
    FileOut = open(pFileOut,'wb')

    # merge BBE16 images
    for i in range(len(BBEFileList)):
        pFilePath = os.path.join(argv[1], BBEFileList[i])
        if not os.path.exists(pFilePath):
            print("error! file not exists: %s"%(pFilePath))
            printusage()
            sys.exit(ERROR)

        FileIn = open(pFilePath,'rb')

        while True:
            chunk = FileIn.read(READ_BUFFER_SIZE)
            if not chunk:
                break
            FileOut.write(chunk)

        FileIn.close()

    # merge HiFi2 image
    for i in range(len(HiFiFileList)):
        pFilePath = os.path.join(argv[2], HiFiFileList[i])
        if not os.path.exists(pFilePath):
            print("error! file not exists: %s"%(pFilePath))
            printusage()
            sys.exit(ERROR)

        FileIn = open(pFilePath,'rb')

        while True:
            chunk = FileIn.read(READ_BUFFER_SIZE)
            if not chunk:
                break
            FileOut.write(chunk)

        FileIn.close()

    FileOut.close()
    print("merge DSP images OK, output file: %s"%(pFileOut))


def main():
    if len(sys.argv) < 4:
        print("error parameters!")
        printusage()
        sys.exit(-1)

    if os.path.isfile(sys.argv[1]):
        merge_for_file(sys.argv)
    elif os.path.isdir(sys.argv[1]):
        merge_for_directory(sys.argv)
    else:
        print("error path: %s"%(sys.argv[1]))
        sys.exit(-1)


if __name__ == "__main__":
    main()
