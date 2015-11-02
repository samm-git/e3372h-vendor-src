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
#include "wakelock_debug.h"

/**************************************************************************
*此处接口用于wakelock LLT，请勿使用
***************************************************************************/
#define LOCK_TEST_OK        0
#define LOCK_TEST_ERROR  1

static struct wake_lock llt_lock;

int wake_lock_init_case0(void)
{
    wake_lock_init(&llt_lock, WAKE_LOCK_SUSPEND, "test_wakelock");
    return LOCK_TEST_OK;
}
int wake_lock_case0(void)
{
    int ret = 0;
    wake_lock(&llt_lock);
    ret = wake_lock_active (&llt_lock);
    if (ret)
        return LOCK_TEST_OK;
    else
        return LOCK_TEST_ERROR;
}

int wake_unlock_case0(void)
{
    int ret = 0;
    wake_unlock(&llt_lock);
    ret = wake_lock_active (&llt_lock);
    if (!ret)
        return LOCK_TEST_OK;
    else
        return LOCK_TEST_ERROR;
}

void debug_wakelock(void)
{
    int i = 0;
   vote_printf("All module vote status(1: oppose sleeping, 0: approve sleeping)\n");
    for (i = 0; i < wakelock_count; i++){
        if(hi6930_wakelock[i].lock){
                int ret = ((unsigned int)hi6930_wakelock[i].lock->flags >> 9) & 0x3;
                if (ret)
                    ret = 1;
                else
                    ret = 0;
                vote_printf("lock name : %-20s, %d, LOCK_ID:%d\n",hi6930_wakelock[i].lock->name, ret, hi6930_wakelock[i].lock_id);
        }
   }
    vote_printf("wake lock:         debug_wake_lock(LOCK_ID)\n");
    vote_printf("wake unlock:       debug_wake_unlock(LOCK_ID)\n");
    vote_printf("wake_lock_destroy: debug_wake_lock_destroy(LOCK_ID)\n");
    return;
}

void debug_wake_lock(int id)
{
      int ret = 0;
      struct wake_lock *lock = hi6930_wakelock[id].lock;
      wake_lock(lock);
      ret = ((unsigned int)lock->flags >> 9) & 0x3;
      if (ret){
            ret = 1;
            vote_printf("wak lock name : %s, status :%d \n", lock->name, ret);
            return;
      }
      ret = 0;
      vote_printf("wak lock name : %s, status :%d \n", lock->name, ret);
      return;
}

void debug_wake_unlock(int id)
{
      int ret = 0;
      struct wake_lock *lock = hi6930_wakelock[id].lock;
      wake_unlock(lock);
      ret = ((unsigned int)lock->flags >> 9) & 0x3;
      if (ret){
            ret = 1;
            vote_printf("wak lock name : %s, status :%d \n", lock->name, ret);
            return;
      }
      ret = 0;
      vote_printf("wak lock name : %s, status :%d \n", lock->name, ret);
      return;
}
void debug_wake_lock_destroy(int id)
{
      struct wake_lock *lock = hi6930_wakelock[id].lock;
      wake_lock_destroy(lock);
      return;
}
/*以下部分用于测试MNTN功能 */
void test_wakelock(int num)
{
	/* coverity[var_decl] */
	struct wake_lock test_lock;
	int i = 0;
	while(i <= num){
        wake_lock_init(&test_lock, WAKE_LOCK_SUSPEND, "test_lock");
		/* coverity[uninit_use_in_call] */
	    wake_lock_destroy(&test_lock);
		i++;
	}
	return;
}
void test_wakelock32(int num)
{
    int i = 0;
	for(i = 0; i < num; i++){
	    struct wake_lock test_lock;
	    wake_lock_init(&test_lock, WAKE_LOCK_SUSPEND, "test1_lock");
	}
	return;
}

