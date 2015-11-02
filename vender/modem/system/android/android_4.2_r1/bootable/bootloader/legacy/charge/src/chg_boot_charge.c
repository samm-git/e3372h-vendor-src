/******************************修改记录***************************************************************************************
  日期         修改人          问题单号            修改内容
******************************************************************************************************************************/
/******************************************************************
* Copyright (C),  2012-2018, HUAWEI Tech. Co., Ltd.
*
* File name:      chg_boot.c
*
* Description:    E5充电归一化BOOT阶段涓流充电代码
*
*

* Editer:         2012.10.29
* Version:        v1.0
*
* Function List:
*
日期               单号                    修改人            修改原因

*******************************************************************/
/******************************问题单修改记录**********************************
    日期       修改人       问题单号                 修改内容
******************************************************************************/
#include "chg_boot_charge.h"
#include "chg_boot_charge_drv.h"
#include "chg_boot_chip_platform.h"
#include "balongv7r2/preboot.h"
#include "bsp_coul.h"

/*如果在boot阶段不能使用I2C控制器需要用GPIO模拟则需要包含chg_boot_2c_virtual.h头文件*/
//#include "chg_boot_2c_virtual.h"

unsigned char g_poweroff_charger_flag = FALSE;

#define SBL1_DELAY_500MS 500
#define SBL1_DELAY_1S 1000
#define SBL1_DELAY_2S 2000
#define SBL1_DELAY_3S 3000

boolean g_is_batt_over_discharge = FALSE;
/***********************************************************************************
函 数 名  :chg_boot_set_batt_over_discharge_flag
功能描述  :设置电池是否过放标志
输入参数  :value
输出参数  :无
返 回 值  : none
注意事项  : 
***********************************************************************************/
void chg_boot_set_batt_over_discharge_flag(boolean value)
{
    g_is_batt_over_discharge = value;
}
/***********************************************************************************
函 数 名  :chg_boot_is_batt_over_discharge
功能描述  :查询电池是否过放
输入参数  :none
输出参数  :无
返 回 值  : TRUE:过放，FALSE:非过放
注意事项  : 
***********************************************************************************/
boolean chg_boot_is_batt_over_discharge(void)
{
    return g_is_batt_over_discharge;
}


/***********************************************************************************
函 数 名  :chg_boot_batt_shorted_check
功能描述  :检测电池是否短路
输入参数  :无
输出参数  :无
返 回 值  : TRUE：电池短路
            FALSE：电池未短路
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_shorted_check( void )
{
    int32_t batt_voltage = 0;
    int32_t index = 0;
	int32_t chagre_index = 0;
    int32_t tc_on = BOOT_PLUS_CHARGING_ON_TIME;    // 充电时间10s
    int32_t max_tc_on = BOOT_PLUS_CHARGING_ON_MAX_TIME;  //充电5min
    
    chg_print_level_message( CHG_MSG_INFO,"PRE-CHG: batt_shorted_check BEGIN!!!\r\n ",0,0,0 );
        /*电池过温*/
        if(TRUE == chg_boot_batt_over_temp_check())
        {
            (void)chg_boot_set_charge_enable(FALSE);
            /*停止充电显示*/
            //chg_boot_display_interface( CHG_DISP_OVER_HEATED );
            return  FALSE;
        }

		for (chagre_index = 0; chagre_index < max_tc_on; chagre_index++)
		{
            /*电池过温*/
            if(TRUE == chg_boot_batt_over_temp_check())
            {
                (void)chg_boot_set_charge_enable(FALSE);
                /*停止充电显示*/
                //chg_boot_display_interface( CHG_DISP_OVER_HEATED );
                return  FALSE;
            }
            else
            {
                (void)chg_boot_set_charge_enable(TRUE);
            }
            /*充电10s*/
            for (index = 0; index < tc_on; index++)
            {
                chg_boot_display_interface( CHG_DISP_CHARGING );
                chg_boot_delay_ms(SBL1_DELAY_500MS);
                chg_boot_display_interface( CHG_DISP_CHARGING );
                chg_boot_delay_ms(SBL1_DELAY_500MS);
                /*踢狗*/
                (void)chg_boot_set_tmr_rst();
                /*系统踢狗*/
                //BSP_WDT_Feed_Force();
                /*电池不在位*/
                if(FALSE == chg_boot_batt_presence_check())
                {
                #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
                    chg_boot_batt_unpresence_handle();
                    #else
                    //(void)chg_boot_batt_unpresence_handle();
                    return  FALSE;
                    #endif
                }
                //chg_boot_delay_ms(SBL1_DELAY_1S);  /* */
                if(FALSE == chg_boot_is_charger_present())
                {
                    /*检测过程中外电源拔除直接返回未短路，有下边的流程就行外电源不在位的拦截*/
                    chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: charger output during batt_shorted_check!!!\r\n ",0,0,0 );
                    return  FALSE;
                }
                else
                {
                    //for lint
                }
            }

            (void)chg_boot_set_charge_enable(FALSE);
            chg_boot_delay_ms(SBL1_DELAY_2S);
                
            if((FALSE == chg_boot_batt_presence_check())
            || (FALSE == chg_boot_is_charger_present()))
            {
                return  FALSE;
            }
        
            batt_voltage = chg_boot_get_batt_volt_value();
            chg_print_level_message( CHG_MSG_INFO, "PRE-CHG: IN batt_shorted_check vbat=%d\r\n ", batt_voltage, 0, 0 );

    		if(batt_voltage >= BAT_CHECK_SHORTED_VOLT)
            {
                batt_voltage = chg_boot_get_batt_volt_value();
                if(batt_voltage >= BAT_CHECK_SHORTED_VOLT)
                {
                    batt_voltage = chg_boot_get_batt_volt_value();
                    if(batt_voltage >= BAT_CHECK_SHORTED_VOLT)
                    {
                        chg_print_level_message( CHG_MSG_ERR,"PRE-CHG:batt_shorted_check: vbat=%d\n",batt_voltage,0,0);
                        return  FALSE;
                    }
                }
            }

		}

    chg_print_level_message( CHG_MSG_INFO,"PRE-CHG: batt_shorted_check END!!!\n ",0,0,0 );
    return  TRUE;
}

/***********************************************************************************
函 数 名  :chg_boot_batt_presence_check
功能描述  :检测电池是否在位
输入参数  :无
输出参数  :无
返 回 值  : TRUE：    电池在位
                  FALSE：电池不在位
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_presence_check( void )
{
    int32_t batt_temp = 0;

    batt_temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
    if(batt_temp <= BATT_NOT_PRESENCE_TEMP)
    {
        batt_temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
        if(batt_temp <= BATT_NOT_PRESENCE_TEMP)
        {
            batt_temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
            if(batt_temp <= BATT_NOT_PRESENCE_TEMP)
            {
                chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: batt_presence_check: battary unpresence!!!\r\n ",0,0,0 );
                return FALSE;
            }
        }
    }
    return TRUE;
}

/***********************************************************************************
函 数 名  :chg_boot_batt_unpresence_handle
功能描述  :检测到电池不在位时处理
输入参数  :无
输出参数  :无
返 回 值  : TRUE：    电池在位
                  FALSE：电池不在位
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/

void chg_boot_batt_unpresence_handle( void )
{
    /* 9x25平台不需要返回值 */

    for ( ; ; )
    {
        /*调用OLED/LED模块提供的接口显示进行不在位提示*/
        chg_boot_display_interface( CHG_DISP_BATTERY_BAD );
        chg_boot_delay_ms(SBL1_DELAY_500MS);
        chg_boot_display_interface( CHG_DISP_BATTERY_BAD );
        chg_boot_delay_ms(SBL1_DELAY_500MS);
        if ( TRUE == chg_boot_batt_presence_check() )
        {
            break;
        }

        //调用驱动适配层提供的外电源在位检测接口；
        if ( FALSE == chg_boot_is_charger_present() )
        {
            chg_boot_delay_ms(SBL1_DELAY_1S);
            /*2、    调用关机接口关机*/
            chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
        }
    }
    /*2、调用关机接口关机，因外电源在位会重启开机*/
    last_shutdown_reason_set( POWER_OFF_REASON_LOW_BATTERY );
    chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);

}


/***********************************************************************************
函 数 名  :chg_boot_batt_over_temp_check
功能描述  :检测电池是否过温
输入参数  :无
输出参数  :无
返 回 值  : FALSE： 电池温度正常
            TRUE：  电池过温
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
boolean chg_boot_batt_over_temp_check( void )
{
    int32_t batt_temp = 0;
    boolean rtnValue = FALSE;
    static boolean last_rtnValue = FALSE;

    batt_temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
    chg_print_level_message( CHG_MSG_ERR, "PRE-CHG: batt_temp=%d!\r\n ",batt_temp,0,0 );
    /*大于45度小于0度停充*/
    if(FALSE == last_rtnValue)
    {
        if(((batt_temp > BATT_STOP_CHARGINE_LOW_TEMP) && (batt_temp < BATT_STOP_CHARGINE_HIGH_TEMP))
            || (batt_temp <= BATT_NOT_PRESENCE_TEMP))
        {
            rtnValue = FALSE;
        }
        else
        {
            chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: batt_over_temp_check:batt over temp!\r\n ",0,0,0 );
            rtnValue = TRUE;
        }

    }
    /*大于3度小于42度复充*/
    else
    {
        if(((batt_temp > BATT_RECHARGINE_LOW_TEMP) && (batt_temp < BATT_RECHARGINE_HIGH_TEMP))
            || (batt_temp <= BATT_NOT_PRESENCE_TEMP))
        {
            chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: batt_over_temp_check:batt temp resum!\r\n ",0,0,0 );
            rtnValue = FALSE;
        }
        else
        {
            rtnValue = TRUE;
        }
    }

    last_rtnValue = rtnValue;
    return rtnValue;
}
/**********************************************************************
函 数 名      : chg_basic_init
功能描述  :  开机初始化:
                          充电芯片+双核通信
输入参数  : 无
输出参数  : 无
返 回 值      : 无
注意事项  : 在预充电部分调用
                         可根据具体平台添加删减
***********************************************************************/
void chg_basic_init( void )
{
    static int32_t s_chg_basic_init_flag = FALSE;
    chg_print_level_message(CHG_MSG_DEBUG,"CHG_TASK:chg_basic_init \n ",0,0,0 );

    if( FALSE == s_chg_basic_init_flag )
    {
        s_chg_basic_init_flag = TRUE;

/***************Note:平台相关代码，根据平台按需要添加，有的平台如V7R1需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
        /* 核间通信初始化*/
        //BSP_CHGC_Init();

        /* 充电芯片初始化*/
        (void)chg_boot_chip_init();
    }
}

/***********************************************************************************
函 数 名  :chg_boot_low_power_trickle_charg_check
功能描述  :电池短路和过放检测
输入参数  :无
输出参数  :无
返 回 值  : 电池状态
注意事项  : 本函数依赖与ADC电池电压检测模块提供的电压读取接口和驱动适配层提供的充电芯片使能接口
***********************************************************************************/
#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
/*9X25平台使用此函数接口*/
void chg_boot_low_power_trickle_charg_check(bl_shared_data_type *bl_shared_data)
#else
void chg_boot_low_power_trickle_charg_check( void )
#endif/*MBB_CHG_PLATFORM_V7R2 == NO*/
{
    int32_t batt_voltage = 0;
    int32_t wait_idx  = 0;
    int32_t trickle_charg_time_count = 0;
    boolean is_over_discharge = FALSE;

/***************Note:平台相关代码，根据平台按需要添加，有的平台如9x25需要
              移植人员根据需要，添加或者移除下边函数调用***************************/
    //判断是否是关机充电模式，如果是则进入低功耗模式
    #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_OFF)
    if (TRUE == chg_boot_is_powdown_charging())
    {
        g_poweroff_charger_flag = TRUE;
        #if 0 /*PN:dongle121, chendongguo, 2013/6/27 deleted*/
        /*进入下载模式，加载recovery 前，屏蔽其他不需加载的组件*/
        /* Iterate over config table to find appsbl entry */
        while (sbl1_config_table[index].host_img_id != NULL)
        {
            if ((sbl1_config_table[index].target_img_sec_type != SECBOOT_APPSBL_SW_TYPE ) &&
                (sbl1_config_table[index].target_img_sec_type != SECBOOT_TZ_KERNEL_SW_TYPE) &&
                (sbl1_config_table[index].target_img_sec_type != SECBOOT_WDT_SW_TYPE) &&
                (sbl1_config_table[index].target_img_sec_type != SECBOOT_RPM_FW_SW_TYPE))
            {
                sbl1_config_table[index].load = FALSE;
                index++;
                continue;
            }
            index++;
        }
        #endif
    }
    #endif

    chg_basic_init();


    /*工厂模式下直接允许开机处理*/
    /*可维可测:允许电池不在位开机*/
    if( ( TRUE == chg_boot_is_ftm_mode () ) \
        || ( TRUE == chg_boot_is_exception_poweroff_poweron_enable() ) \
        || ( TRUE == chg_boot_is_no_battery_powerup_enable() ) )
    {
        set_pw_charge_flag(FALSE);
        return ;

    }
    /*需要等待库仑计稳定后才能使用*/
    #ifdef CONFIG_COUL
    bsp_coul_ready();
    #endif
    //调用驱动适配层的外电源在位检测的接口
    if ( TRUE == chg_boot_is_charger_present() )
    {
        if ( FALSE == chg_boot_batt_presence_check() )
        {

            chg_boot_batt_unpresence_handle();

        }
        else
        {
            /*添加电池电压检测，如果电池电压大于3.0则退出检测*/
#ifdef CONFIG_COUL
            int32_t ocv_voltage = 1;
            int32_t count = 3;
            ocv_voltage = bsp_coul_ocv_get();

            if(0 == ocv_voltage)
            {
                /*电池可能过放，此时库仑计采集电压无效，需要激活处理*/
                is_over_discharge = TRUE;
                while(count--)
                {
                    chg_boot_set_charge_enable(TRUE);
                    chg_boot_delay_ms(SBL1_DELAY_3S);
                    chg_boot_set_charge_enable(FALSE);
                    chg_boot_delay_ms(SBL1_DELAY_1S);
                }
            }
            batt_voltage = chg_boot_get_batt_volt_value();
#else
            batt_voltage = chg_boot_get_batt_volt_value();
            if(0 == batt_voltage)
            {
                is_over_discharge = TRUE;
            }
#endif
            chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: charge in and vbat=%d\r\n ",batt_voltage,0,0 );
            if ( batt_voltage > BAT_CHECK_JUDGMENT_VOLT )
            {
                /* 9x25平台不需要返回值 */
                if ((batt_voltage < BAT_CHECK_POWERON_VOLT)
                    && (FALSE == chg_boot_is_powdown_charging()))
                {
                    chg_boot_display_interface( CHG_DISP_BATTERY_LOWER );

                    chg_boot_delay_ms(SBL1_DELAY_3S);
                    chg_boot_display_interface( CHG_DISP_OK );

                    chg_boot_set_powdown_charging(); //lint -e628
                }
                else
                {
                    //do nothing
                }
                return;

            }

            /*如果电池短路*/
            if ( TRUE == chg_boot_batt_shorted_check() )
            {
                /* 9x25平台不需要返回值 */

                /*1、调用OLED/LED模块提供的接口显示进行坏电池提示 */

                for(;;)
                {
                    /*调用OLED/LED模块提供的接口显示进行坏电池提示*/
                    chg_boot_display_interface( CHG_DISP_BATTERY_BAD );
                    if(FALSE == chg_boot_is_charger_present())
                    {
                        chg_boot_delay_ms(SBL1_DELAY_1S);
                        /*调用关机接口关机*/
                        chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                    }
                    else
                    {
                        chg_boot_delay_ms(SBL1_DELAY_500MS);
                    }
                }

            }
            else
            {
                if(is_over_discharge)
                {
                    chg_boot_set_batt_over_discharge_flag(TRUE);
                    chg_print_level_message(CHG_MSG_ERR, "PRE-CHG:batt is over discharge!\n",0,0,0);
                }
                #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
                if ( FALSE == chg_boot_batt_presence_check() )
                {
                    chg_boot_batt_unpresence_handle();
                }
                if (FALSE == chg_boot_is_charger_present())
                {
                    chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                }
                #endif
                #ifdef CONFIG_COUL
                /*刚进入涓充前，进行一次库仑计强制校准*/
                chg_print_level_message(CHG_MSG_ERR, "PRE-CHG:bsp_coul_cail_on COUL_CAIL_ON");
                bsp_coul_cail_on();
                bsp_coul_ready();
                #endif
                batt_voltage = chg_boot_get_batt_volt_value();
                chg_print_level_message( CHG_MSG_ERR, "PRE-CHG: Before while poll vbat=%d\r\n ", batt_voltage, 0, 0 );
                if(batt_voltage <= BAT_CHECK_JUDGMENT_VOLT)
                {
                    while(trickle_charg_time_count < BOOT_TRICKLE_CHARGE_TIME)
                    {
                        if (FALSE == chg_boot_is_charger_present())
                        {
                            /* 9x25平台不需要返回值 */
                            chg_boot_delay_ms(SBL1_DELAY_1S);
                            /* 1,调用关机接口关机*/
                            chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                        }

                        /*电池温度正常*/
                        if (FALSE == chg_boot_batt_over_temp_check())
                        {
                            /*使能充电*/
                            (void)chg_boot_set_charge_enable( TRUE );


                            /*显示充电动画*/
                            //chg_boot_display_interface( CHG_DISP_CHARGING );

                            /*充10S钟*/
                            for ( wait_idx = 0; wait_idx < BOOT_TRICKLE_CHARGING_TIME;wait_idx++)
                            {
                                //踢狗
                                (void)chg_boot_set_tmr_rst();


                                chg_boot_display_interface( CHG_DISP_CHARGING );
                                chg_boot_delay_ms(SBL1_DELAY_500MS);
                                chg_boot_display_interface( CHG_DISP_CHARGING );

                                chg_boot_delay_ms( SBL1_DELAY_500MS );  /* */
                                /*电源不在位*/
                                if (FALSE == chg_boot_is_charger_present())
                                {
                                    /* 9x25平台不需要返回值 */
                                    chg_boot_delay_ms(SBL1_DELAY_1S);
                                    //调用关机接口关机
                                    chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                                    
                                }
                                /*电池不在位*/
                                if(FALSE == chg_boot_batt_presence_check())
                                {
                                    chg_boot_batt_unpresence_handle();

                                }
                                /*涓流充电过程中电池短路检测*/
                                batt_voltage = chg_boot_get_batt_volt_value();
                                if(batt_voltage < BAT_CHECK_SHORTED_VOLT)
                                {
                                    batt_voltage = chg_boot_get_batt_volt_value();
                                    if(batt_voltage < BAT_CHECK_SHORTED_VOLT)
                                    {
                                        batt_voltage = chg_boot_get_batt_volt_value();
                                        if(batt_voltage < BAT_CHECK_SHORTED_VOLT)
                                        {
                                            chg_print_level_message(CHG_MSG_ERR,
                                                "PRE-CHG:batt_shorted_check:battary shorted\n",0,0,0);
                                            /* 9x25平台不需要返回值 */

                                            for(;;)
                                            {
                                                /*调用OLED/LED模块提供的接口显示进行坏电池提示*/
                                                chg_boot_display_interface( CHG_DISP_BATTERY_BAD );
                                                if(FALSE == chg_boot_is_charger_present())
                                                {
                                                    chg_boot_delay_ms(SBL1_DELAY_1S);
                                                    /*调用关机接口关机*/
                                                    chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                                                }
                                                else
                                                {
                                                    chg_boot_delay_ms(SBL1_DELAY_500MS);
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                            trickle_charg_time_count++;
                        }
                        /*电池温度异常*/
                        else
                        {

                            /*电池温度异常期间不计入30分钟预充电计时*/
                            /*禁止充电*/
                            (void)chg_boot_set_charge_enable(FALSE);
#if (MBB_CHG_LED == FEATURE_ON)
                            chg_boot_display_interface( CHG_DISP_BATTERY_LOWER );
#else
                            /*停止充电显示*/
                            chg_boot_display_interface( CHG_DISP_OVER_HEATED );
#endif
                            chg_boot_delay_ms(SBL1_DELAY_3S);
                        }

                        batt_voltage = chg_boot_get_batt_volt_value();
                        chg_print_level_message( CHG_MSG_ERR, "PRE-CHG: vbat=%d\r\n ", batt_voltage, 0, 0 );
                        chg_print_level_message( CHG_MSG_ERR, "PRE-CHG: charging=%d\r\n ",
                        chg_boot_is_IC_charging(),0,0 );
                        /*如果涓流充电期间电池电压大于3.0V，则跳出涓流充电进行开机处理*/
                        if(batt_voltage > BAT_CHECK_JUDGMENT_VOLT)
                        {
                            chg_print_level_message( CHG_MSG_ERR, "PRE-CHG: vbat=%d,exit pre-charging!\r\n ", batt_voltage, 0, 0 );
                            break;
                        }
                    }
                    /*涓充时间大于30分钟*/
                    if( trickle_charg_time_count >= BOOT_TRICKLE_CHARGE_TIME)
                    {
                        /*调用驱动适配层提供的接口禁止充电*/
                        (void)chg_boot_set_charge_enable(FALSE);
                        /* 9x25平台不需要返回值 */

                        for(;;)
                        {
                            /*调用OLED/LED模块提供的接口显示进行坏电池提示*/
                            chg_boot_display_interface( CHG_DISP_BATTERY_BAD );
                            if(FALSE == chg_boot_is_charger_present())
                            {
                                chg_boot_delay_ms(SBL1_DELAY_1S);
                                /*调用关机接口关机*/
                                chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);
                            }
                            else
                            {
                                chg_boot_delay_ms(SBL1_DELAY_500MS);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        //判断电池是否在位，防止NTC开路的场景单板开机；
        if ( FALSE == chg_boot_batt_presence_check() )
        {
            chg_boot_batt_unpresence_handle();
        }
        else
        {
            batt_voltage = chg_boot_get_batt_volt_value();
            chg_print_level_message( CHG_MSG_ERR,"PRE-CHG: trickle charg batt only batt_voltage=%d\r\n ",batt_voltage,0,0 );
            if(batt_voltage > BAT_CHECK_POWERON_VOLT)
            {

                return;

            }
            else
            {
                /* 9x25平台不需要返回值 */

                /*1、调用低电提醒接口进行低电提醒*/
                chg_boot_display_interface( CHG_DISP_BATTERY_LOWER );

                chg_boot_delay_ms(SBL1_DELAY_3S);

                /* 2、调用关机接口进行关机*/
                chg_boot_set_power_off(DRV_SHUTDOWN_LOW_BATTERY);

            }

        }
    }
}



