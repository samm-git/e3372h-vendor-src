 /*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _NV_CTRL_H_
#define _NV_CTRL_H_


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -save -e537*/
#include "nv_comm.h"
#include "nv_file.h"
/*lint -restore +e537*/

typedef enum
{
    NV_FILE_STOR_FS     = 0,
    NV_FILE_STOR_NON_FS,
    NV_FILE_STOR_BUTT
}NV_FILE_STOR_TYPE_ENUM;

struct nv_file_p
{
    NV_FILE_STOR_TYPE_ENUM stor_type;
    void* fd;
};


typedef FILE* (*file_open)(const s8 * path,const s8* mode);
typedef s32  (*file_close)(FILE* fp);
typedef s32  (*file_read)(u8* ptr,u32 size,u32 count,FILE* fp);
typedef s32  (*file_write)(u8* ptr,u32 size,u32 count,FILE* fp);
typedef s32  (*file_remove)(const s8* path);
typedef s32  (*file_seek)(FILE* fp,s32 offset,s32 whence);
typedef s32  (*file_ftell)(FILE* fp);
typedef s32  (*file_access)(const s8* path,s32 mode);
typedef s32  (*file_mkdir)(const s8* path);

struct file_ops_table_stru
{
    file_open   fo;
    file_read   fr;
    file_write  fw;
    file_close  fc;
    file_remove frm;
    file_seek   fs;
    file_ftell  ff;
    file_access fa;
    file_mkdir  fm;
};


u32 nv_file_init(void);
FILE* nv_file_open(const s8 * path,const s8* mode);
s32 nv_file_read(u8 * ptr, u32 size, u32 count, FILE * fp);
s32 nv_file_write(u8 * ptr, u32 size, u32 count, FILE * fp);
s32 nv_file_close(FILE * fp);
s32 nv_file_seek(FILE * fp, s32 offset, s32 whence);
s32 nv_file_remove(const s8 * path);
s32 nv_file_ftell(FILE * fp);
s32 nv_file_access(const s8* path,s32 mode);


s32 nv_file_copy(s8 * dst_path, s8 * src_path, bool path);
u32 nv_copy_img2backup(void);
s32 nv_modify_upgrade_flag(bool flag);
bool nv_get_upgrade_flag(void);

/*sc file*/
#define SC_FILE_NUM    12

#define SC_FILE_EXIST_MAGIC    0x37ab65cd
struct sc_file_info
{
    unsigned int magic_num;   /*0x37ab65cd*/
    int len;
    int off;
    char  path[128];
};

#define SC_FILE_PACKET_MAGIC     0xdeacb589
struct sc_file_packet_info_struct
{
    unsigned int magic_num;   /*0xdeacb589*/
    int total_len;
    struct sc_file_info file_info[12];
};


#define SC_PACKET_TEMP_FILE       "/modem_log/sc_packet.bin"
#define SC_FILE_BACKUP_FLAG       "/modem_log/sc_backup.flag"
#define SC_BACKUP_SEC_NAME        "block2mtd: /dev/block/mmcblk0p7"
#define SC_BACKUP_SEC_OFFSET      0x280000   /*出厂分区 2.5M偏移处*/

int sc_file_backup(void);
int sc_file_restore(void);
bool get_sc_file_back_state(void);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*_NV_CTRL_H_*/


