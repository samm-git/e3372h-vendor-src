/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include "ddr_alg.h"

void    err_log_printf(ulv *err_addr, ul err_data, ul b_data, ul t_type , ul step );
extern void arch_clean_cacheline(addr_t start);
extern void arch_invalidate_cacheline(addr_t start);
extern int put_log( const char *fmt, ...);

int ddr_test_0(ulv* bufa, ulv* bufb, size_t count)
{
    size_t i;
    ulv *p1 = bufa;
    ulv *p2 = bufb;
    printk( KERN_ERR "ddr_test_0,%lx,%lx,%lx\n", (ul)bufa, (ul)bufb, (ul)count);

    for ( i = 0; i < count; i++ )
    {
        *p1++ = i * 3;
        *p2++ = ~i * 3;
    }

    printk( KERN_ERR "ddr_test_0 exit\n");

    return 0;
}
EXPORT_SYMBOL_GPL(ddr_test_0);

int ddr_test_1(ulv* bufa, ulv* bufb, size_t count)
{

    ulv *p1 = bufa;
    ulv *p2 = bufb;
    size_t i;

    printk( KERN_ERR "ddr_test_1,%lx,%lx,%lx\n", (ul)bufa, (ul)bufb, (ul)count);

    for ( i = 0; i < count; i++ )
    {
        *p1++ = 0xFFFFFFFF;
        *p2++ = 0xFFFFFFFF;
    }

    for ( i = 0; i < count; i++ )
    {
        *p1++ = 0;
        *p2++ = 0;
    }
    printk( KERN_ERR "ddr_test_1 exit\n");

    return 0;
}
EXPORT_SYMBOL_GPL(ddr_test_1);

/* prbs 按照cache line 并行测试die0与die1 */

/**********************************************************************************
函数名：int test_prbs_data_align_test_7(ulv* bufa, ulv* bufb, size_t count)

功能描述: 用PRBS align算法检测数据总线

                其他: PASS
 Modification :
 Restriction  : 对外函数

***********************************************************************************/
int test_prbs_data_align_test_7(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *p1;
    ulv *p2;
    ulv *end;
    ul    backdata;
    ul    i = 0;        /* 用于标记随机数循环 */
    ul    r = 0;      /* return 状态 */
    ul    prbs_array[4] = {0x86081fd5, 0xcae689e2, 0x6b648e17, 0x19dd2c6f}; /* PRBS种子已固定 */
    ul    test_type = DDR_TEST_ERR_PRBS_DATA_ALIGN;
    ul    step = 0;
    ul    err_count = 0;
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

    /*按照地址升序 w*/
    while (p1 < end)
    {
        for (i = 0; i < PRBS_LEN_7; i++)
        {
            make_prbs_addrdata_align(prbs_array, backdata, i);

            /*按照地址升序 w*/
            *p1 = backdata;
            *p2 = ~backdata;

            p1 ++;
            p2 ++;

            if (p1 > end)
            {
                break;
            }
        }
    }

    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

    /*按照地址升序 r*/
    while (p1 < end)
    {
        for (i = 0; i < PRBS_LEN_7; i++)
        {
            make_prbs_addrdata_align(prbs_array, backdata, i);

            if (backdata != *p1)
            {
                step = 1;
                r = -1;
                err_log_printf(p1, (ul)*p1, backdata, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            if (~backdata != *p2)
            {
                step = 2;
                r = -1;
                err_log_printf(p2, (ul)*p2, ~backdata, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            p1 ++;
            p2 ++;

            if (p1 > end)
            {
                break;                    /* 测试地址大于结束地址的时候退出循环 */
            }
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;

} /**********************************************************************************
函数名：int test_prbs_data_flip_test_7(ulv* bufa, ulv* bufb, size_t count)

功能描述: 用PRBS flip算法检测数据总线

 Modification :
 Restriction  : 对外函数
***********************************************************************************/
int test_prbs_data_flip_test_7(ulv* bufa, ulv* bufb, size_t count)
{
    ulv   *p1;
    ulv   *p2;
    ulv   *end;
    ul    backdata;
    ul    i = 0;
    ul    k = 0;
    ul    j = 1;
    ul    r = 0;
    ul    step;
    ul    err_count = 0;
    ul    prbs_array[4] = {0x86081fd5, 0xcae689e2, 0x6b648e17, 0x19dd2c6f}; /* PRBS种子已固定 */
    ul    test_type = DDR_TEST_ERR_PRBS_DATA_FLIP;


    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

    /*按照地址升序 w*/
    while (p1 < end)
    {
        for (i = 0; i < PRBS_LEN_7; i++)
        {
            j <<= 1;
            k += 1;
            if (j == 0x0)
            {
                j = 0x1;
                k = 0;
            }

            make_prbs_addrdata_alignflip(prbs_array, backdata, i, k);

            /*按照地址升序 w*/
            *p1 = backdata;
            *p2 = ~backdata;

            p1 ++;
            p2 ++;

            if (p1 > end)
            {
                break;
            }
        }
    }


    k = 0;
    j = 1;
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

    /*按照地址升序 r*/
    while (p1 < end)
    {
        for (i = 0; i < PRBS_LEN_7; i++)
        {
            j <<= 1;
            k += 1;
            if (j == 0x0)
            {
                j = 0x1;
                k = 0;
            }

            make_prbs_addrdata_alignflip(prbs_array, backdata, i, k);
            if (backdata != *p1)
            {
                step = 1;
                r = -1;
                err_log_printf(p1, (ul)*p1, backdata, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            if (~backdata != *p2)
            {
                step = 2;
                r = -1;
                err_log_printf(p2, (ul)*p2, ~backdata, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            p1 ++;
            p2 ++;

            if (p1 > end)
            {
                break;                    /* 测试地址大于结束地址的时候退出循环 */
            }
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;

}

void    err_log_printf(ulv *err_addr, ul err_data, ul b_data, ul t_type , ul step )
{
    printk( KERN_ERR"FAILURE: 0x%08lX,0x%08lX,0x%08lX,0x%lX,0x%lX.\n",
            (ul)err_addr, err_data, b_data, t_type, step);
    put_log( "FAILURE: 0x%08lX,0x%08lX,0x%08lX,0x%lX,0x%lX.\n",
             (ul)err_addr, err_data, b_data, t_type, step );
    //show_current_freq();

    ERR_TOTAL++ ;        /* 全局错误地址计数 */
}

int test_walking_ones(ulv* bufa, ulv* bufb, size_t count)
{
    ulv   *p1;
    ulv   *p2;
    ulv   *end;
    ul    pattern, pattern_a, pattern_b;
    ul    i = 0;        /* 循环测试次数 */
    ul    r = 0;
    ul    step;
    ul    err_count = 0;
    ul start_pattern = 0;
    ul    test_type = DDR_TEST_ERR_VARIATION_WALKING_ONES;
    ul index;

    for (i = 0;i < 2;i++)
    {
        p1 = (ulv*)bufa;
        p2 = (ulv*)bufb;
        end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

        /*按照地址升序 w*/
        while (p1 < end)
        {
            index = 0;

            for (start_pattern = 0x55555555; start_pattern != 0x0; start_pattern <<= 1)
            {
                if (i != 0)
                {
                    pattern = ~ start_pattern;
                }
                else
                {
                    pattern = start_pattern;
                }

                /*按照地址升序 w*/

                if (index % 2)
                {
                    pattern_a = pattern;
                    pattern_b  = ~pattern;
                }
                else
                {
                    pattern_a = ~pattern;
                    pattern_b  = pattern;
                }


                *p1 = pattern_a;
                *p2 = pattern_b;
                p1 ++;
                p2 ++;

                pattern <<= 1;
                if (pattern == 0x0)
                {
                    pattern = 0x55555555;
                }

                index++;

                if (p1 > end)
                {
                    break;
                }
            }
        }


        /*按照地址升序 r*/
        p1 = (ulv*)bufa;
        p2 = (ulv*)bufb;
        end = (ulv *)((ul)bufa + ((ul)count * (CPU_BITS_WIDE / 8)) - 1);

        while (p1 < end)
        {
            index = 0;

            for (start_pattern = 0x55555555; start_pattern != 0x0; start_pattern <<= 1)
            {
                if (i != 0)
                {
                    pattern = ~ start_pattern;
                }
                else
                {
                    pattern = start_pattern;
                }

                if (index % 2)
                {
                    pattern_a = pattern;
                    pattern_b  = ~pattern;
                }
                else
                {
                    pattern_a = ~pattern;
                    pattern_b  = pattern;
                }

                if (pattern_a != (*p1))
                {
                    step = 1;
                    r = -1;
                    err_log_printf(p1, (ul)*p1, pattern_a, test_type , step );
                    if (++ err_count > ERR_COUNT)
                    {
                        return r;        /* 错误检出大于阈值退出测试 */
                    }
                }

                if (pattern_b != (*p2))
                {
                    step = 2;
                    r = -1;
                    err_log_printf(p2, (ul)*p2, pattern_b, test_type , step );
                    if (++ err_count > ERR_COUNT)
                    {
                        return r;        /* 错误检出大于阈值退出测试 */
                    }
                }

                p1 ++;
                p2 ++;
                pattern <<= 1;
                if (pattern == 0x0)
                {
                    pattern = 0x55555555;
                }

                index++;

                if (p1 > end)
                {
                    break;                    /* 测试地址大于结束地址的时候退出循环 */
                }
            }
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;
}





/*****************************************************************************
 Description  : test_variation_walking_ones内存测试函数
             用1 走步数组为背景数据)检测内存，共计4*4=16n。
 Modification :
 Restriction  : 需保证在颗粒外围互联正常的前提下对存储单元进行测试
*****************************************************************************/
/*aulBackDataArray[]数组元素请勿调整顺序及删减，BitMap如下，采用1左移位产生
bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit    bit
31    30    29    28    27    26    25    24    23    22    21    20    19    18    17    16    15    14    13    12    11    10    9    8    7    6    5    4    3    2    1    0
3    2    1    0    3    2    1    0    3    2    1    0    3    2    1    0    3    2    1    0    3    2    1    0    3    2    1    0    3    2    1    0

0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0
0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1
0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1
1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1
1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1
1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0
1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0    1    0    1    1    0    1    0    0
0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0    0    1    1    1    1    0    0    0

END*/
int test_variation_walking_ones(ulv* bufa, ulv* bufb, size_t count)
{
    ulv   *p1;
    ulv   *p2;
    ul    pattern_array[] = { 0x1E  , 0x2D  , 0x4B  , 0x87, 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87, 0x78, 0x69, 0x5A, 0x4B, 0x3C, 0x2D, 0x1E, 0x0F };
    ul    array_num;
    ul    pattern_a, pattern_b;
    ul    index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul    offset;                                                                            /* 内存块测试偏移变量 */
    ul    step = 0;                                                                       /* 第几次读 */
    ul    err_count = 0;                                                                  /* 错误地址数 */
    ul    test_type = DDR_TEST_ERR_VARIATION_WALKING_ONES;                                /* 测试算法标记 */
    ul    r = 0;
    ul    i = 0;
    ul    j = 2;


    /* 背景数据组数 */
    array_num = sizeof(pattern_array) / sizeof(pattern_array[0]);

    for (i = 0;i < j;i++)
    {
        for (index = 0; index < array_num ; index++)                /* 背景数据循环 */
        {
            if (i != 0)
            {
                pattern_a = ~ ULL_BYTE(pattern_array[index]);           /* 背景数据拼装成32bit数据 */
                pattern_b = ULL_BYTE(pattern_array[index]);            /* 背景数据拼装成32bit数据 */
            }
            else
            {
                pattern_a = ULL_BYTE(pattern_array[index]);            /* 背景数据拼装成32bit数据 */
                pattern_b = ~ ULL_BYTE(pattern_array[index]);           /* 背景数据拼装成32bit数据 */
            }

            /*↑ (wx)*/
            p1 = (ulv*)bufa;
            p2 = (ulv*)bufb;
            for (offset = 0; offset < count ; offset ++)
            {
                *p1 = pattern_a;
                *p2 = pattern_b;
                p1 ++;
                p2 ++;
            }

            /*↑(rx) */
            p1 = (ulv*)bufa;
            p2 = (ulv*)bufb;
            for (offset = 0; offset < count ; offset ++)
            {

                if (pattern_a != *p1)
                {
                    step = 1;
                    r = -1;
                    err_log_printf(p1, (ul)*p1, pattern_a, test_type , step );
                    if (++ err_count > ERR_COUNT)
                    {
                        return r;        /* 错误检出大于阈值退出测试 */
                    }
                }

                if (pattern_b != *p2)
                {
                    step = 2;
                    r = -1;
                    err_log_printf(p2, (ul)*p2, pattern_b, test_type , step );
                    if (++ err_count > ERR_COUNT)
                    {
                        return r;        /* 错误检出大于阈值退出测试 */
                    }
                }

                p1 ++;
                p2 ++;

            }
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;
}


/*****************************************************************************
 Description  : test_variation_walking_ones内存测试函数
             用1 走步数组为背景数据)检测内存，共计4*4=16n。
 Modification :
 Restriction  : 需保证在颗粒外围互联正常的前提下对存储单元进行测试
*****************************************************************************/

int test_own_addr(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *p1;
    ulv *p2;
    // ulv *end;
    ul    offset;                                                                            /* 内存块测试偏移变量 */
    ul    step = 0;                                                                       /* 第几次读 */
    ul    err_count = 0;                                                                  /* 错误地址数 */
    ul    test_type = TEST_OWN_ADDR;                                                        /* 测试算法标记 */
    ul    r = 0;
    ul    pattern_a, pattern_b;


    /*↑ (wx)*/
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    for (offset = 0; offset < count ; offset ++)
    {
        *p1 = (ul)p1;
        *p2 = (ul)p2;
        p1 ++;
        p2 ++;
    }

    /*↑(rx) */
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    for (offset = 0; offset < count ; offset ++)
    {

        if ((ul)p1 != *p1)
        {
            step = 1;
            r = -1;
            err_log_printf(p1, (ul)*p1, (ul)p1, test_type , step );
            if (++ err_count > ERR_COUNT)
            {
                return r;        /* 错误检出大于阈值退出测试 */
            }
        }

        if ((ul)p2 != *p2)
        {
            step = 2;
            r = -1;
            err_log_printf(p2, (ul)*p2, (ul)p2, test_type , step );
            if (++ err_count > ERR_COUNT)
            {
                return r;        /* 错误检出大于阈值退出测试 */
            }
        }

        p1 ++;
        p2 ++;

    }

    if (err_count > ERR_COUNT)
    {
        return r;        /* 错误检出大于阈值退出测试 */
    }


    /*↑ (wx)*/
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    for (offset = 0; offset < count ; offset ++)
    {
        pattern_a = (ul)(((ul)p1 & 0xffff) << (CPU_BITS_WIDE / 2) | ((ul)p1 & 0xffff0000) >> (CPU_BITS_WIDE / 2) )  ;
        pattern_b = (ul)(((ul)p2 & 0xffff) << (CPU_BITS_WIDE / 2) | ((ul)p2 & 0xffff0000) >> (CPU_BITS_WIDE / 2) )  ;
        *p1 = pattern_a;
        *p2 = pattern_b;
        p1 ++;
        p2 ++;
    }

    /*↑(rx) */
    p1 = (ulv*)bufa;
    p2 = (ulv*)bufb;
    for (offset = 0; offset < count ; offset ++)
    {
        pattern_a = (ul)(((ul)p1 & 0xffff) << (CPU_BITS_WIDE / 2) | ((ul)p1 & 0xffff0000) >> (CPU_BITS_WIDE / 2) )  ;
        pattern_b = (ul)(((ul)p2 & 0xffff) << (CPU_BITS_WIDE / 2) | ((ul)p2 & 0xffff0000) >> (CPU_BITS_WIDE / 2) )  ;

        if (pattern_a != *p1)
        {
            step = 3;
            r = -1;
            err_log_printf(p1, (ul)*p1, pattern_a, test_type , step );
            if (++ err_count > ERR_COUNT)
            {
                return r;        /* 错误检出大于阈值退出测试 */
            }
        }

        if (pattern_b != *p2)
        {
            step = 4;
            r = -1;
            err_log_printf(p2, (ul)*p2, pattern_b, test_type , step );
            if (++ err_count > ERR_COUNT)
            {
                return r;        /* 错误检出大于阈值退出测试 */
            }
        }

        p1 ++;
        p2 ++;

    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;

}

/*****************************************************************************
 Function     : ddr_test_bit_equalizing
 Description  : ddr_test_bit_equalizing内存测试函数
             用ddr_test_bit_equalizing 算法(以bit均衡数组为背景数据)检测内存，共计2*48=96n。
          ↑(wx)  ;↑(rx)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : 0:    成功
                其他: 失败
 Modification :
 Restriction  : 需保证在颗粒外围互联正常的前提下对存储单元进行测试
*****************************************************************************/
/*aulBackDataArray[]数组元素请勿调整顺序及删减，BitMap均衡分布是本算法的核心思想，
算法原语(Wx),(Rx),(Rx,Wx+1),(Rx+1)，其中x为背景数据数组元素下标
 bit3 bit2 bit1 bit0     bit3 bit2 bit1 bit0     bit3 bit2 bit1 bit0     bit3 bit2 bit1 bit0   <bit位>
    6    3    2    1            3    2    1    6            2    1    6    3            1    6    3    2    <单bit连续被读写的次数>
1    0    0    0    0    0        0    0    0    0    0        0    0    0    0    0        0    0    0    0    0
2    0    0    0    1    1        0    0    1    0    2        0    1    0    0    4        1    0    0    0    8
3    0    0    1    0    2        0    1    0    0    4        1    0    0    0    8        0    0    0    1    1
4    0    1    1    1    7        1    1    1    0    E        1    1    0    1    D        1    0    1    1    B
5    0    1    0    0    4        1    0    0    0    8        0    0    0    1    1        0    0    1    0    2
6    0    1    0    1    5        1    0    1    0    A        0    1    0    1    5        1    0    1    0    A
7    1    0    1    0    A        0    1    0    1    5        1    0    1    0    A        0    1    0    1    5
8    1    0    1    1    B        0    1    1    1    7        1    1    1    0    E        1    1    0    1    D
9    1    0    0    0    8        0    0    0    1    1        0    0    1    0    2        0    1    0    0    4
10    1    1    0    1    D        1    0    1    1    B        0    1    1    1    7        1    1    1    0    E
11    1    1    1    0    E        1    1    0    1    D        1    0    1    1    B        0    1    1    1    7
12    1    1    1    1    F        1    1    1    1    F        1    1    1    1    F        1    1    1    1    F

END*/

int ddr_test_bit_equalizing(ulv* bufa, ulv* bufb, size_t count)
{
    /* aulBackDataArray[]数组元素请勿调整顺序及删减，BitMap均衡分布是本算法的核心思想 */

    /* 测试时长受限时请选择6-3-2-1编码背景数据，共48组
        uint32 aulBackDataArray[] ={
            0x00,0x11,0x22,0x77,0x44,0x55,0xAA,0xBB,0x88,0xDD,0xEE,0xFF,
            0x00,0x22,0x44,0xEE,0x88,0xAA,0x55,0x77,0x11,0xBB,0xDD,0xFF,
            0x00,0x44,0x88,0xDD,0x11,0x55,0xAA,0xEE,0x22,0x77,0xBB,0xFF,
            0x00,0x88,0x11,0xBB,0x22,0xAA,0x55,0xDD,0x44,0xEE,0x77,0xFF };
    */

    /* 测试时长不受限时请选择8-4-2-1编码背景数据，共64组 */
    ul aulBackDataArray[] =
    {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
        0x00, 0x22, 0x44, 0x66, 0x88, 0xAA, 0xCC, 0xEE, 0x11, 0x33, 0x55, 0x77, 0x99, 0xBB, 0xDD, 0xFF,
        0x00, 0x44, 0x88, 0xCC, 0x11, 0x55, 0x99, 0xDD, 0x22, 0x66, 0xAA, 0xEE, 0x33, 0x77, 0xBB, 0xFF,
        0x00, 0x88, 0x11, 0x99, 0x22, 0xAA, 0x33, 0xBB, 0x44, 0xCC, 0x55, 0xDD, 0x66, 0xEE, 0x77, 0xFF
    };

    ul  pattern, pattern_a, pattern_b;
    ulv *p1;
    ulv *p2;
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul step = 0;                                                                       /* 第几次读 */
    ul err_count = 0;
    ul test_type = DDR_TEST_ERR_BIT_EQUALIZING;                                        /* 测试算法标记 */
    ul r = 0;                                                                  /* 错误返回状态 */


    /* 背景数据组数 */
    ulBackDataGrpNum = sizeof(aulBackDataArray) / sizeof(aulBackDataArray[0]);

    for (index = 0; index < ulBackDataGrpNum ; index++)    /* 背景数据循环 */
    {
        pattern = ULL_BYTE(aulBackDataArray[index]);            /* 背景数据拼装成32bit数据 */
        pattern_a = pattern;
        pattern_b = ~pattern;

        p1 = (ulv*)bufa;
        p2 = (ulv*)bufb;
        for (offset = 0; offset < count ; offset ++)
        {
            *p1 = (ul)pattern_a;
            *p2 = (ul)pattern_b;
            p1 ++;
            p2 ++;
        }

        /*↑(rx) */
        p1 = (ulv*)bufa;
        p2 = (ulv*)bufb;
        for (offset = 0; offset < count ; offset ++)
        {

            if ((ul)pattern_a != *p1)
            {
                step = index << SHIFTING | (index + 1);                 /* 第n次读 */
                r = -1;
                err_log_printf(p1, (ul)*p1, (ul)pattern_a, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            if ((ul)pattern_b != *p2)
            {
                step = index << SHIFTING | (index + 1);                 /* 第n次读 */
                r = -1;
                err_log_printf(p2, (ul)*p2, (ul)pattern_b, test_type , step );
                if (++ err_count > ERR_COUNT)
                {
                    return r;        /* 错误检出大于阈值退出测试 */
                }
            }

            p1 ++;
            p2 ++;

        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return r;
}


/*****************************************************************************
 Function     : ddr_test_march_ic
 Description  : March IC内存测试函数
             用March IC- 算法(以0 和1 为背景数据)检测内存，共计12*6=72n。
    ↑ (w1);↑( r 1); ↑ (w0);↑( r0 ,  w1);      ↑( r1 ,  w0);
    ↓( r0 ,  w1); ↓( r1 ,  w0);     Delay    ↓( r0); ↓
 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : -1:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_ic(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_IC;                                                /* 测试算法标记 */
    ul limit_temp = limit ;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */

    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /*↑ (w1)*/
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w1*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);    /* 地址按cache line 大小进行偏移 */
        }

        /*↑(r1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 1;                                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↑ (w0)*/
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /*↑( r0,  w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 2;                                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1*/
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }


        /* ↑( r1 ,  w0); */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓( r0 ,  w1) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↓ ( r1 ,  w0) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        DDR_TEST_DELAY(DDR_TEST_MARCH_IC_DELAY);                                            /* 测试延时 */


        /*↓(r0) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */                                                                                /* 同时向die0 与die1并行读4条cache_line的数据 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
    }
    SPRINTF_INFOR(err_count, test_type);    /* 当前算法错误数 */
    return 0;
}


/*****************************************************************************
 Function     : ddr_test_march_lr
 Description  : March Lr内存测试函数
             用March Lr 算法(以0 和1 为背景数据)检测内存，共计14*6=84n。
          ↓(w0)↓(r0,w1)↑(r1,w0,r0,w1)↑(r1,w0)↑(r0,w1,r1,w0)↓(r0)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : -1:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_lr(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_LR;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */
    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /* ↓(w0) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /* ↓(r0,w1) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }


        /*↑(r1,w0,r0,w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↑(r1,w0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↑(r0,w1,r1,w0) */

        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↓(r0) */
        up_down_status = DOWN;                                                               /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
    }

    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}






/*****************************************************************************
 Function     : ddr_test_march_raw
 Description  : March Raw内存测试函数
             用March Raw 算法(以0 和1 为背景数据)检测内存，共计26*6=156n。
          ↑(w0)↑(r0,w0,r0,r0,w1,r1)↑(r1,w1,r1,r1,w0,r0)↓(r0,w0,r0,r0,w1,r1)↓(r1,w1,r1,r1,w0,r0)↓(r0)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : -1:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_raw(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_RAW;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */
    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /*↑ (w0)*/
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /*↑(r0,w0,r0,r0,w1,r1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* r0 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↑(r1,w1,r1,r1,w0,r0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* r1 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 8;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r0,w0,r0,r0,w1,r1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 9;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 10;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* r0 */
            step = index << SHIFTING | 11;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 12;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r1,w1,r1,r1,w0,r0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 13;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 14;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* r1 */
            step = index << SHIFTING | 15;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 16;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 17;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}





/*****************************************************************************
 Function     : ddr_test_march_sl
 Description  : March SL内存测试函数
             用March SL 算法(以0 和1 为背景数据)检测内存，共计42*6=252n。
          ↑(w0) ;↑(r0,r0,w1,w1,r1,r1,w0,w0,r0,w1) ;↑(r1,r1,w0,w0,r0,r0,w1,w1,r1,w0);
          ↓(r0,r0,w1,w1,r1,r1,w0,w0,r0,w1) ↓(r1,r1,w0,w0,r0,r0,w1,w1,r1,w0) ;↓(r0)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : -1:失败
                0: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/

int ddr_test_march_sl(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_SL;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */
    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /* ↑w0 */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /*↑(r0,r0,w1,w1,r1,r1,w0,w0,r0,w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↑(r1,r1,w0,w0,r0,r0,w1,w1,r1,w0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 8;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 9;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 10;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r0,r0,w1,w1,r1,r1,w0,w0,r0,w1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 11;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 12;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 13;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 14;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 15;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }


        /*↓(r1,r1,w0,w0,r0,r0,w1,w1,r1,w0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 16;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 17;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 18;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 19;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 20;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
        /*↓(r0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 21;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}





/*****************************************************************************
 Function     : ddr_test_march_sr
 Description  : March Sr内存测试函数
             用March Sr 算法(以0 和1 为背景数据)检测内存，共计14*6=84n。
          ↓(w0);↑(r0,W1,R1, w0);↑(r0,r0);↑(w1) ↓(r1,w0,r0,w1);↓(r1,r1)

 Input        : base:    内存首地址
            limit        内存空间字节数
Output       : None
 Return       : -1:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_sr(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_SR;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */
    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /* ↓(w0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /* ↑(r0,W1,R1, w0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↑(r0,r0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↑(w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */

        }

        /* ↓(r1,w0,r0,w1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
        /*↓(r1,r1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 8;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
    }
    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}


/*****************************************************************************
 Function     : ddr_test_march_ss
 Description  : March Ss内存测试函数
             用March Ss 算法(以0 和1 为背景数据)检测内存，共计22*6=132n。
          ↑(w0) ;↑(r0,r0,W0,r0,w1) ;↑(r1,r1,w1,r1,w0);↓(r0,r0,w0,r0,w1) ;↓(r1,r1,w1,r1,w0) ;↓(r0)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : 0:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_ss(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_SS;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */
    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /*↑ (w0)*/
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }


        /*↑(r0,r0,W0,r0,w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
        /*↑(r1,r1,w1,r1,w0) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
        /*↓(r0,r0,w0,r0,w1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* r0 */
            step = index << SHIFTING | 8;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 9;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r1,r1,w1,r1,w0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 10;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* r1 */
            step = index << SHIFTING | 11;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 12;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 13;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

    }

    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}



/*****************************************************************************
 Function     : ddr_test_march_g
 Description  : March G内存测试函数
             用March G 算法(以0 和1 为背景数据)检测内存，共计23*6=138n。
          ↑(w0);↑(r0,w1,r1,w0,r0,w1);↑(r1,w0,w1);↓(r1,w0, w1,w0);↓(r0,w1,w0);↓(r0,w1,r1);↓(r1,w0,r0)

 Input        : base:    内存首地址
            limit        内存空间字节数
 Output       : None
 Return       : -1:     失败
                其他: PASS
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int ddr_test_march_g(ulv* bufa, ulv* bufb, size_t count)
{
    ulv *base = bufa;
    ul limit = count << 2;
    ul Data0[] = BACKGROUND_DATA0;                                                    /* 背景数据 "0"向量 */
    ul Data1[] = BACKGROUND_DATA1;                                                    /* 背景数据 "1"向量 */
    ulv *addr0_0,    *addr0_1;                                                    /* Die0 地址 */
    ulv *addr1_0,    *addr1_1;                                                    /* Die0 地址 */
    ul ulBackDataGrpNum;                                                            /* 背景数据的组数 */
    ul index;                                                                            /* 背景数据需依次循环遍历的变量 */
    ul offset;                                                                            /* 内存块测试偏移变量 */
    ul up_down_status = UP;                                                            /* UP 升序，DOWN 降序 */
    ul step = 0;                                                                       /* 第n 次读 */
    ul err_count = 0;                                                                  /* 错误地址数 */
    ul *err = &err_count;                                                               /* 错误地址数指针 */
    ul test_type = DDR_TEST_ERR_MARCH_G;                                            /* 测试算法标记 */
    ul limit_temp = limit;                                    /* 内存总字节数 */
    ul return_status;                                                                    /* 错误返回状态 */

    base = base;

    ulBackDataGrpNum = sizeof(Data0) / sizeof(Data0[0]);                                        /* 背景数据组数 */
    for (index = 0; index < ulBackDataGrpNum; index++)                                                /* 测试多组背景数据 */
    {
        /*↑ (w0)*/
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* w0*/
            /*  双die双交织并行模式，同时向4条cache_line写数据 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }

        /*↑(r0,w1,r1,w0,r0,w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 1;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 2;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 3;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */

        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↑(r1,w0,w1) */
        up_down_status = UP;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 4;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r1,w0, w1,w0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 5;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /* ↓(r0,w1,w0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 6;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r0,w1,r1) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r0 */
            step = index << SHIFTING | 7;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);

            /* w1 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index]);
            /* r1 */
            step = index << SHIFTING | 8;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);

            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }

        /*↓(r1,w0,r0) */
        up_down_status = DOWN;                                                                   /* UP 升序，DOWN 降序 */
        Q_INTERLEAVE_ADDR(base, up_down_status, limit_temp);                                  /* 内存地址初始化*/
        for (offset = 0; offset < (limit_temp >> 1) ; offset += CACHE_LINE_SIZE)                      /* 地址按cache line 大小进行偏移 */
        {
            /* r1 */
            step = index << SHIFTING | 9;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data1[index] , (ulv *)err, test_type, step);


            /* w0 */
            q_write_and_flush_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index]);
            /* r0 */
            step = index << SHIFTING | 10;                                                       /* 第n次读 */
            x_read_and_check_cache_line((ulv *)addr0_0, (ulv *)addr0_1, (ulv *)addr1_0, (ulv *)addr1_1, Data0[index] , (ulv *)err, test_type, step);


            Q_OFFSET_ADDR ((ulv *)&addr0_0, (ulv *)&addr0_1, (ulv *)&addr1_0, (ulv *)&addr1_1, up_down_status);                    /* 地址按cache line 大小进行偏移 */
        }
        return_status = BREAK_TEST(err_count, test_type);
        if (CUR_TES_CONTINUE != return_status)
        {
            return -1;            /* 根据错误地址数判断是否退出测试 */
        }
    }

    SPRINTF_INFOR(err_count, test_type);                                                                    /* 当前算法错误数 */
    return 0;
}


/*****************************************************************************
 Function     : q_write_and_flush_cache_line
 Description  : q_write_and_flush_cache_line    将ullData写入base_addr为起始地址的内存中, 并且把cache line中的数据刷入到内存中.
 Input        : base_addr    起始地址
              :    ullData        当前写入内存的背景数据
 Output       : None
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
void    q_write_and_flush_cache_line(ulv *die0_0, ulv *die0_1, ulv *die1_0, ulv *die1_1, ul ulldata)
{
    ul offset;                                                    /* 内存块测试偏移*/
    ulv *addr_die0_0     = die0_0;                                /* die0 前半段 */
    ulv *addr_die0_1     = die0_1;                                /* die0 后半段 */
    ulv *addr_die1_0     = die1_0;                                /* die1 前半段 */
    ulv *addr_die1_1     = die1_1;                                /* die1 后半段 */
    ul data ;                                                    /* 存放背景数据*/


    for (offset = 0; offset < QWORD_NUM_IN_CACHE_LINE ; offset ++)        /* 内存块测试偏移*/
    {
        if ((offset % 2) != 0)
        {
            data = ~ ulldata;                   /* 奇地址为相反数*/
        }
        else
        {
            data = ulldata;                                        /* 偶地址为当前数*/
        }


        *(ulv *)(addr_die0_0 ) = data;                        /* die0 前半段 */
        *(ulv *)(addr_die1_0 ) = ~ data;                       /* die1 前半段 */

        *(ulv *)(addr_die1_1) = data;                        /* die1 后半段 */
        *(ulv *)(addr_die0_1) = ~ data;                       /* die0 后半段 */

        addr_die0_0 ++;                                            /* die0 前半段内存块测试偏移4个字节 */
        addr_die1_0 ++;                                            /* die1 前半段内存块测试偏移4个字节 */
        addr_die1_1 ++;                                            /* die1 后半段内存块测试偏移4个字节 */
        addr_die0_1 ++;                                            /* die0 后半段内存块测试偏移4个字节 */
    }

    arch_clean_cacheline((ul)die0_0 );                            /* die0 前半段 */
    arch_clean_cacheline((ul)die1_0 );                            /* die1 前半段 */

    arch_clean_cacheline((ul)die1_1 );                                /* die1 后半段 */
    arch_clean_cacheline((ul)die0_1 );                                /* die0 后半段 */
}


/* =============================================================================
**  Function : Q_OFFSET_ADDR
**  description: skip some addr offset
**
** =============================================================================
*/
void Q_OFFSET_ADDR(ulv *addr0_0, ulv *addr0_1, ulv *addr1_0, ulv *addr1_1, ul up_down_status)
{
    if (up_down_status == UP)
    {
        (*addr0_0) += CACHE_LINE_SIZE;
        (*addr1_0) += CACHE_LINE_SIZE;
        (*addr0_1) += CACHE_LINE_SIZE;
        (*addr1_1) += CACHE_LINE_SIZE;
    }
    else
    {
        (*addr0_0) -= CACHE_LINE_SIZE;
        (*addr1_0) -= CACHE_LINE_SIZE;
        (*addr0_1) -= CACHE_LINE_SIZE;
        (*addr1_1) -= CACHE_LINE_SIZE;
    }
}




/*****************************************************************************
 Function     : x_read_and_check_cache_line
 Description  : x_read_and_check_cache_line    cache line大小，将base_addr为起始地址内存中的数据读出并比较.
 Input        : base_addr    起始地址
              :    ullData        当前写入内存的背景数据
              : test_type    测试算法类型
 Output       : None
 Return       : 1:    成功
                其他: 失败
 Modification :
 Restriction  : 对外函数
*****************************************************************************/
int x_read_and_check_cache_line(ulv *die0_0, ulv *die0_1, ulv *die1_0, ulv *die1_1, ul ulldata, ulv *error_count, ul test_type, ul step)
{
    ul offset;                                                    /* 内存块测试偏移*/
    ulv *addr_die0_0     = die0_0;                                /* die0 前半段 */
    ulv *addr_die0_1     = die0_1;                                /* die0 后半段 */
    ulv *addr_die1_0     = die1_0;                                /* die1 前半段 */
    ulv *addr_die1_1     = die1_1;                                /* die1 后半段 */
    ul data_temp, data;
    ulv err_die0_0 = 0;                                            /* 用于记录与offset对应的错误偏移 */
    ulv err_die0_1 = 0;                                            /* 用于记录与offset对应的错误偏移 */
    ulv err_die1_0 = 0;                                            /* 用于记录与offset对应的错误偏移 */
    ulv err_die1_1 = 0;                                            /* 用于记录与offset对应的错误偏移 */
    ulv *err_count = error_count;                               /* 错误计数 */
    ul die = 0;                                                  /* 错误die 标识 */

    for (offset = 0; offset < QWORD_NUM_IN_CACHE_LINE ; offset ++)        /* 内存块测试偏移 */
    {
        if ((offset % 2) != 0)
        {
            data = ~ ulldata;                   /* 奇地址为相反数*/
        }
        else
        {
            data = ulldata;                                        /* 偶地址为当前数*/
        }

        /* die0 前半段 */
        data_temp =    *(ulv *)(addr_die0_0);
        if ( data_temp != data)
        {
            (*err_count)++;
            ERR_TOTAL++;
            err_die0_0 |= 1 << offset;                                /* 错误cache line字节对应 */
            die = 0;
            X_DDR_TEST_LOG_ERR_PATTERN_EX(addr_die0_0, data_temp, data , test_type, offset, step, die);
        }

        /* die1 前半段 */
        data_temp =    *(ulv *)(addr_die1_0);
        if ( data_temp != (~data))
        {
            (*err_count)++;
            ERR_TOTAL++;
            err_die1_0 |= 1 << offset;                                /* 错误cache line字节对应 */
            die = 2;
            X_DDR_TEST_LOG_ERR_PATTERN_EX(addr_die1_0, data_temp, ~data , test_type, offset, step, die);
        }

        /* die1 后半段 */
        data_temp =    *(ulv *)(addr_die1_1);
        if ( data_temp != data)
        {
            (*err_count)++;
            ERR_TOTAL++;
            err_die1_1 |= 1 << offset;                                /* 错误cache line字节对应 */
            die = 3;
            X_DDR_TEST_LOG_ERR_PATTERN_EX(addr_die1_1, data_temp, data , test_type, offset, step, die);
        }

        /* die0后半段 */
        data_temp =    *(ulv *)(addr_die0_1);
        if ( data_temp != (~data))
        {
            (*err_count)++;
            ERR_TOTAL++;
            err_die0_1 |= 1 << offset;                                /* 错误cache line字节对应 */
            die = 1;
            X_DDR_TEST_LOG_ERR_PATTERN_EX(addr_die0_1, data_temp, ~data , test_type, offset, step, die);
        }

        addr_die0_0 ++;                                            /* die0 前半段内存块测试偏移4个字节 */
        addr_die1_0 ++;                                            /* die1 前半段内存块测试偏移4个字节 */
        addr_die1_1 ++;                                            /* die1 后半段内存块测试偏移4个字节 */
        addr_die0_1 ++;                                            /* die0 后半段内存块测试偏移4个字节 */
    }
    arch_invalidate_cacheline((unsigned long)die0_0);
    arch_invalidate_cacheline((unsigned long)die0_1);
    arch_invalidate_cacheline((unsigned long)die1_0);
    arch_invalidate_cacheline((unsigned long)die1_1);

    if ((err_die0_0 | err_die0_1 | err_die1_0  | err_die1_1 ) != 0 )           /* 记录同一cache line有多个Word字节出错的情况 */
    {
        return T_FAIL;
    }
    return T_PASS;
}

/* =============================================================================
**  Function : X_DDR_TEST_LOG_ERR_PATTERN_EX
**  description: detect whether continue test or not and log errs
**
** =============================================================================
*/
int BREAK_TEST(ul err_count , ul test_type)
{
    if (!BREAK_TEST_MARK)
    {
        if (0 != err_count)
        {
            printk( KERN_ERR"test_type=0x%lX,test_fail\n", test_type);
            put_log( "test_type=0x%lX,test_fail\n", test_type );
            return    QUIT_TEST;
        }
    }
    else
    {
        if (1 == OPTIMIZE_TEST_MARK)
        {
            if (ERR_TOTAL >= ERR_MAX_COUNT)
            {
                printk( KERN_ERR"CUR_ERR_TOTAL=0x%lX,test_type=0x%lX\n", err_count, test_type);
                put_log( "CUR_ERR_TOTAL=0x%lX,test_type=0x%lX\n", err_count, test_type );
                printk( KERN_ERR"ALL_ERR_TOTAL=0x%lX\n", ERR_TOTAL);
                put_log( "ALL_ERR_TOTAL=0x%lX\n", ERR_TOTAL );
                return    QUIT_TEST;
            }
            if (err_count >= ERR_COUNT)
            {
                printk( KERN_ERR"BRK_ERR_TOTAL=0x%lX,test_type=0x%lX\n", err_count, test_type);
                put_log( "BRK_ERR_TOTAL=0x%lX,test_type=0x%lX\n", err_count, test_type );
                return    CUR_TEST_BREAK;
            }
        }
    }
    return    CUR_TES_CONTINUE;
}



/* =============================================================================
**  Function : SPRINTF_INFOR
**  description: log test info
**
** =============================================================================
*/
void SPRINTF_INFOR(ul err_count, ul test_type)
{

    if (err_count != 0)
    {
        printk( KERN_ERR"[%lu]test_type=0x%lx,FAIL\n", get_seconds(), test_type);
        put_log( "[%lu]test_type=0x%lx,FAIL\n", get_seconds(), test_type );
        printk( KERN_ERR"CUR_ERR_TOTAL=0x%lx,test_type=0x%lx\n", err_count, test_type);
        put_log( "CUR_ERR_TOTAL=0x%lx,test_type=0x%lx\n", err_count, test_type );
    }
    else
    {
        printk( KERN_ERR"[%lu]test_type=0x%lx,OK\n", get_seconds(), test_type);
        put_log( "[%lu]test_type=0x%lx,OK\n", get_seconds(), test_type );
    }
}


/* =============================================================================
**  Function : X_DDR_TEST_LOG_ERR_PATTERN_EX
**  description: log errs
**
** =============================================================================
*/
void   X_DDR_TEST_LOG_ERR_PATTERN_EX(ulv *ADDR, ul ERR_DATA, ul  BACK_DATA , ul TEST_TYPE , ul CUR_CACHE_SITE, ul STEP, ul DIE)
{
    char     error_die[][7] = {"die0_0", "die0_1", "die1_0", "die1_1", "die0_x", "die1_x"};

    printk( KERN_ERR"errInfo:e_die=%s,e_addr=0x%lx,e_data=0x%lx,b_data=0x%lx,e_type=0x%lx,cache_site=0x%lx,e_step=0x%lx\n",
            error_die[DIE], (ul)ADDR, ERR_DATA, BACK_DATA, TEST_TYPE, CUR_CACHE_SITE, STEP);
    put_log( "errInfo:e_die=%s,e_addr=0x%lx,e_data=0x%lx,b_data=0x%lx,e_type=0x%lx,cache_site=0x%lx,e_step=0x%lx\n",
             error_die[DIE], (ul)ADDR, ERR_DATA, BACK_DATA, TEST_TYPE, CUR_CACHE_SITE, STEP );
}

