cmd_libbb/ptr_to_globals.o := arm-none-linux-gnueabi-gcc -Wp,-MD,libbb/.ptr_to_globals.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.21.1)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wno-format-security -Wdeclaration-after-statement  -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -fno-unwind-tables -fno-asynchronous-unwind-tables -Os     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ptr_to_globals)"  -D"KBUILD_MODNAME=KBUILD_STR(ptr_to_globals)" -c -o libbb/ptr_to_globals.o libbb/ptr_to_globals.c

deps_libbb/ptr_to_globals.o := \
  libbb/ptr_to_globals.c \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/errno.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/features.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/predefs.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/cdefs.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/wordsize.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/gnu/stubs.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/errno.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/linux/errno.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/errno.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/asm-generic/errno.h \
  /opt/CodeSourcery/Sourcery_G++_Lite/bin/../arm-none-linux-gnueabi/libc/usr/include/asm-generic/errno-base.h \

libbb/ptr_to_globals.o: $(deps_libbb/ptr_to_globals.o)

$(deps_libbb/ptr_to_globals.o):
