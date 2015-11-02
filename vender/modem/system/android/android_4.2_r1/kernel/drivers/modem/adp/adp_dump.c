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


#include "drv_dump.h"
#include "bsp_dump.h"

#ifdef __cplusplus
extern "C"
{
#endif

BSP_VOID DRV_SYSTEM_ERROR(BSP_S32 modId, BSP_S32 arg1, BSP_S32 arg2, BSP_CHAR* arg3, BSP_S32 arg3Length)
{
    return system_error(modId, arg1, arg2, arg3, arg3Length);
}

BSP_S32 DRV_SAVE_REGISTER(BSP_S32 funcType, OM_SAVE_FUNC *pFunc)
{
    return BSP_OK;
}

BSP_U32  DRV_EXCH_MEM_MALLOC(BSP_U32 ulSize)
{
    char *buffer = NULL;
    u32 length = 0;

    if(BSP_OK != bsp_dump_get_buffer(DUMP_SAVE_MOD_OSA_APP, &buffer, &length))
    {
        return (BSP_U32)NULL;
    }
    else
    {
        return (BSP_U32)((ulSize > length) ? (NULL) : (buffer));
    }
}

BSP_S32 DRV_EXCH_FILE_SAVE(BSP_VOID *address, BSP_U32 length, BSP_U8 IsFileEnd, BSP_U8 type)
{
    return BSP_OK;
}

BSP_VOID DRV_EXCH_HOOK_FUNC_ADD(BSP_VOID)
{
    return;
}

BSP_VOID DRV_EXCH_HOOK_FUNC_DELETE(BSP_VOID)
{
    bsp_dump_trace_stop();
    return;
}

BSP_VOID DRV_STACK_HIGH_RECORD(BSP_VOID)
{
    return;
}

BSP_VOID DRV_VICINT_HOOK_ADD(pFUNCPTR p_Func)
{
    return;
}

BSP_VOID DRV_VICINT_HOOK_DEL(BSP_VOID)
{
    return;
}

BSP_VOID DRV_VICINT_IN_HOOK_ADD(pFUNCPTR p_Func)
{
    return;
}

BSP_VOID DRV_VICINT_IN_HOOK_DEL(BSP_VOID)
{
    return;
}

BSP_VOID DRV_VICINT_OUT_HOOK_ADD(pFUNCPTR p_Func)
{
    return;
}

BSP_VOID DRV_VICINT_OUT_HOOK_DEL(BSP_VOID)
{
    return;
}

BSP_VOID DRV_VICINT_LVLCHG_HOOK_ADD(pFUNCPTR p_Func)
{
    return;
}

BSP_VOID DRV_VICINT_LVLCHG_HOOK_DEL(BSP_VOID)
{
    return;
}

/*********************************** V7R2 ÐÂÔö **********************************/
BSP_S32 DRV_DUMP_SAVE_REGISTER_HOOK(DUMP_SAVE_MOD_ENUM ModId, DUMP_SAVE_HOOK pFunc)
{
    return bsp_dump_register_hook(ModId, pFunc);
}

BSP_S32 DRV_DUMP_GET_BUFFER(DUMP_SAVE_MOD_ENUM ModId, BSP_CHAR** buffer, BSP_U32* length)
{
    return bsp_dump_get_buffer(ModId, buffer, length);
}

BSP_S32 DRV_DUMP_SAVE_FILE(DUMP_SAVE_FILE_ENUM FileId, BSP_CHAR* buffer, BSP_U32 length, DUMP_SAVE_FILE_MODE_ENUM mode)
{
    return bsp_dump_save_file(FileId, buffer, length, mode);
}

#ifdef __cplusplus
}
#endif

