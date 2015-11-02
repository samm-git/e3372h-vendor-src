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

/*lint -save -e537*/
#include <bsp_ipc.h>
#include <drv_ipc.h>
#include <osl_bio.h>
#include <osl_irq.h>
#include <bsp_memmap.h>
/*lint -restore +e537*/
BSP_S32 BSP_DRV_IPCIntInit(void)
{
	return  0;
}
BSP_S32 BSP_IPC_SemCreate(BSP_U32 u32SignalNum)
{
	return bsp_ipc_sem_create(u32SignalNum);
}

BSP_S32 BSP_IPC_SemDelete(BSP_U32 u32SignalNum)
{
	return bsp_ipc_sem_delete(u32SignalNum);
}

BSP_S32 BSP_IPC_IntEnable (IPC_INT_LEV_E ulLvl)
{
	return bsp_ipc_int_enable(ulLvl);
}
BSP_S32 BSP_IPC_IntDisable (IPC_INT_LEV_E ulLvl)
{
	return bsp_ipc_int_disable(ulLvl);
}
BSP_S32 BSP_IPC_IntConnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter)
{
	return bsp_ipc_int_connect(ulLvl,(voidfuncptr)routine,parameter);
}

BSP_S32 BSP_IPC_IntDisonnect  (IPC_INT_LEV_E ulLvl,VOIDFUNCPTR routine, BSP_U32 parameter)
{
	return bsp_ipc_int_disconnect(ulLvl,(voidfuncptr)routine,parameter);
}
BSP_S32 BSP_IPC_IntSend(IPC_INT_CORE_E enDstCore, IPC_INT_LEV_E ulLvl)
{
	return bsp_ipc_int_send(enDstCore, ulLvl);
}

BSP_S32 BSP_IPC_SemTake(BSP_U32 u32SignalNum,BSP_S32 s32timeout)
{
	return bsp_ipc_sem_take(u32SignalNum,s32timeout);
}
BSP_VOID BSP_IPC_SemGive(BSP_U32 u32SignalNum)
{
	(void)bsp_ipc_sem_give(u32SignalNum);
	return;
}

BSP_VOID BSP_IPC_SpinLock (BSP_U32 u32SignalNum)
{
	(void)bsp_ipc_spin_lock (u32SignalNum);
 	return;
}

BSP_VOID BSP_IPC_SpinUnLock (BSP_U32 u32SignalNum)
{
	(void)bsp_ipc_spin_unlock (u32SignalNum);
	return;
}
BSP_S32 BSP_IPC_SpinLock_IrqSave(BSP_U32 u32SignalNum, unsigned long *flags)
{
	local_irq_save(*flags);
	return  bsp_ipc_spin_lock(u32SignalNum);
	
}
BSP_S32	BSP_IPC_SpinUnLock_IrqRestore(BSP_U32 u32SignalNum,unsigned long *flags)
{
	BSP_S32 ret = 0;
	ret = bsp_ipc_spin_unlock(u32SignalNum);
	local_irq_restore(*flags);
	return ret;
}

