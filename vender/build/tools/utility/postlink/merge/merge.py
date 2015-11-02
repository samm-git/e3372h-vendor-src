#!/usr/bin/env python
# -*- coding: utf-8 -*-
# created by Zhang Jingyuan(z00227143), 11/23/2012
#
# 合并Ram初始化和BootRom文件，为USB自举多台加载使用

import os
import sys
import struct

# 返回值
OK = 0
ERROR = 1

# 一次读取的字节数
READ_BUFFER_SIZE = 10*1024

# 文件传输类型：RAM初始化文件
LOAD_RAM = 0x01

# 文件传输类型：USB加载文件
LOAD_USB = 0x02

## 镜像头文件结构
##/* 下载文件结构 */
##typedef struct
##{
##    UINT8 ucType;           /* 下载文件类型 */
##    UINT32 ulLength;         /* 下载文件长度 */
##    UINT32 ulAddress;        /* 下载文件写入地址 */
##    UINT32 ulOffset;         /* 下载文件在Image文件中的偏移量 */
##} DOWNLOAD_FILE_ST;
##
##/* 镜像文件头结构 */
##typedef struct
##{
##    UINT16 usCRC16;         /* 镜像文件CRC16校验值 */
##    UINT8 ucFileNum;        /* 下载文件个数 */
##    UINT8 ucVersion[32];    /* 镜像文件版本信息 */
##    DOWNLOAD_FILE_ST stDownloadFile[3];
##} IMAGE_FILE_HEAD_ST;
IMAGE_FILE_HEAD_FORMAT = "HB32sxBxIIIBxIIIBxIII"


def printusage():
    print("\nusage:")
    print("merge.py <raminit file> <raminit entry addr> <usbloader file> <usbloader entry addr> <outfile> [kernel]")
    print("\n")


def main():
    if len(sys.argv) < 6:
        printusage()
        sys.exit(ERROR)

    pFileRam = sys.argv[1]
    ulDestAddrRamInit = long(sys.argv[2],16)
    pFileUsb = sys.argv[3]
    ulDestAddrUsbLoader = long(sys.argv[4],16)
    pFileOut = sys.argv[5]

    print("%s and %s will be merged to %s"%(pFileRam, pFileUsb, pFileOut))


    usCRC16 = 0     # 镜像文件CRC16校验值
    ucFileNum = 0   # 下载文件个数
    ucVersion = '\0'   # 镜像文件版本信息

    # 下载文件结构数组
    # [*][0] - UINT8: 下载文件类型
    # [*][1] - UINT32:下载文件长度
    # [*][2] - UINT32:下载文件写入地址
    # [*][3] - UINT32:下载文件在Image文件中的偏移量
    myList = [[0] * 4 for i in range(3)]

    stImageFileHead = struct.pack(IMAGE_FILE_HEAD_FORMAT,
                                  usCRC16,
                                  ucFileNum,
                                  ucVersion,
                                  myList[0][0],
                                  myList[0][1],
                                  myList[0][2],
                                  myList[0][3],
                                  myList[1][0],
                                  myList[1][1],
                                  myList[1][2],
                                  myList[1][3],
                                  myList[2][0],
                                  myList[2][1],
                                  myList[2][2],
                                  myList[2][3])

    # 镜像在merge后文件中的偏移
    ulOffset = struct.calcsize(IMAGE_FILE_HEAD_FORMAT)
    
    FileOut = open(pFileOut,'wb')

    # 先写入文件头，后续再更新
    FileOut.write(stImageFileHead)

    print("write dummy head to merged file.")

    # 先拷贝Ram初始化程序
    ulFileSize = os.path.getsize(pFileRam)  # 文件大小
    FileRam = open(pFileRam,'rb')
    while True:
        chunk = FileRam.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        FileOut.write(chunk)
    FileRam.close()

    print("write %s to merged %s succeed."%(pFileRam, pFileOut))

    myList[0][0] = LOAD_RAM
    myList[0][1] = ulFileSize
    myList[0][2] = ulDestAddrRamInit
    myList[0][3] = ulOffset
    ucFileNum += 1

    ulOffset += ulFileSize

    # 再拷贝USB加载程序，即BootRom
    ulFileSize = os.path.getsize(pFileUsb)
    FileUsb = open(pFileUsb,'rb')
    while True:
        chunk = FileUsb.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        FileOut.write(chunk)
    FileUsb.close()

    print("write %s to merged %s succeed."%(pFileUsb, pFileOut))

    if len(sys.argv) > 6:
        if os.path.exists(sys.argv[6]):
            ulFileSize += os.path.getsize(sys.argv[6])

            FileKernel = open(sys.argv[6], 'rb')
            KernelHead = struct.unpack("7s", FileKernel.read(7))
            if KernelHead != "ANDROID":
                ulFileSize -= 128
                FileKernel.seek(128, os.SEEK_SET)
                KernelHead = struct.unpack("7s", FileKernel.read(7))[0]
                if KernelHead != "ANDROID":
                    print("image error, no 'ANDROID' flag")
                    sys.exit(ERROR)
                else:
                    FileKernel.seek(128, os.SEEK_SET)
            else:
                FileKernel.seek(0, os.SEEK_SET)
                
            while True:
                chunk = FileKernel.read(READ_BUFFER_SIZE)
                if not chunk:
                    break
                FileOut.write(chunk)
            FileKernel.close()
            print("write %s to merged %s succeed."%(sys.argv[6], pFileOut))
            
        else:
            print("%s not exists"%(sys.argv[2]))
            sys.exit(ERROR)
    else:
        print("no kernel image, skip...")

    myList[1][0] = LOAD_USB
    myList[1][1] = ulFileSize
    myList[1][2] = ulDestAddrUsbLoader
    myList[1][3] = ulOffset
    ucFileNum +=1

    # 暂时不用CRC，由USB保证传输OK
    usCRC16 = 0
    # 暂不使用，镜像文件版本信息
    ucVersion = '\0'

    # 更新文件头
    stImageFileHead = struct.pack(IMAGE_FILE_HEAD_FORMAT,
                                  usCRC16,
                                  ucFileNum,
                                  ucVersion,
                                  myList[0][0],
                                  myList[0][1],
                                  myList[0][2],
                                  myList[0][3],
                                  myList[1][0],
                                  myList[1][1],
                                  myList[1][2],
                                  myList[1][3],
                                  myList[2][0],
                                  myList[2][1],
                                  myList[2][2],
                                  myList[2][3])
    
    FileOut.seek(0)
    FileOut.write(stImageFileHead)

    FileOut.close()

    print("updated file header succeed.")

    sys.exit(OK)    

if __name__ == "__main__":
    main()
