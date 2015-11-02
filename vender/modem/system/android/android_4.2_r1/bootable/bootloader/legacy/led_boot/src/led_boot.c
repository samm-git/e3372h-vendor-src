/***********************************************************************************************

                  版权所有 (C), 2013-2016, 华为终端有限公司

 ***********************************************************************************************
  文 件 名   : led_boot.c
  版 本 号   : 初稿
  生成日期   : 2014/7/23
  最近修改   :
  功能描述   : 本模块给boot 阶段提供led灯的控制亮灭接口: 
               
***********************************************************************************************/
 /*******************************修改记录开始**********************************
日期           修改人          问题单号                 修改内容
********************************修改记录结束**********************************/
#include "bsp_pmu.h"
#include <balongv7r2/gpio.h>
#include <mbb_leds.h>

#define long_time    5000000    /*dr常亮寄存器值*/

#ifdef BSP_CONFIG_BOARD_E5
#define current    4500    /*单位uA，E5的电流设为4.5mA*/
#else
#define current    3000    /*单位uA，电流设为3mA*/
#endif

int g_dr_id = 0;    /*全局变量dr控制id*/

static int strncmp(const char *s1, const char *s2, int n)
{
    if (n == 0)
    {
        return (0);
    }
   
    do {
            if (*s1 != *s2++)
            {
                return (*(unsigned char *)s1 - *(unsigned char *)--s2);
            }

            if (*s1++ == 0)
            {
                break;
            }

        } while (--n != 0);
    return (0);
}

#if(FEATURE_ON == MBB_LED_DR)
/************************************************************************
 *函数原型 ： static int change_name(const char *name)
 *描述     ： 根据操作的led名字选择dr控制id
 *输入     ：操作的led名字
 *输出     ： 无
 *返回值   ： 成功0；失败-1
 *修改历史     :
   日    期   : 20140723
   修改内容   : 新生成函数
*************************************************************************/
static int change_name(const char *name)
{
    /* reject null */
    if(NULL == name)
    {
        cprintf("[%s] name is null. \n", __func__);
        return  - 1;
    }


    if(!strncmp(name, LED_NODE_1, strlen(LED_NODE_1)))
    {
        g_dr_id = PMU_DR01;
        return 0;
    }
    else if(!strncmp(name, LED_NODE_2, strlen(LED_NODE_2)))
    {
        g_dr_id = PMU_DR02;
        return 0;
    }
    else if(!strncmp(name, LED_NODE_3, strlen(LED_NODE_3)))
    {
        g_dr_id = PMU_DR03;
        return 0;
    }
    else if(!strncmp(name, LED_NODE_4, strlen(LED_NODE_4)))
    {
        g_dr_id = PMU_DR04;
        return 0;
    }
    else if(!strncmp(name, LED_NODE_5, strlen(LED_NODE_5)))
    {
        g_dr_id = PMU_DR05;
        return 0;
    }
    else
    {
        cprintf("[%s] match name is fail. \n", __func__);
        return  -1;
    }
    
}

/************************************************************************
 *函数原型 ： static void dr_led_switch(int numb, int onoff)
 *描述     ： dr控制led灯状态切换开关
 *输入     ：对应数组下标int numb, 状态 int onoff
 *输出     ： 无
 *返回值   ： 
 *修改历史     :
   日    期   : 20140723
   修改内容   : 新生成函数
*************************************************************************/
static void dr_led_switch(int numb, int onoff)
{
        int ret = 0;
        DR_BRE_TIME dr_bre_time_st;
 
        ret = change_name(led_dr[numb].name);
        if(0 != ret)
        {
            return;
        }

        else
        {
            /* 设置呼吸灯亮度值 */
            (void)bsp_dr_current_set(g_dr_id, current, current);

            /* 设置呼吸灯时间 此处只设置常亮时间，灭由总开关控制*/
            dr_bre_time_st.bre_fall_ms = 0;
            dr_bre_time_st.bre_off_ms = 0;
            dr_bre_time_st.bre_on_ms = (unsigned int)(long_time);
            dr_bre_time_st.bre_rise_ms = 0;
            (void)bsp_dr_bre_time_set(g_dr_id, &dr_bre_time_st);

            if(PMU_DR02 < g_dr_id)
            {
                bsp_dr_start_delay_set(g_dr_id, 0);    /*dr345需要延时启动设置*/
            }

            /* 呼吸灯开关 */
            (void)bsp_dr_switch(g_dr_id, onoff);
        
        }

    return;
}
#endif

#if(FEATURE_ON == MBB_LED_GPIO)
/************************************************************************
 *函数原型 ： static void gpio_led_switch(int numb, int onoff)
 *描述     ： gpio控制led灯状态切换开关
 *输入     ：对应数组下标int numb, 状态 int onoff
 *输出     ： 无
 *返回值   ： 
 *修改历史     :
   日    期   : 20140723
   修改内容   : 新生成函数
*************************************************************************/
static void gpio_led_switch(int numb, int onoff)
{    
    (void)gpio_direction_output(gpio_exp_leds_config[numb].gpio, onoff);   
 
    return;
}
#endif

/************************************************************************
 *函数原型 ： int led_boot_status_set(char* led_name, int onoff)
 *描述     ： 提供boot阶段 的led灯控制接口
 *输入     ：要控制的led的名字, 状态 int onoff
 *输出     ： 无
 *返回值   ： 成功0；失败-1
 *修改历史     :
   日    期   : 20140723
   修改内容   : 新生成函数
*************************************************************************/
int led_boot_status_set(char* led_name, int onoff)
{
    int i = 0;
    int num = 0;
    
  
    if(NULL == led_name)
    {
        cprintf("[%s] input led_name is NULL. \n",  __func__);
        return - 1;
    }
    if((LEDS_GPIO_DEFSTATE_OFF != onoff) && (LEDS_GPIO_DEFSTATE_ON != onoff))
    {
        cprintf("[%s] input onoff = [%d] wrong. \n", __func__, onoff);
        return -1;
    }
    
#if(FEATURE_ON == MBB_LED_GPIO)
    num = (int)(ARRAY_SIZE(gpio_exp_leds_config));

    for(i = 0; i < num; i++)     /*根据name  查找对应控制的led*/
    {
        if(!strncmp(gpio_exp_leds_config[i].name , led_name , strlen(led_name) ))
        {
            gpio_led_switch( i , onoff );      /*查找成功gpio控制调用*/
            return 0;    
        }       
    }
#endif

#if(FEATURE_ON == MBB_LED_DR)
    num = (int)(ARRAY_SIZE(led_dr));

    for(i = 0; i < num; i++)     /*根据name  查找对应控制的led*/
    {
        if(!strncmp(led_dr[i].name , led_name , strlen(led_name) ))
        {
            dr_led_switch( i , onoff);      /*查找成功dr控制调用*/
            return 0;    
        }       
    }
#endif

    cprintf("[%s] input led_name no found. \n", __func__);  /*查找失败*/

return  -1;

}






