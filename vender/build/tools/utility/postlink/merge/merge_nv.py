# created by Zhang Jingyuan(z00227143), 2013/02/18
#
# merge NV files

import os,sys,struct

# Return Value
OK = 0
ERROR = 1

# Read Buffer
READ_BUFFER_SIZE = 10*1024

# Image Head
##struct
##{
##  u32 status;
##  u32 hnv_file_offset;
##  u32 xml_file_offset;
##  u32 hnv_file_length;
##  u32 xml_file_length;
##};
IMAGE_FILE_HEAD_FORMAT = "IIIII"


def printusage():
    print("\nusage:")
    print("merge_nv.py <hnv_file> <xml_file> <outfile>\n")
    print("\n")


def main():
    if len(sys.argv) < 4:
        print("error parameters!")
        printusage()
        sys.exit(ERROR)

    image_status = 0
    hnv_file_offset = 0
    xml_file_offset = 0
    hnv_file_length = 0
    xml_file_length = 0
    
    if os.path.exists(sys.argv[1]):
        hnv_file_offset = struct.calcsize(IMAGE_FILE_HEAD_FORMAT)
        hnv_file_length = os.path.getsize(sys.argv[1])
    else:
        print("file not exits: %s"%(sys.argv[1]))
        sys.exit(ERROR)

    if os.path.exists(sys.argv[2]):
        xml_file_offset = hnv_file_offset + hnv_file_length
        xml_file_length = os.path.getsize(sys.argv[2])
    else:
        print("file not exits: %s"%(sys.argv[2]))
        sys.exit(ERROR)
    
    pFileOut = sys.argv[3]

    FileOut = open(pFileOut, 'wb')

    # write head to outfile
    FileOut.write(struct.pack(IMAGE_FILE_HEAD_FORMAT,
                              image_status,
                              hnv_file_offset,
                              xml_file_offset,
                              hnv_file_length,
                              xml_file_length))

    # copy hnv_file to outfile
    FileIn = open(sys.argv[1], 'rb')
    while True:
        chunk = FileIn.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        FileOut.write(chunk)
    FileIn.close()
    print("write %s to merged %s succeed."%(sys.argv[1],pFileOut))

    # copy xml_file to outfile
    FileIn = open(sys.argv[2], 'rb')
    while True:
        chunk = FileIn.read(READ_BUFFER_SIZE)
        if not chunk:
            break
        FileOut.write(chunk)
    FileIn.close()
    print("write %s to merged %s succeed."%(sys.argv[2],pFileOut))

    FileOut.close()
    print("merge nv files succeed.")

    sys.exit(OK)


if __name__ == "__main__":
    main()
