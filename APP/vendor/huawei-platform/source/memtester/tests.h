/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

#ifndef DDR_TESTS_H
#define DDR_TESTS_H
/*
 * Very simple yet very effective memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2012 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 * This file contains the declarations for the functions for the actual tests,
 * called from the main routine in memtester.c.  See other comments in that
 * file.
 *
 */

/* Function declaration. */


/* PRBS测试用 */

/*lint -e10 -e49 -e101 -e132 -e620 -e749 -e750 -e752*/

#define  SEED_LEN_7    7
#define  PRBS_LEN_7   (1<<SEED_LEN_7)-1  /*lint !e620*/  /* 不重复的PRBS长度 */
#define  MASK_1         (0x00000001ul)/*lint !e620*/
#define MTEST_PRBS_DATA_7                0x00000004
#define DATA_WIDTH  32  /* 处理器位宽字节数 */

#define SEQUNCE_INC 0x11111111   /* 随机数测试修正背景数据用 */
/* 将4bit或8bit数据进行左移位拼装成32bit位宽数据，用于内存背景数据构造。2013-2-25 / z00157998 */
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

} ddr_test_error_type;/*lint !e751*/


int test_prbs_data_align_test_7(ulv* bufa, ulv* bufb, size_t count, char* log_buf, int thread_id);
int test_prbs_data_flip_test_7(ulv* bufa, ulv* bufb, size_t count, char* log_buf, int thread_id);
void err_log_printf(ulv *err_addr, ul err_data, ul b_data, ul t_type , ul step , char* log_buf, int thread_id);
int test_walking_ones(ulv* bufa, ulv* bufb, size_t count, char* log_buf, int thread_id);
int test_variation_walking_ones(ulv* bufa, ulv* bufb, size_t count, char* log_buf, int thread_id);
int test_own_addr(ulv* bufa, ulv* bufb, size_t count, char* log_buf, int thread_id);













int test_stuck_address(unsigned long volatile *bufa, size_t count, char* log_buf, int thread_id) ;
int test_random_value(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_xor_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_sub_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_mul_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_div_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_or_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_and_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_seqinc_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_solidbits_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_checkerboard_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_blockseq_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_walkbits0_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_walkbits1_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_bitspread_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_bitflip_comparison(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
#ifdef TEST_NARROW_WRITES
int test_8bit_wide_random(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
int test_16bit_wide_random(unsigned long volatile *bufa, unsigned long volatile *bufb, size_t count , char* log_buf, int thread_id);
#endif
int test_assist(unsigned long volatile * bufa, unsigned long volatile * bufb, size_t count);

#endif
/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */

