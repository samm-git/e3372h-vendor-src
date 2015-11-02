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
#include <linux/earlysuspend.h>
#include <linux/suspend.h>
#include "osl_types.h"
#include <linux/workqueue.h>

extern struct workqueue_struct *suspend_work_queue;
extern struct work_struct early_suspend_work;
extern struct work_struct late_resume_work;

u32 g_ulEarlySuspend1 = 0;
u32 g_ulEarlySuspend2 = 0;

void earlysuspend_suspend0 (struct early_suspend *h)
{
    g_ulEarlySuspend1 = 1;
    earlysuspend_print_dbg("earlysuspend_suspend0");
}
void earlysuspend_resume0(struct early_suspend *h)
{
    g_ulEarlySuspend1 = 0;
    earlysuspend_print_dbg("earlysuspend_resume0");
}
void earlysuspend_suspend1 (struct early_suspend *h)
{
    g_ulEarlySuspend2 = 1;
    earlysuspend_print_dbg("earlysuspend_suspend1");

}
void earlysuspend_resume1(struct early_suspend *h)
{
    g_ulEarlySuspend2 = 0;
    earlysuspend_print_dbg("earlysuspend_resume1");

}

u32 earlysuspend_show()
{
    printk("g_ulEarlySuspend1: = %x", g_ulEarlySuspend1);
    printk("g_ulEarlySuspend2: = %x", g_ulEarlySuspend2);
}

struct early_suspend earlysuspend_struct_test01 =
{
    NULL,
    EARLY_SUSPEND_LEVEL_BLANK_SCREEN,
    earlysuspend_suspend0,
    earlysuspend_resume0
};
struct early_suspend earlysuspend_struct_test02 =
{
    NULL,
    EARLY_SUSPEND_LEVEL_STOP_DRAWING,
    earlysuspend_suspend1,
    earlysuspend_resume1
};



s32 earlysuspend_test_register()
{
    register_early_suspend(&earlysuspend_struct_test01);
    register_early_suspend(&earlysuspend_struct_test02);
    return 0;
}

s32 earlysuspend_test_unregister()
{
    unregister_early_suspend(&earlysuspend_struct_test01);
    unregister_early_suspend(&earlysuspend_struct_test02);
    return 0;
}

