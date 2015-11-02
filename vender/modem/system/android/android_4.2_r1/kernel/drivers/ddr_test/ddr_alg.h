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
#ifndef _DDR_ALG_H_XXX_
#define _DDR_ALG_H_XXX_

#include <linux/types.h>

#define UL_LEN 32
//typedef unsigned long    size_t;
//typedef long    off_t;
typedef unsigned long ul;
typedef unsigned long long ull;
typedef unsigned long volatile ulv;
typedef unsigned char volatile u8v;
typedef unsigned short volatile u16v;

typedef unsigned long addr_t;

typedef int (*ddr_test_func)(ulv* bufa, ulv* bufb, size_t count);

struct test
{
    char *name;
    ddr_test_func fp;
};
/*
union {
    unsigned char bytes[UL_LEN/8];
    ul val;
} mword8;

union {
    unsigned short u16s[UL_LEN/16];
    ul val;
} mword16;
*/
int ddr_test_0(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_1(ulv* bufa, ulv* bufb, size_t count);
int test_prbs_data_align_test_7(ulv* bufa, ulv* bufb, size_t count);
int test_prbs_data_flip_test_7(ulv* bufa, ulv* bufb, size_t count);
int test_walking_ones(ulv* bufa, ulv* bufb, size_t count);
int test_variation_walking_ones(ulv* bufa, ulv* bufb, size_t count);
int test_own_addr(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_bit_equalizing(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_ic(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_lr(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_raw(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_sl(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_sr(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_ss(ulv* bufa, ulv* bufb, size_t count);
int ddr_test_march_g(ulv* bufa, ulv* bufb, size_t count);
void X_Q_INTERLEAVE_ADDR(ulv *addr0_0, ulv *addr0_1, ulv *addr1_0, ulv *addr1_1, ul base, ul up_down_status, ul limit);
void X_Q_INTERLEAVE_ADDR(ulv *addr0_0, ulv *addr0_1, ulv *addr1_0, ulv *addr1_1, ul base, ul up_down_status, ul limit);
void q_write_and_flush_cache_line(ulv *die0_0, ulv *die0_1, ulv *die1_0, ulv *die1_1, ul ulldata);
void Q_OFFSET_ADDR(ulv *addr0_0, ulv *addr0_1, ulv *addr1_0, ulv *addr1_1, ul up_down_status);
int x_read_and_check_cache_line(ulv *die0_0, ulv *die0_1, ulv *die1_0, ulv *die1_1, ul ulldata, ulv *error_count, ul test_type, ul step);
int BREAK_TEST(ul err_count , ul test_type);
void SPRINTF_INFOR(ul err_count, ul test_type);
void   X_DDR_TEST_LOG_ERR_PATTERN_EX(ulv *ADDR, ul ERR_DATA, ul  BACK_DATA , ul TEST_TYPE , ul CUR_CACHE_SITE, ul STEP, ul DIE);


#define BACKGROUND_DATA0                 {0x0 ,0x55555555 ,0x33333333 ,0x0F0F0F0F ,0x00FF00FF ,0x0000FFFF }
#define BACKGROUND_DATA1                 {0xFFFFFFFF ,0xAAAAAAAA ,0xCCCCCCCC ,0xF0F0F0F0 ,0xFF00FF00 ,0xFFFF0000 }


#define UP   0        /* 地址升序测试  */
#define DOWN   1        /* 地址降序测试  */


#define Q_INTERLEAVE_ADDR(base,up_down_status,limit);\
    if(up_down_status == UP)\
    {\
        addr0_0 = (ulv*)bufa;\
        addr1_0 = (ulv*)bufb;\
        addr0_1 = (ulv*)((ul)addr0_0 + (limit >>1));\
        addr1_1 = (ulv*)((ul)addr1_0 + (limit >>1));\
    }\
    else\
    {\
        addr0_1 = (ulv*)((ul)bufa + limit - CACHE_LINE_SIZE);\
        addr1_1 = (ulv*)((ul)bufb + limit - CACHE_LINE_SIZE);\
        addr0_0 = (ulv*)((ul)addr0_1 - (limit >>1));\
        addr1_0 = (ulv*)((ul)addr1_1 - (limit >>1));\
    }

/**
 * Macro for delaying for some time.
 * Note: The actual delay time is highly dependent on CPU frequency. Each
 *       iteration of the inner loop executes 4-5 instructions, and each
 *       instruction takes one cycle. 100,000,000 is hardcoded here for
 *       iterations of roughly 1 sec when CPU frequency is 400MHz or so.
 */
#define DDR_TEST_DELAY(COUNT) \
    do \
    { \
        ul i, j; \
        for (i = 0; i < (COUNT); i++) \
        { \
            for (j = 0; j < 100000000; j++) \
            { \
                i++; \
            } \
            i -= j; \
        } \
    } while (0)

/**
 * Count of delay time between entering and exiting DDR self refresh.
 */
#define DDR_TEST_SELF_REFRESH_DELAY     10

/**
 * Count of delay time between entering and exiting DDR deep power down.
 */
#define DDR_TEST_DEEP_POWER_DOWN_DELAY  10

#define DDR_TEST_MARCH_IC_DELAY     1  /* 延时秒 */

/**
  * Infinite loop
  */
#define INFINITE_LOOP() while(1)

/* BREAK_TEST_MARK = 0；满足生产测试配置，只要有错就终止测试 */
/* BREAK_TEST_MARK = 1；满足研发测试配置，只要算法测试错误数不大于阈值
即可继续测 */
//#define  BREAK_TEST_MARK   0
//#define ERR_MAX_COUNT   2000   /* 所有算法测试错误累计超出此域即终止测试 */
//#define ERR_COUNT    5    /* 局部算法测试错误累计超出此域即终止当前算法测试 */

volatile static  ul     ERR_TOTAL = 0;    /* 所有算法错误计数*/

#define  BREAK_TEST_MARK   1    /* 1 研发测试，0 生产测试 */
#define OPTIMIZE_TEST_MARK   1    /* 调优信息打印开关1打开0关闭 */
#define ERR_MAX_COUNT    10   /* 所有算法测试错误累计超出此域即终止测试 */
#define	ERR_COUNT					2				/* 局部算法测试错误累计超出此域即终止当前算法测试 */
#define	SHIFTING                12/* 移位掩码，用于标记MARCH类算法第n 组背景数据 */
#define CUR_TEST_BREAK    0x55AA
#define CUR_TES_CONTINUE   0xAA55
#define QUIT_TEST     -1

/* OPTIMIZE_TEST_MARK=1时，错误超过阈值时退出，OPTIMIZE_TEST_MARK=0
时，有错即退出 */


#define BUF_A   1   /* 内存buf 1分区 */
#define BUF_A_B   2   /* 内存buf 2分区 */
#define BUF_A_B_C  3   /* 2分区，相邻字节取反 */
#define BUF_A_B_C_D  4   /* 内存buf 4分区 */

//#define SHIFTING     10  /* 移位掩码，用于标记MARCH类算法第n 组背景数据 */



#define MEMORY_BITS_WIDE   32  /* 内存单颗粒位宽 */

#define BIT_WIDTH_32     4  /* 32位处理器采用4字节地址对齐*/
#define CACHE_LINE_SIZE        64  /* cache line大小 */
#define QWORD_NUM_IN_CACHE_LINE  16  /* 一个cache line 大小相当于16个32位CPU总线位宽 */


#define  MEM_SIZE         0x20000000  /* 单片内存容量 */


/* PRBS测试用 */

//#define SHIFTING     12    /* 移位掩码，用于标记MARCH类算法第n 组背景数据 */

#define  SEED_LEN_7    7
#define  PRBS_LEN_7   (1<<SEED_LEN_7)-1  /* 不重复的PRBS长度 */
#define  MASK_1         0x00000001
#define MTEST_PRBS_DATA_7                0x00000004
#define DATA_WIDTH  32  /* 处理器位宽字节数 */

#define SEQUNCE_INC 0x11111111   /* 随机数测试修正背景数据用 */
#define ULL_BYTE(x) (ul)((ul)x | (ul)x<<8 | (ul)x<<16 | (ul)x<<24)
#define CPU_BITS_WIDE 32


/**********************************************************************************

功能描述: 由PRBS序列生成地址线上的数据或者数据线上的数据，按照align方式

入口参数:
        UINT32 *prbs_array      序列数据
        UINT32 *paddrdata       生成的位宽数据
        UINT32 addrdata_wide    总线位宽 (地址线是从0到n-1,传入参数地址线宽为n,同理数据线)
        UINT32 ilocation        选取的序列中指定的bit位(需要从prbs sequence中的第几个bit获取数据 参数范围 1-->n)

返 回 值:
************************************************************************************/

#define make_prbs_addrdata_align(prbs_array,paddrdata,ilocation)  \
    if(((prbs_array[ilocation>>5]>>(ilocation & 0x1f))&MASK_1) == 1)\
    {\
        paddrdata = 0xffffffff;\
    }\
    else\
    {\
        paddrdata = 0;\
    }


/**********************************************************************************

功能描述: 由PRBS序列生成地址线上的数据或者数据线上的数据，按照flip方式

入口参数:
        UINT32 *prbs_array      序列数据
        UINT32 *paddrdata       生成的位宽数据
        UINT32 addrdata_wide    总线位宽 (地址线是从0到n-1,传入参数地址线宽为n,同理数据线)
        UINT32 ilocation        选取的序列中指定的bit位(需要从prbs sequence中的第几个bit获取数据 参数范围 1-->n)
        UINT32 index_line       总线索引位(索引地址线，数据线，确定哪根地址线或数据线取反 参数范围 0-->n-1)

返 回 值:
************************************************************************************/

#define make_prbs_addrdata_alignflip(prbs_array, paddrdata, ilocation, index_line) \
    if(((prbs_array[ilocation>>5]>>(ilocation & 0x1f))&MASK_1) == 1)\
    {\
        paddrdata = 0xffffffff;\
    }\
    else\
    {\
        paddrdata = 0;\
    }\
    paddrdata = (paddrdata)^(MASK_1<<(index_line & 0x1f));

/**
 * DDR test error types.
 */

typedef enum
{

    T_FAIL          = 0x0, /* 0 表示测试失败*/
    T_PASS           = 0x1, /* 1 表示测试成功 */

    DDR_TEST_ERR_MAX = 0x7FFFFFFF,  /**< Force enum to 32 bits */
    DDR_TEST_ERR_NONE        = 0x0F   , /**< No error */



    DDR_LOG_AREA_TEST     = 0x88 ,


    /* 随机数算法分支，用于区分是否为内存单元故障还是总线故障
    内存单元故障时，检测结果会透传，具有寄生效应，总线故障仅表现为单一地址空间故障。
    */
    DDR_TEST_ERR_RANDOM_XOR   = 0xF0 , /*< RANDOM_XOR */
    DDR_TEST_ERR_RANDOM_SUB   = 0xF1 , /*< RANDOM_SUB */
    DDR_TEST_ERR_RANDOM_MUL   = 0xF2 , /*< RANDOM_MUL */
    DDR_TEST_ERR_RANDOM_OR   = 0xF3 , /*< RANDOM_OR */
    DDR_TEST_ERR_RANDOM_AND   = 0xF4 , /*< RANDOM_AND */
    DDR_TEST_ERR_RANDOM_INC   = 0xF5 , /*< RANDOM_INC */


    DDR_TEST_ERR_UNATTACHED       = 0x1F   , /*< No DDR attached */


    /* 注意算法位置的编排，要与gastrMemTesters[]对应 */
    /* 生产阶段推荐算法 */
    DDR_TEST_ERR_PRBS_DATA_ALIGN   = 0x0, /*< PRBS_DATA_ALIGN  */
    DDR_TEST_ERR_PRBS_DATA_FLIP   = 0x1, /*< DATA_FLIP  */

    DDR_TEST_ERR_BIT_EQUALIZING      = 0x2,  /*< bit均衡测试 */
    DDR_TEST_ERR_MARCH_IC    = 0x3 , /*< MARCH_IC  */
    DDR_TEST_ERR_RANDOM    = 0x4 , /*< RANDOM */


    /* 强化阶段的筛选算法，可以用于研发及疑似故障定位 */
    DDR_TEST_ERR_MARCH_LA       = 0x5 ,  /*< MARCH_LA */
    DDR_TEST_ERR_MARCH_LR       = 0x6 ,  /*< MARCH_LR */
    DDR_TEST_ERR_MARCH_RAW       = 0x7 ,  /*< MARCH_RAW */
    DDR_TEST_ERR_MARCH_SL       = 0x8,  /*< MARCH_SL */
    DDR_TEST_ERR_MARCH_SR    = 0x9 , /*< MARCH_SR */
    DDR_TEST_ERR_MARCH_SS    = 0xA , /*< MARCH_SS */
    DDR_TEST_ERR_MARCH_G    = 0xB , /*< MARCH_G */
    DDR_TEST_ERR_WALKING_ONES     = 0xC ,  /*< Walking ones error */
    DDR_TEST_ERR_VARIATION_WALKING_ONES = 0xD, /*< VARIATION_WALKING_ONES */
    DDR_TEST_ERR_DEVIL_ARITHMETIC  = 0xE, /*< DEVIL_ARITHMETIC */
    DDR_TEST_ERR_MODEL_DATA   = 0xF , /*< DDR_TEST_ERR_MODEL_DATA */
    TEST_OWN_ADDR = 0x10,


} ddr_test_error_type;


#endif

