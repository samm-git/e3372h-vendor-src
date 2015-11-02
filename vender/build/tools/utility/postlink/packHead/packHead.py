#!/usr/bin/env python
#created by Zhang Jingyuan(z00227143), 2013/04/20
#
#add head for all image

import os,sys,struct,gzip


###########  M3Boot  ###########
#45 interrupt vectors for p531, 144 interrupt vectors for v7r2
M3Boot_IMAGE_LENTH_OFFSET_P531 = 45 * 4
M3Boot_IMAGE_LENTH_OFFSET_V7R2 = 144 * 4
#4 bytes length, 260 bytes root ca and 32 bytes version
M3Boot_COPYRIGHT_OFFSET_P531 = M3Boot_IMAGE_LENTH_OFFSET_P531 + 4 + 260 + 32
M3Boot_COPYRIGHT_OFFSET_V7R2 = M3Boot_IMAGE_LENTH_OFFSET_V7R2 + 4 + 260 + 32

###########  FastBoot  ###########
FASTBOOT_IMAGE_LENGTH_OFFSET = 11 * 4

###########  PackHead  ###########
IMAGE_NAME_SIZE = 16
IMAGE_VERSION_LEN = 32
IMAGE_HEAD_SIZE = 128

READ_BUFFER_SIZE = 10*1024

#level is an integer from 1 to 9 controlling the level of compression;
#1 is fastest and produces the least compression,
#9 is slowest and produces the most.
COMPRESS_LEVEL = 9


def print_usage():
    print(
"""
usage:
    1.packHead.py <file type> <in file> <out file> <load addr> [DEFLATE]
    <file type>:
        m3boot
        fastboot
        m3image
        kernel
        vxworks
        dsp
        nvdload
        ...
""")

#deflate image
def deflate(pFileIn, offset):
    pFileGz = pFileIn + '.gz'
    FileIn = open(pFileIn, 'rb')
    FileGz = gzip.open(pFileGz, 'wb', COMPRESS_LEVEL)

    FileIn.seek(offset, os.SEEK_SET)
    
    while True:
        chunk = FileIn.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        FileGz.write(chunk)

    FileIn.close()
    FileGz.close()
    
    print('deflate success, output file: %s'%(pFileGz))

#set m3boot image length
#
#we will set image length in different offset for p531 and v7r2
#according to the copyright offset
def image_m3boot():
    if len(sys.argv) < 5:
        print("error parameters")
        print_usage()
        sys.exit(-1)

    #v7r2 or p531
    filein = open(sys.argv[2], 'rb')
    M3Boot_IMAGE_LENTH_OFFSET = 0
    filein.seek(M3Boot_COPYRIGHT_OFFSET_P531, os.SEEK_SET)
    copyrightinfo = struct.unpack("I", filein.read(4))[0]
    if copyrightinfo == 0x79706F43:
        M3Boot_IMAGE_LENTH_OFFSET = M3Boot_IMAGE_LENTH_OFFSET_P531
    else:
        filein.seek(M3Boot_COPYRIGHT_OFFSET_V7R2, os.SEEK_SET)
        copyrightinfo = struct.unpack("I", filein.read(4))[0]
        if copyrightinfo == 0x79706F43:
            M3Boot_IMAGE_LENTH_OFFSET = M3Boot_IMAGE_LENTH_OFFSET_V7R2
    
    if M3Boot_IMAGE_LENTH_OFFSET == 0:
        print("error image, copyright not found")
        sys.exit(-2)
        
    #set image length
    fileout = open(sys.argv[3], 'wb')
    filein.seek(0, os.SEEK_SET)
    while True:
        chunk = filein.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        fileout.write(chunk)

    image_length = os.path.getsize(sys.argv[2])
    fileout.seek(M3Boot_IMAGE_LENTH_OFFSET, os.SEEK_SET)
    fileout.write(struct.pack("I", image_length))

    filein.close()
    fileout.close()
    
    print("succeed to handle m3boot")


#set fastboot image length
def image_fastboot():
    if len(sys.argv) < 5:
        print("error parameters")
        print_usage()
        sys.exit(-1)

    filein = open(sys.argv[2], 'rb')
    fileout = open(sys.argv[3], 'wb')
    while True:
        chunk = filein.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        fileout.write(chunk)

    image_length = os.path.getsize(sys.argv[2])
    fileout.seek(FASTBOOT_IMAGE_LENGTH_OFFSET, os.SEEK_SET)
    fileout.write(struct.pack("I", image_length))

    filein.close()
    fileout.close()
    
    print("succeed to handle fastboot")


#pack head for other image
##pragma pack(1)
##/* 128 Bytes */
##typedef struct tagIMAGE_HEAD_S
##{
##    CHAR    aucMagicName[IMAGE_NAME_SIZE];
##    CHAR    aucVer[IMAGE_VERSION_LEN];
##    U32     ulVerIdx;
##
##    U32     ulImgLen;
##    U32     ulDestAddr; 
##    U32     ulCRC;
##    BOOL    bIsSecVer;
##    CHAR    aucReserved[128 - IMAGE_NAME_SIZE - IMAGE_VERSION_LEN - sizeof(BSP_U32) * 4 - sizeof(BSP_BOOL)];
##}IMAGE_HEAD_S;
##pragma pack()
def image_pack_head():
    if len(sys.argv) < 5:
        print("error parameters")
        print_usage()
        sys.exit(-1)

    is_image_packed = False

    IMAGE_HEAD_FORMAT = str(IMAGE_NAME_SIZE) + 's' \
                        + str(IMAGE_VERSION_LEN) + 'sIIIIII' \
                        + str(IMAGE_HEAD_SIZE-IMAGE_NAME_SIZE-IMAGE_VERSION_LEN-24) + 's'

    filein = open(sys.argv[2], 'rb')
    fileout = open(sys.argv[3], 'wb')

    image_name,image_version,image_index,image_length, \
    load_addr,image_crc,is_secver,is_image_compressed,reserved \
        = struct.unpack(IMAGE_HEAD_FORMAT,filein.read(IMAGE_HEAD_SIZE))

    if not cmp(image_name[0:len(sys.argv[1])],sys.argv[1].upper()):
        is_image_packed = True
        print("file: %s has packed head, will update..."%(sys.argv[2]))
    else:
        print("new file, pack head...")

    if len(sys.argv) >= 6 and not cmp(sys.argv[5].upper(),'DEFLATE'):
        if is_image_packed and is_image_compressed == 0:
            deflate(sys.argv[2], IMAGE_HEAD_SIZE)
        else:
            deflate(sys.argv[2], 0)
        is_image_compressed = 1
        filein.close()
        sys.argv[2] = sys.argv[2] + '.gz'
        filein = open(sys.argv[2], 'rb')
    else:
        is_image_compressed = 0

    image_name = sys.argv[1].upper()
    image_version = '\0'
    image_index = 0
    image_crc = 0
    is_secver = 0
    load_addr = int(sys.argv[4],16)
    image_length = os.path.getsize(sys.argv[2])
    reserved = '\0'

    if not is_image_packed:      
        filein.seek(0, os.SEEK_SET)
        
    fileout.write(struct.pack(IMAGE_HEAD_FORMAT,
                              image_name,
                              image_version,image_index,
                              image_length,load_addr,
                              image_crc,is_secver,
                              is_image_compressed,
                              reserved))

    while True:
        chunk = filein.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        fileout.write(chunk)

    filein.close()
    fileout.close()
    
    print("succeed to handle %s"%(sys.argv[1]))

def main():
    if len(sys.argv) < 2:
        print("error parameters")
        print_usage()
        sys.exit(-1)
        
    if not cmp(sys.argv[1], "m3boot"):
        image_m3boot()
    elif not cmp(sys.argv[1], "fastboot"):
        image_fastboot()
    else:
        image_pack_head() 
    
    sys.exit(0)


if __name__ == "__main__":
    main()    
