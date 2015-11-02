/******************************************************************************

                 版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : soc_timer_interface.h
  版 本 号   : 初稿
  作    者   : Excel2Code
  生成日期   : 2012-06-05 10:07:59
  最近修改   :
  功能描述   : 接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年6月5日
    作    者   : m53479
    修改内容   : 从《Hi6620V100SOC寄存器手册_TIMER.xml》自动生成

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_TIMER_INTERFACE_H__
#define __SOC_TIMER_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_TIMER
 ****************************************************************************/
/* 寄存器说明：TIMERn计数初值寄存器，保存着定时器的计数初值。
            当定时器处于周期模式且计数值递减到0时，将TIMERn_LOAD寄存器的值重新载入计数器。当直接写本寄存器时，在被TIMCLKENn使能的下一个TIMCLK的上升沿，定时器当前的计数器将更新为写入值。
            TIMERn_LOAD的最小有效值为1，如果往TIMERn_LOAD寄存器写0，将立即产生定时中断。
            当向TIMERn_BGLOAD寄存器写入值时，TIMERn_LOAD寄存器的值也会被覆盖，但定时器计数的当前值不会受到影响。
            若在被TIMCLKENn使能的TIMCLK信号的上升沿到来之前，向TIMERn_BGLOAD寄存器和TIMERn_LOAD寄存器都写入值，则在被TIMCLKENn使能的TIMCLK信号的下一个上升沿，定时计数器的值首先更新为TIMERn_LOAD寄存器的写入值。此后，每当计数器递减到0时，其值重载为TIMERn_BGLOAD寄存器与TIMERn_LOAD寄存器中最后被写入时的写入值。
            在分别对TIMERn_BGLOAD寄存器和TIMERn_LOAD寄存器进行了两次写入之后，读TIMERn_LOAD寄存器返回的值为TIMERn_BGLOAD寄存器的写入值，即为周期模式下计数器第2次递减到0时载入的有效值。
   详      述：Timern的计数初值。
   UNION 结构：无 */
#define SOC_TIMER_TIMERN_LOAD_ADDR(base, t)           ((base) + 0x000)

/* 寄存器说明：TIMERn当前计数值寄存器。
            当向TIMERn_LOAD寄存器的写操作发生后，TIMERn_VALUE寄存器在PCLK时钟域立刻反映出计数器的新载入值，无须等到下一个被TIMCLKENn使能的TIMCLK时钟沿。
   详      述：正在递减的TIMERn的当前值。
               <B>注意：当定时器处于16位模式时，32位的TIMERnValue寄存器的高16位并未被自动设为0。若该定时器之前处于32位模式，并且自进入16位模式后TIMERnLoad寄存器从未被写过，则TIMERnValue寄存器的高16位可能具有非零值。</B>
   UNION 结构：无 */
#define SOC_TIMER_TIMERN_VALUE_ADDR(base, t)          ((base) + 0x004)

/* 寄存器说明：定时器控制寄存器，用于控制定时器的操作模式及中断的产生。
   位域定义UNION结构:  SOC_TIMER_TIMERN_CONTROL_UNION */
#define SOC_TIMER_TIMERN_CONTROL_ADDR(base, t)        ((base) + 0x008)

/* 寄存器说明：中断清除寄存器，对本寄存器的任何写操作都会清除Timer定时器的中断。
   位域定义UNION结构:  SOC_TIMER_TIMERN_INTCLR_UNION */
#define SOC_TIMER_TIMERN_INTCLR_ADDR(base, t)         ((base) + 0x00C)

/* 寄存器说明：原始中断状态寄存器。
   位域定义UNION结构:  SOC_TIMER_TIMERN_RIS_UNION */
#define SOC_TIMER_TIMERN_RIS_ADDR(base, t)            ((base) + 0x010)





/*****************************************************************************
  3 枚举定义
*****************************************************************************/



/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/



/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/



/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_TIMER
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_TIMER_TIMERN_CONTROL_UNION
 结构说明  : TIMERN_CONTROL 寄存器结构定义。地址偏移量:0x008＋0x020×(t)，初值:0x00000000，宽度:32
 寄存器说明: 定时器控制寄存器，用于控制定时器的操作模式及中断的产生。
*****************************************************************************/
typedef union
{
    unsigned int      value;
    struct
    {
         unsigned int  timeren   : 1;  /* bit[0]   : 定时器使能。
                                                    0：禁止；
                                                    1：使能。 */
        unsigned int  oneshot   : 1;  /* bit[1]   : 选择计数模式为单次计数模式还是周期计数模式。
                                                    0：自由运行模式；
                                                    1：周期计数模式。 */
        unsigned int  timermask : 1;  /* bit[2]   : 选择是否屏蔽计数器中断。
                                                    0：不屏蔽计数器中断；
                                                    1：屏蔽计数器中断；*/
         unsigned int  reserved : 1;  /* bit[3]   : 保留。 */
          unsigned int  timern_en_ack : 1;  /* bit[4]   : 只读，使能配置反馈标志
		                                                  1计时使能配置生效，
		                                                  0计时使能配置未生效，*/
          
          unsigned int  reserved_1: 26; /* bit[5-31]: 保留。 */
    } reg;
} SOC_TIMER_TIMERN_CONTROL_UNION;
#define SOC_TIMER_TIMERN_CONTROL_oneshot_START    (3)
#define SOC_TIMER_TIMERN_CONTROL_oneshot_END      (3)
#define SOC_TIMER_TIMERN_CONTROL_timersize_START  (3)
#define SOC_TIMER_TIMERN_CONTROL_timersize_END    (3)
#define SOC_TIMER_TIMERN_CONTROL_timerpre_START   (3)
#define SOC_TIMER_TIMERN_CONTROL_timerpre_END     (3)
#define SOC_TIMER_TIMERN_CONTROL_intenable_START  (2)
#define SOC_TIMER_TIMERN_CONTROL_intenable_END    (2)
#define SOC_TIMER_TIMERN_CONTROL_timermode_START  (1)
#define SOC_TIMER_TIMERN_CONTROL_timermode_END    (1)
#define SOC_TIMER_TIMERN_CONTROL_timeren_START    (0)
#define SOC_TIMER_TIMERN_CONTROL_timeren_END      (0)


/*****************************************************************************
 结构名    : SOC_TIMER_TIMERN_INTCLR_UNION
 结构说明  : TIMERN_INTCLR 寄存器结构定义。地址偏移量:0x00C＋0x020×(t)，初值:0x00000000，宽度:32
 寄存器说明: 中断清除寄存器，对本寄存器的任何写操作都会清除Timer定时器的中断。
*****************************************************************************/
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  timern_intclr : 1;  /* bit[0]   : 中断清除寄存器位。
                                                        写1清除中断，写0无影响。 */
        unsigned int  timer0_intclr : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_TIMER_TIMERN_INTCLR_UNION;
#define SOC_TIMER_TIMERN_INTCLR_timern_intclr_START  (0)
#define SOC_TIMER_TIMERN_INTCLR_timern_intclr_END    (0)
#define SOC_TIMER_TIMERN_INTCLR_timer0_intclr_START  (1)
#define SOC_TIMER_TIMERN_INTCLR_timer0_intclr_END    (31)


/*****************************************************************************
 结构名    : SOC_TIMER_TIMERN_RIS_UNION
 结构说明  : TIMERN_RIS 寄存器结构定义。地址偏移量:0x010＋0x020×(t)，初值:0x00000000，宽度:32
 寄存器说明: 原始中断状态寄存器。
*****************************************************************************/
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  timernris : 1;  /* bit[0]   : 读时返回计数器的原始中断状态。
                                                    0：未产生原始中断；
                                                    1：已产生原始中断。 */
        unsigned int  reserved  : 31; /* bit[1-31]: 保留。写入无效，读时返回0。 */
    } reg;
} SOC_TIMER_TIMERN_RIS_UNION;
#define SOC_TIMER_TIMERN_RIS_timernris_START  (0)
#define SOC_TIMER_TIMERN_RIS_timernris_END    (0)


/*****************************************************************************
 结构名    : SOC_TIMER_TIMERN_MIS_UNION
 结构说明  : TIMERN_MIS 寄存器结构定义。地址偏移量:0x014＋0x020×(t)，初值:0x00000000，宽度:32
 寄存器说明: 屏蔽后中断状态寄存器。
*****************************************************************************/
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  timernmis : 1;  /* bit[0]   : 读时返回屏蔽后的计数器中断状态。
                                                    0：未产生中断；
                                                    1：已产生中断。 */
        unsigned int  reserved  : 31; /* bit[1-31]: 保留。 */
    } reg;
} SOC_TIMER_TIMERN_MIS_UNION;
#define SOC_TIMER_TIMERN_MIS_timernmis_START  (0)
#define SOC_TIMER_TIMERN_MIS_timernmis_END    (0)






/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_timer_interface.h */
