/******************************************************************************
                  版权所有 (C), 2001-2013,华为终端有限公司
 ******************************************************************************
  文 件 名   : chg_charge_drv.c
  版 本 号   : 1.0
  生成日期   : 1/29/2013
  最近修改   :
  功能描述   : BQ24192 Charge IC Driver
  函数列表   : 包含的函数名列表
******************************************************************************/

/**********************问题单修改记录******************************************
日    期              修改人         问题单号           修改内容
******************************************************************************/

 /*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "chg_boot_charge_drv.h"
#include "i2c.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

extern void chg_boot_delay_ms (uint32_t microsecond);
#if 0 /*PN:dongle121, chendongguo, 2013/6/27 deleted*/
extern int32_t bsp_i2c_read(uint8_t reg);
extern int32_t bsp_i2c_write(uint8_t reg, int32_t value);
extern void reg_huawei_gpio_out(uint32 gpio_num, uint32 pull);
#endif
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/
int32_t boot_bq24192_i2c_read(uint8_t reg, uint16_t *pval);
int32_t boot_bq24192_i2c_write(uint8_t reg, uint8_t val);
/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define bq24192_SLAVE_I2C_ADDR              (0x6B)
#define bq24192_MAX_REGISTER_CNT            (11)
#if (MBB_CHG_BQ27510 == FEATURE_ON)
#define bq27510_SLAVE_I2C_ADDR              (0x55)
#endif

/*充电初始化默认参数*/
#define CHG_DEFAULT_TERM_CURRENT            (128)
#define CHG_DEFAULT_VREG                    (4200)
#define CHG_BOOT_DEFAULT_DPM_VOLT           (4760)
//add by zhangfei
#define CHG_INPUT_LIMIT_500MA               (500)


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

static chg_chip_part_nr g_chg_chip_pn = CHG_CHIP_PN_INVALID;
 
/******************************************************************************
  Function      boot_bq24192_i2c_read
  Description   读取I2C设备指定寄存器地址的值
  Input         reg   :寄存器地址
  Output        *pval :用于将寄存器的值写入该整型数据所在地址
  Return        0     :函数执行成功
                -1    :函数执行失败
  Others        N/A
******************************************************************************/
/* BEGIN: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
int32_t boot_bq24192_i2c_read(uint8_t reg, uint16_t *pval)
{
    int32_t ret = 0;
    uint8_t chg_bq_reg = 0;
    chg_bq_reg = reg;

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    uint8_t slave_id = bq24192_SLAVE_I2C_ADDR;
    uint16_t read_data = 0;
    ret = bsp_i2c_byte_data_receive((uint8_t)slave_id, (uint8_t)chg_bq_reg, &read_data);
    if(ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "bsp_i2c_byte_data_receive fail%d\n",0,0,0);
        return -1;
    }
    else
    {
        *pval = (uint16_t)read_data;
        return 0;
    }

#else
    chg_boot_delay_ms(1);
    ret = bsp_i2c_read((uint8_t)chg_bq_reg);
    if (ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "Failed due to I2c read error ret %d\n", ret,0,0);
        return -1;
    }
    else
    {
        *pval = (uint16_t)ret;
        return 0;
    }
#endif/*MBB_CHG_PLATFORM_V7R2 == YES*/
}
/* END: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
/******************************************************************************
  Function      boot_bq24192_i2c_write
  Description   对I2C设备寄存器指定地址写入数值
  Input         reg  : 寄存器地址
                val  : 希望写入上述寄存器地址的值
  Output        N/A
  Return        0    : 函数执行成功
                -1   : 函数执行失败
  Others        N/A
******************************************************************************/
/* BEGIN: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
int32_t boot_bq24192_i2c_write(uint8_t reg, uint8_t val)
{
    int32_t ret = 0;
    uint8_t chg_bq_reg = 0;
    chg_bq_reg = reg;

#if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
    uint8_t slave_id = bq24192_SLAVE_I2C_ADDR;

    ret = bsp_i2c_byte_data_send((uint8_t)slave_id,(uint8_t)chg_bq_reg, (uint8_t)val);

    if(ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "bsp_i2c_byte_data_send fail%d\n",0,0,0);
        return -1;
    }
    else
    {
        return 0;
    }

#else
    chg_boot_delay_ms(1);
    ret = bsp_i2c_write((uint8_t)chg_bq_reg, (int32_t)val);
    if (ret < 0)
    {
        chg_print_level_message(CHG_MSG_ERR, "Failed due to I2c read error ret %d\n", ret,0,0);
        return -1;
    }
    else
    {
        return 0;
    }
#endif/*MBB_CHG_PLATFORM_V7R2 == YES*/
}
/* END: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/

/******************************************************************************
Function:       chg_boot_set_cur_level
Description:    设定充电芯片对电池的充电电流
Input:          充电电流值
Output:         N/A
Return:         TRUE     : 函数执行成功
                FALSE    : 函数执行失败
Others:         N/A
******************************************************************************/
boolean chg_boot_set_cur_level(uint32_t chg_cur)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret      = 0;
    int32_t cur_temp = 0;

    if ((BQ24192_ICHARGE_MAX < chg_cur)
        || (BQ24192_ICHARGE_OFFSET > chg_cur))
    {
        chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_cur_level Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_CHG_CUR_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING BQ24192_CHG_CUR_CTL_REG!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_cur_level() reg %d, value 0x%x before!\n", reg, reg_val,0);

    reg_val &= ~(BQ24192_ICHARGE_MASK);

    cur_temp = chg_cur - BQ24192_ICHARGE_OFFSET;
    if (CHG_CURRENT_2048MA_B7 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_2048;
        cur_temp = cur_temp - CHG_CURRENT_2048MA_B7;
    }

    if (CHG_CURRENT_1024MA_B6 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_1024;
        cur_temp = cur_temp - CHG_CURRENT_1024MA_B6;
    }

    if (CHG_CURRENT_512MA_B5 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_512;
        cur_temp = cur_temp - CHG_CURRENT_512MA_B5;
    }

    if (CHG_CURRENT_256MA_B4 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_256;
        cur_temp = cur_temp - CHG_CURRENT_256MA_B4;
    }

    if (CHG_CURRENT_128MA_B3 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_128;
        cur_temp = cur_temp - CHG_CURRENT_128MA_B3;
    }

    if (CHG_CURRENT_64MA_B2 <= cur_temp)
    {
        reg_val |= BQ24192_ICHARGE_64;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_cur_level() reg %d, value 0x%x after!\n", reg, reg_val,0);

    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGE CURRENT!\n",0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
Function:       chg_set_20pct_cur_level
Description:    设定充电芯片对电池的充电电流的20%标记
Input:          TRUE,FALSE
Output:         N/A
Return:         TRUE     : 函数执行成功
                FALSE    : 函数执行失败
Others:         BQ24192特有
******************************************************************************/
boolean chg_boot_set_20pct_cur_level(boolean is_20pct)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret      = 0;

    reg = BQ24192_CHG_CUR_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING BQ24192_CHG_CUR_CTL_REG!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_set_20pct_cur_level() reg %d, value 0x%x before!\n", reg, reg_val,0);

    reg_val &= ~(BQ24192_FORCE_20PCT_MASK);

    if (TRUE == is_20pct)
    {
        reg_val |= BQ24192_FORCE_20PCT_EN;
    }
    else
    {
        reg_val |= BQ24192_FORCE_20PCT_DIS;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_set_20pct_cur_level() reg %d, value 0x%x after!\n", reg, reg_val,0);

    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGE %20 CURRENT!\n",0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_supply_limit
  Description   设定输入电流限制
  Input         输入电流值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_supply_limit(uint32_t lmt_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;
    boolean op_result = TRUE;

    if ((CHG_IINPUT_LIMIT_3000MA < lmt_val)
        || (CHG_IINPUT_LIMIT_100MA > lmt_val))
    {
        chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_supply_limit Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_INPUT_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING BQ24192_INPUT_CTL_REG!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG,"CHG_DRV_DBG:chg_boot_set_supply_limit() reg %d, value 0x%x before!\n", 
                              reg, reg_val,0);

    /*清除寄存器原始值*/
    reg_val &= ~(BQ24192_IINLIMIT_MASK);

    /*根据输入电流值对寄存器进行配置*/
    switch (lmt_val)
    {
        case CHG_IINPUT_LIMIT_3000MA:
        {
            reg_val |= BQ24192_IINLIMIT_3000;

            break;
        }
        case CHG_IINPUT_LIMIT_2000MA:
        {
            reg_val |= BQ24192_IINLIMIT_2000;

            break;
        }
        case CHG_IINPUT_LIMIT_1500MA:
        {
            reg_val |= BQ24192_IINLIMIT_1500;

            break;
        }
        case CHG_IINPUT_LIMIT_1200MA:
        {
            reg_val |= BQ24192_IINLIMIT_1200;

            break;
        }
        case CHG_IINPUT_LIMIT_1000MA:
        {
            if (CHG_CHIP_PN_BQ24296 == g_chg_chip_pn)
            {
                reg_val |= BQ24296_IINLIMIT_1000;
            }
            else
            {
                /*IINPUT_LIMT_1000MA isN'T supported only but BQ24296, 
                  we set to 900mA for safe in this case.*/
                reg_val |= BQ24192_IINLIMIT_900;
            }
            break;
        }
        case CHG_IINPUT_LIMIT_900MA:
        {
            reg_val |= BQ24192_IINLIMIT_900;

            break;
        }
        case CHG_IINPUT_LIMIT_500MA:
        {
            reg_val |= BQ24192_IINLIMIT_500;

            break;
        }
        case CHG_IINPUT_LIMIT_150MA:
        {
            reg_val |= BQ24192_IINLIMIT_150;

            break;
        }
        default:
        {
            reg_val |= BQ24192_IINLIMIT_100;

            break;
        }

    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_supply_limit() reg %d, value 0x%x after!\n", reg, reg_val,0);

    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_DEBUG, "ERROR SETTING CHARGING SUPPLY CURRENT LIMIT!\n",0,0,0);
        return FALSE;
    }
    else
    {
        return op_result;
    }
}

/******************************************************************************
  Function      chg_boot_set_term_current
  Description   设置充电截止电流
  Input         截止电流值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_term_current(uint32_t term_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;
    int32_t term_temp = 0;

    if ((BQ24192_ITERM_MAX < term_val)
        || (BQ24192_ITERM_OFFSET > term_val))
    {
        chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_term_current Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_IPRECHG_TERM_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_term_current!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_term_current() reg %d, value 0x%x before!\n",
                            reg, reg_val,0);

    reg_val &= ~(BQ24192_ITERM_MASK);

    term_temp = term_val - BQ24192_ITERM_OFFSET;
    if (CHG_TERM_CURRENT_1024MA <= term_temp)
    {
        reg_val |= BQ24192_ITERM_1024;
        term_temp = term_temp - CHG_TERM_CURRENT_1024MA;
    }

    if (CHG_TERM_CURRENT_512MA <= term_temp)
    {
        reg_val |= BQ24192_ITERM_512;
        term_temp = term_temp - CHG_TERM_CURRENT_512MA;
    }

    if (CHG_TERM_CURRENT_256MA <= term_temp)
    {
        reg_val |= BQ24192_ITERM_256;
        term_temp = term_temp - CHG_TERM_CURRENT_256MA;
    }

    if (CHG_TERM_CURRENT_128MA <= term_temp)
    {
        reg_val |= BQ24192_ITERM_128;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_term_current() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING TERM CURRENT!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/******************************************************************************
  Function      chg_boot_set_charge_enable
  Description   设定是否使能充电
  Input         en_val:TRUE   表明使能充电
                       FALSE  表明禁止充电
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_charge_enable(boolean enable)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_POWER_ON_CFG_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_charge_enable!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_charge_enable() reg %d, value 0x%x before!\n",
                             reg, reg_val,0);

    reg_val &= ~(BQ24192_CHG_CFG_MASK);

    if (TRUE == enable)
    {
        reg_val |= BQ24192_CHG_CFG_BAT;
    }
    else
    {
        reg_val |= BQ24192_CHG_CFG_DIS;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_charge_enable() reg %d, value 0x%x after!\n",
                             reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING ENABLE VALUE!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_vreg_level
  Description   设置恒压充电电压
  Input         充电恒压电压值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_vreg_level(uint32_t vreg_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;
    int32_t vreg_temp = 0;

    if ((BQ24192_BAT_REG_VOL_MAX < vreg_val) || (BQ24192_BAT_REG_VOL_OFFSET > vreg_val))
    {
        chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_vreg_level Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_CHG_VOLT_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING BQ24192_CHG_VOLT_CTL_REG!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_vreg_level() reg %d, value 0x%x before!\n", reg, reg_val,0);

    /*将reg_val中设置电池电压的比特位清零*/
    reg_val &= ~(BQ24192_BAT_REG_VOL_MASK);

    vreg_temp = vreg_val - BQ24192_BAT_REG_VOL_OFFSET;
    if (CHG_BAT_VREG_512MV_B7 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_512;
        //mod by zhangfei
        vreg_temp = vreg_temp - CHG_BAT_VREG_512MV_B7;
    }

    if (CHG_BAT_VREG_256MV_B6 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_256;
        vreg_temp = vreg_temp - CHG_BAT_VREG_256MV_B6;
    }

    if (CHG_BAT_VREG_128MV_B5 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_128;
        vreg_temp = vreg_temp - CHG_BAT_VREG_128MV_B5;
    }

    if (CHG_BAT_VREG_64MV_B4 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_64;
        vreg_temp = vreg_temp - CHG_BAT_VREG_64MV_B4;
    }

    if (CHG_BAT_VREG_32MV_B3 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_32;
        vreg_temp = vreg_temp - CHG_BAT_VREG_32MV_B3;
    }

    if (CHG_BAT_VREG_16MV_B2 <= vreg_temp)
    {
        reg_val |= BQ24192_BAT_REG_VOL_16;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_vreg_level() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING VREG LEVEL!\n",0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_suspend_mode
  Description   设置充电IC为suspend模式
  Input         hz_mode: TRUE   表明设定充电芯片进入suspend模式
                         FALSE  表明设定芯片退出suspend模式
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_suspend_mode(boolean enable)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_INPUT_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_suspend_mode!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_suspend_mode() reg %d, value 0x%x before!\n",
                             reg, reg_val,0);

    /*清空HZ比特位*/
    reg_val &= ~(BQ24192_EN_HIZ_MASK);

    if (TRUE == enable)
    {
        reg_val |= BQ24192_EN_HIZ;
    }
    else
    {
        reg_val |= BQ24192_DIS_HIZ;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_suspend_mode() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING HZ_MODE!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_supply_prefer
  Description   设定充电芯片输入端的优先级
  Input         chg_cur: CHG_SUPPLY_PREFER_IN   表明输入优先级为IN优先
                         CHG_SUPPLY_PREFER_USB  表明输入优先级为USB优先
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_supply_prefer(chg_supply_prefer_value sup_val)
{
    //BQ24192 has no this function.
    return TRUE;
}

/******************************************************************************
  Function      chg_boot_set_te_enable
  Description   设定是否使能截止电流
  Input         chg_cur: TRUE   表明使能充电截止电流，充电电流达到设定的
                                截止值后充电就会停止
                         FALSE  表明禁止充电截止电流
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_te_enable(boolean enable)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_TERM_TIMER_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_te_enable!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_te_enable() reg %d, value 0x%x before!\n", reg, reg_val,0);

    reg_val &= ~(BQ24192_TERM_CUR_MASK);

    if(TRUE == enable)
    {
         reg_val |= BQ24192_EN_TERM_CUR;
    }
    else
    {
         reg_val |= BQ24192_FBD_TERM_CUR;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_te_enable() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING TE VALUE!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_ts_enable
  Description   设定芯片温度保护功能使能
  Input         chg_cur: TRUE   使能芯片自带的温度保护功能
                         FALSE  禁用芯片自带的温度保护功能
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_ts_enable(boolean enable)
{
    //BQ24192 has no this function.
    return TRUE;
}

/******************************************************************************
  Function      chg_boot_set_charge_mode
  Description   设定芯片是否进行涓流充电
  Input         chg_cur: TRUE   使能LOW_CHG功能，芯片以100mA小电流对
                         电池进行充电
                         FALSE  禁用LOW_CHG功能，芯片以0x05寄存器设定
                         的充电电流对电池进行充电
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_charge_mode(boolean enable)
{
    //BQ24192 has no this function.
    return TRUE;
}

/******************************************************************************
Function:       chg_boot_set_prechg_cur_level
Description:    设定充电芯片对电池的预充电电流
Input:          预充电电流值
Output:         N/A
Return:         TRUE     : 函数执行成功
                FALSE    : 函数执行失败
Others:         BQ24161特有
******************************************************************************/
boolean chg_boot_set_prechg_cur_level(uint32_t chg_cur)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;
    int32_t term_temp = 0;

    if ((BQ24192_IPRECHG_MAX < chg_cur)
        || (BQ24192_IPRECHG_OFFSET > chg_cur))
    {
        chg_print_level_message(CHG_MSG_ERR, "chg_boot_set_prechg_cur_level Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_IPRECHG_TERM_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_prechg_cur_level!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_prechg_cur_level() reg %d, value 0x%x before!\n",
                             reg, reg_val,0);

    reg_val &= ~(BQ24192_IPRECHG_MASK);
    if (CHG_CHIP_PN_BQ24196 == g_chg_chip_pn)
    {
        term_temp = chg_cur - BQ24192_IPRECHG_OFFSET;
    }
    else
    {
        term_temp = chg_cur;
    }
    if (CHG_PRECHG_CURRENT_1024MA <= term_temp)
    {
        reg_val |= BQ24192_IPRECHG_1024;
        term_temp = term_temp - CHG_PRECHG_CURRENT_1024MA;
    }

    if (CHG_PRECHG_CURRENT_512MA <= term_temp)
    {
        reg_val |= BQ24192_IPRECHG_512;
        term_temp = term_temp - CHG_PRECHG_CURRENT_512MA;
    }

    if (CHG_PRECHG_CURRENT_256MA <= term_temp)
    {
        reg_val |= BQ24192_IPRECHG_256;
        term_temp = term_temp - CHG_PRECHG_CURRENT_256MA;
    }

    if (CHG_PRECHG_CURRENT_128MA <= term_temp)
    {
        reg_val |= BQ24192_IPRECHG_128;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_prechg_cur_level() reg %d, value 0x%x after!\n",
                             reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING PRECHARGE CURRENT!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/******************************************************************************
  Function      chg_boot_set_stat_enable
  Description   设定芯片是否使能STAT比特位
  Input         chg_cur: TRUE   使能STAT比特位
                         FALSE  禁用STAT比特位
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_stat_enable(boolean enable)
{
    //BQ24192 has no this function.
    return TRUE;
}

/******************************************************************************
  Function      chg_boot_set_dpm_val
  Description   设定DPM值，即当充电器输入电压小于该设定值之后，芯片会避免
                输入电压的进一步下降(避免输入电流无限制增大导致损坏充电设备)
                从而逐渐减小甚至停止对电池进行充电，以全部电流用于给系统供电
                目前DPM设定值为4.2V
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_dpm_val(uint32_t dpm_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;
    int32_t dpm_temp = 0;

    if ((BQ24192_DPM_VOL_MAX < dpm_val) || (BQ24192_DPM_VOL_OFFSET > dpm_val))
    {
        chg_print_level_message(CHG_MSG_ERR, "Para invalid!\n", 0,0,0);
        return FALSE;
    }

    reg = BQ24192_INPUT_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_dpm_val!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_dpm_val() reg %d, value 0x%x before!\n", reg, reg_val,0);

    reg_val &= ~(BQ24192_DPM_VOL_MASK);

    dpm_temp = dpm_val - BQ24192_DPM_VOL_OFFSET;
    if (CHG_VINDPM_640MV_B6 <= dpm_temp)
    {
        reg_val |= BQ24192_DPM_VOL_640MV;
        dpm_temp = dpm_temp - CHG_VINDPM_640MV_B6;
    }

    if (CHG_VINDPM_320MV_B5 <= dpm_temp)
    {
        reg_val |= BQ24192_DPM_VOL_320MV;
        dpm_temp = dpm_temp - CHG_VINDPM_320MV_B5;
    }

    if (CHG_VINDPM_160MV_B4 <= dpm_temp)
    {
        reg_val |= BQ24192_DPM_VOL_160MV;
        dpm_temp = dpm_temp - CHG_VINDPM_160MV_B4;
    }

    if (CHG_VINDPM_80MV_B3 <= dpm_temp)
    {
        reg_val |= BQ24192_DPM_VOL_80MV;
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_dpm_val() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING DPM!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_safety_timer_enable
  Description   设定是否使能充电芯片的安全计时器功能
  Input         chg_cur: 00 (01,10) 使能安全计时器，芯片默认使能，并以27min
                         对快速充电时间进行限制(6h,9h)
                         11  禁用安全计时器
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_safety_timer_enable(chg_safety_timer_value tmr_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_TERM_TIMER_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_safety_timer_enable!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_safety_timer_enable() reg %d, value 0x%x before!\n",
                             reg, reg_val,0);

    reg_val &= ~(BQ24192_CHG_TIMER_MASK);
    reg_val &= ~(BQ24192_CHG_TIMER_EN_MASK);

    switch (tmr_val)
    {
        case CHG_5HOUR_SAFETY_TIMER:
        {
            reg_val |= BQ24192_CHG_TIMER_5HRS;
            reg_val |= BQ24192_CHG_TIMER_EN;

            break;
        }
        case CHG_8HOUR_SAFETY_TIMER:
        {
            reg_val |= BQ24192_CHG_TIMER_8HRS;
            reg_val |= BQ24192_CHG_TIMER_EN;

            break;
        }
        case CHG_12HOUR_SAFETY_TIMER:
        {
            reg_val |= BQ24192_CHG_TIMER_12HRS;
            reg_val |= BQ24192_CHG_TIMER_EN;

            break;
        }
        case CHG_20HOUR_SAFETY_TIMER:
        {
            reg_val |= BQ24192_CHG_TIMER_20HRS;
            reg_val |= BQ24192_CHG_TIMER_EN;

            break;
        }
        default:
        {
            reg_val |= BQ24192_CHG_TIMER_DIS;

            break;
        }

    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_set_safety_timer_enable() reg %d, value 0x%x after!\n",
                             reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING SAFETY TIMER!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_set_tmr_setting
  Description   设置充电芯片踢狗时间
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_tmr_setting(chg_watchdog_timer_value tmr_val)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_TERM_TIMER_CTL_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_tmr_setting!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }
    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_tmr_setting() reg %d, value 0x%x before!\n", reg, reg_val,0);

    reg_val &= ~(BQ24192_WATCHDOG_TMR_MASK);

    switch (tmr_val)
    {
        case CHG_40S_WATCHDOG_TIMER:
        {
            reg_val |= BQ24192_WATCHDOG_TMR_40;

            break;
        }
        case CHG_80S_WATCHDOG_TIMER:
        {
            reg_val |= BQ24192_WATCHDOG_TMR_80;

            break;
        }
        case CHG_160S_WATCHDOG_TIMER:
        {
            reg_val |= BQ24192_WATCHDOG_TMR_160;

            break;
        }
        default:
        {
            reg_val |= BQ24192_WATCHDOG_TMR_DIS;

            break;
        }

    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_tmr_setting() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR SETTING CHARGING WATCHDOG TIMER!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}

/******************************************************************************
  Function      chg_boot_set_tmr_rst
  Description   对充电芯片执行踢狗操作
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_tmr_rst(void)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_POWER_ON_CFG_REG;

    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_set_tmr_rst!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_tmr_rst() reg %d, value 0x%x before!\n", reg, reg_val,0);
    reg_val |= BQ24192_TMR_RST;

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_set_tmr_rst() reg %d, value 0x%x after!\n", reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR KICKING WATCHDOG!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_reset_all_reg
  Description   重置所有bq24192寄存器至默认配置
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_reset_all_reg(void)
{
    uint8_t reg     = 0;
    uint16_t reg_val = 0;
    int32_t ret     = 0;

    reg = BQ24192_POWER_ON_CFG_REG;
    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_reset_all_reg!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        /*do nothing*/
    }

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_reset_all_reg() reg %d, value 0x%x before!\n",
                             reg, reg_val,NULL);
    reg_val |= BQ24192_RESET_ALL;

    chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_reset_all_reg() reg %d, value 0x%x after!\n",
                             reg, reg_val,0);
    ret = boot_bq24192_i2c_write(reg, reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR RESETTING ALL REGISTERS!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_read_reg_dcdc
  Description   读取充电芯片所有寄存器的值，并保存在reg_val_dcdc全局变量中
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
static boolean chg_boot_read_reg_dcdc(uint8_t *reg_val_dcdc)
{
    uint8_t chg_bq_reg = 0;
    uint16_t reg_val = 0;
    int32_t ret = 0;

    if (NULL == reg_val_dcdc)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR chg_boot_read_reg_dcdc!\n");
        return FALSE;
    }
    
    /*lint -e18*/
    memset(reg_val_dcdc, 0, (sizeof(uint8_t)*bq24192_MAX_REGISTER_CNT));
    /*lint +e18*/
    ret = boot_bq24192_i2c_read(0, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_read_reg_dcdc!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        //do nothing
    }

    chg_bq_reg = BQ24192_INPUT_CTL_REG;
    for (; chg_bq_reg < bq24192_MAX_REGISTER_CNT; chg_bq_reg++)
    {
        (void)boot_bq24192_i2c_read(chg_bq_reg, &reg_val);
        reg_val_dcdc[chg_bq_reg] = reg_val;
        //del by zhangfei
        //linux平台有专门的充电IC调试接口，这里不打印避免log满天飞
        //chg_print_level_message(CHG_MSG_DEBUG, "CHG_DRV_DBG:chg_boot_read_reg_dcdc() reg %d, value 0x%x!\n",
        //        chg_bq_reg, g_reg_val_dcdc[chg_bq_reg],0);
    }
    return TRUE;
}


/******************************************************************************
  Function      chg_boot_get_IC_status
  Description   获取充电IC状态，读取充电芯片所有寄存器的值，并识别其中的只读
                比特位，写入chg_status_type结构体中，该结构体能体现所有充电芯片
                状态
  Input         N/A
  Output
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_get_IC_status(chg_status_type *chg_stat_ptr)
{
    boolean ret = FALSE;
    uint8_t reg_val_dcdc_all[bq24192_MAX_REGISTER_CNT] = {0};
    if (NULL == chg_stat_ptr)
    {
        return FALSE;
    }

    ret = chg_boot_read_reg_dcdc(reg_val_dcdc_all);
    if(FALSE == ret)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING chg_boot_get_IC_status!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        //do nothing
    }

    chg_stat_ptr->chg_vbus_stat = (chg_dcdc_vbus_stat) \
    /* array offset */
        ((reg_val_dcdc_all[8] & BQ24192_VBUS_STAT_MASK) >> BQ24192_VBUS_STAT_BITPOS);

    chg_stat_ptr->chg_chrg_stat = (chg_dcdc_chrg_stat) \
    /* array offset */
        ((reg_val_dcdc_all[8] & BQ24192_CHRG_STAT_MASK) >> BQ24192_CHRG_STAT_BITPOS);

    chg_stat_ptr->chg_watchdog_fault = (chg_dcdc_watchdog_fault) \
    /* array offset */
        ((reg_val_dcdc_all[9] & BQ24192_WATCHDOG_FLT_MASK) >> BQ24192_WATCHDOG_FLT_BITPOS);

    chg_stat_ptr->chg_chrg_fault = (chg_dcdc_chrg_fault) \
    /* array offset */
        ((reg_val_dcdc_all[9] & BQ24192_CHRG_FLT_MASK) >> BQ24192_CHRG_FLT_BITPOS);

    chg_stat_ptr->chg_bat_fault = (chg_dcdc_bat_fault) \
    /* array offset */
        ((reg_val_dcdc_all[9] & BQ24192_BAT_FLT_MASK) >> BQ24192_BAT_FLT_BITPOS);

    chg_stat_ptr->chg_ce_stat = (chg_dcdc_ce_status) \
    /* array offset */
        ((reg_val_dcdc_all[1] & BQ24192_CHG_CFG_MASK) >> BQ24192_CHG_CFG_BITPOS);

    return TRUE;
}

/******************************************************************************
  Function      chg_boot_is_charger_present
  Description   外电源是否在位
  Input         N/A
  Output        N/A
  Return        FALSE     : 外电源不在位
                TRUE      : 外电源在位
  Others        N/A
******************************************************************************/
boolean chg_boot_is_charger_present(void)
{
    chg_status_type  chg_stat = {CHG_STAT_UNKNOWN,
                                 CHG_STAT_NOT_CHARGING,
                                 CHG_WATCHDOG_NORMAL,
                                 CHG_FAULT_NORMAL,
                                 CHG_BAT_NORMAL,
                                 CHG_CONFIG_CHG_DIS};
    boolean op_result = TRUE;

    op_result = chg_boot_get_IC_status(&chg_stat);
    if (FALSE == op_result)
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR READING IC_status!\n", 0,0,0);
        return FALSE;
    }
    else
    {
        //do nothing
    }

    if (CHG_STAT_UNKNOWN == chg_stat.chg_vbus_stat)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************************************************
  Function      chg_boot_is_IC_charging
  Description   充电IC是否在充电
  Input         N/A
  Output        N/A
  Return        TRUE     : 正在充电
                FALSE    : 停止充电
  Others        N/A
******************************************************************************/
boolean chg_boot_is_IC_charging(void)
{
    chg_status_type  chg_stat = {CHG_STAT_UNKNOWN,
                                 CHG_STAT_NOT_CHARGING,
                                 CHG_WATCHDOG_NORMAL,
                                 CHG_FAULT_NORMAL,
                                 CHG_BAT_NORMAL,
                                 CHG_CONFIG_CHG_DIS};
    boolean op_result = TRUE;

    op_result = chg_boot_get_IC_status(&chg_stat);
    if (FALSE == op_result)
    {
        return FALSE;
    }
    else
    {
        //do nothing
    }

    //mod by zhangfei
    if ((CHG_STAT_PRE_CHARGING == chg_stat.chg_chrg_stat)
        || (CHG_STAT_FAST_CHARGING == chg_stat.chg_chrg_stat))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************
  Function      chg_boot_get_stop_charging_reason
  Description   获取充电停止原因
  Input         N/A
  Output        N/A
  Return        充电停止原因chg_stop_reason:
                CHG_STOP_COMPLETE,
                CHG_STOP_TIMEOUT,
                CHG_STOP_INVALID_TEMP,
                CHG_STOP_BY_SW
  Others        N/A
******************************************************************************/
chg_stop_reason chg_boot_get_stop_charging_reason(void)
{
    chg_status_type chg_stat = { CHG_STAT_UNKNOWN,
                                 CHG_STAT_NOT_CHARGING,
                                 CHG_WATCHDOG_NORMAL,
                                 CHG_FAULT_NORMAL,
                                 CHG_BAT_NORMAL,
                                 CHG_CONFIG_CHG_DIS};
    boolean op_result = TRUE;

    op_result = chg_boot_get_IC_status(&chg_stat);
    if (FALSE == op_result)
    {
        return CHG_STOP_OTHER_REASON;
    }
    else
    {
        //do nothing
    }

    //mod by zhangfei
    if (CHG_STAT_CHARGE_DONE == chg_stat.chg_chrg_stat)
    {
        return CHG_STOP_COMPLETE;
    }
    else if ((CHG_WATCHDOG_EXP == chg_stat.chg_watchdog_fault)
        || (CHG_FAULT_SAFTY_TIMER_EXP == chg_stat.chg_chrg_fault))
    {
        return CHG_STOP_TIMEOUT;
    }
    else if (CHG_FAULT_THERMAL_SHUTDOWN == chg_stat.chg_chrg_fault)
    {
        return CHG_STOP_INVALID_TEMP;
    }
    else if (CHG_FAULT_INPUT_FLT == chg_stat.chg_chrg_fault)
    {
        return CHG_STOP_SUPPLY_ERR;
    }
    else if (CHG_BAT_OVP == chg_stat.chg_bat_fault)
    {
        return CHG_STOP_BAT_ERR;
    }
    else if (CHG_CONFIG_CHG_DIS == chg_stat.chg_ce_stat)
    {
        return CHG_STOP_BY_SW;
    }
    else
    {
        chg_print_level_message(CHG_MSG_ERR, "ERROR OTHER STOP CHARGING REASON!\n", 0,0,0);
        return CHG_STOP_OTHER_REASON;
    }
}

/******************************************************************************
  Function      chg_boot_set_chip_cd
  Description   设置芯片片选使能
  Input         拉低芯片对应的CD的GPIO表示使能芯片，拉高表示关闭芯片
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_set_chip_cd(boolean enable)
{

    if(TRUE == enable)
    {
    /* Begin daizhicheng 2013-07-01 added */
    #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
        /*lint -e18*/
        //gpio_set_value(CHG_BQ24192_CD_GPIO, 1);
        gpio_direction_output(CHG_BQ24192_CD_GPIO, 1);
        /*lint +e18*/
    #else
        /*拉高确定GPIO31*/
        reg_huawei_gpio_out(CHG_BQ24192_CD_GPIO, 1);
    #endif
    }
    else
    {
    #if (MBB_CHG_PLATFORM_V7R2 == FEATURE_ON)
        //gpio_set_value(CHG_BQ24192_CD_GPIO, 0);
        gpio_direction_output(CHG_BQ24192_CD_GPIO, 0);
    #else
        /*拉低确定GPIO31*/
        reg_huawei_gpio_out(CHG_BQ24192_CD_GPIO, 0);
    #endif
    /* End daizhicheng 2013-07-01 added */
    }


    return TRUE;
}



/******************************************************************************
  Function      chg_boot_set_boost_therm_protect_threshold
  Description   设置充电芯片BOOST模式下温度保护门限值
  Input         temp      : 待设置的门限值温度, 0 表示禁用保护功能。
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        该接口目前仅BQ24296芯片支持，其他芯片直接返回TRUE.
******************************************************************************/
boolean chg_boot_set_boost_therm_protect_threshold(int32_t temp)
{
    uint8_t  reg         = 0;
    uint16_t reg_val     = 0;
    int32_t  ret         = 0;
    int32_t temp_in     = temp;

    if (CHG_CHIP_PN_BQ24296 != g_chg_chip_pn)
    {
        chg_print_level_message(CHG_MSG_INFO, 
            "Boost thermal protect isN'T supported on %s[%d] charger IC.\n",
            CHG_CHIP_PN_BQ24196 == g_chg_chip_pn ? "BQ24196" : "UNKNOWN",
            (uint8_t)g_chg_chip_pn);
        return TRUE;
    }

    /*BCOLD和BHOT门限设置在不同的寄存器，先判断要操作哪个寄存器*/
    if (temp_in >= 0)
    {
        reg = BQ24x96_BOOSTV_THERMREG_REG;
        chg_print_level_message(CHG_MSG_DEBUG,
            "Temp[%d'C] to set >= 0, Reg[0x%02x] selected.\n",
            temp_in, BQ24x96_BOOSTV_THERMREG_REG);

        ret = boot_bq24192_i2c_read(reg, &reg_val);
        if (0 != ret)
        {
            chg_print_level_message(CHG_MSG_ERR, 
                "Read BOOST_THERMPRT_REG[0x06] error with 0x%x!\n", ret, 0, 0);
            return FALSE;
        }

        chg_print_level_message(CHG_MSG_DEBUG, 
            "CHG_DRV_DBG:chg_boot_set_boost_therm_protect_threshold() " \
            "reg %d, value 0x%x before!\n", reg, reg_val,0);
        
        reg_val &= ~(BQ24296_BHOT_MASK);
        if (0 == temp_in)
        {
            /*Disable Boost Mode Thermal Protect Function.*/
            reg_val |= BQ24296_BHOT_DISABLE;

            chg_print_level_message(CHG_MSG_DEBUG,
                "Boost Mode Thermal Protection Disabled.\n", 0, 0, 0);
        }
        else
        {
            if (temp_in >= CHG_BHOT_THOLD_PLUS65DEGC)
            {
                reg_val |= BQ24296_BHOT2_65DEGC;
            }
            else if (temp_in >= CHG_BHOT_THOLD_PLUS60DEGC)
            {
                reg_val |= BQ24296_BHOT0_60DEGC;
            }
            else if (temp_in >= CHG_BHOT_THOLD_PLUS55DEGC)
            {                
                reg_val |= BQ24296_BHOT1_55DEGC;
            }
            else
            {
                chg_print_level_message(CHG_MSG_ERR,
                    "Invalid BHOT THERM_PRT_TH %d.\n", temp_in, 0, 0);

                reg_val |= BQ24296_BHOT1_55DEGC;
                return FALSE;
            }
        }

        ret = boot_bq24192_i2c_write(reg, (uint8_t)reg_val);
        if (0 != ret)
        {
            chg_print_level_message(CHG_MSG_ERR, 
                "Write BOOST_THERMPRT_REG[0x06] error with 0x%x!\n", ret, 0, 0);
            return FALSE;
        }

        chg_print_level_message(CHG_MSG_DEBUG, 
            "CHG_DRV_DBG:chg_boot_set_boost_therm_protect_threshold() " \
            "reg[0x%02x], value 0x%x after!\n", reg, reg_val,0);
    }
    else
    {
        reg = BQ24192_CHG_CUR_CTL_REG;
        chg_print_level_message(CHG_MSG_DEBUG,
            "Temp[%d'C] to set < 0, Reg[0x%02x] selected.\n",
            temp_in, BQ24192_CHG_CUR_CTL_REG);

        ret = boot_bq24192_i2c_read(reg, &reg_val);
        if (0 != ret)
        {
            chg_print_level_message(CHG_MSG_ERR, 
                "Read Boost BCOLD REG[0x02] error with 0x%x!\n", ret, 0, 0);
            return FALSE;
        }

        chg_print_level_message(CHG_MSG_DEBUG, 
            "CHG_DRV_DBG:chg_boot_set_boost_therm_protect_threshold() " \
            "reg[0x%02x], value 0x%x before!\n", reg, reg_val,0);
        
        reg_val &= ~(BQ24296_BCOLD_MASK);
        if (temp_in <= CHG_BCOLD_THOLD_MINUS20DEGC)
        {
            reg_val |= BQ24296_BCOLD1_N20DEGC;
        }
        else if (temp_in <= CHG_BCOLD_THOLD_MINUS10DEGC)
        {
            reg_val |= BQ24296_BCOLD0_N10DEGC;
        }
        else
        {
            chg_print_level_message(CHG_MSG_ERR,
                "Invalid BCOLD THERM_PRT_TH %d.\n", temp_in, 0, 0);
        
            reg_val |= BQ24296_BCOLD0_N10DEGC;
            return FALSE;
        }

        ret = boot_bq24192_i2c_write(reg, (uint8_t)reg_val);
        if (0 != ret)
        {
            chg_print_level_message(CHG_MSG_ERR, 
                "Write Boost BCOLD REG[0x02] error with 0x%x!\n", ret, 0, 0);
            return FALSE;
        }

        chg_print_level_message(CHG_MSG_DEBUG, 
            "CHG_DRV_DBG:chg_boot_set_boost_therm_protect_threshold() " \
            "reg[0x%02x], value 0x%x after!\n", reg, reg_val,0);
    }

    return TRUE;
}

/******************************************************************************
  Function      chg_boot_set_boost_volt
  Description   设置充电芯片BOOST模式电压值
  Input         待设置的Boost模式电压值
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        该接口目前仅BQ24296芯片支持，其他芯片返回TRUE.
******************************************************************************/
boolean chg_boot_set_boost_volt(uint32_t boostv)
{
    uint8_t  reg         = 0;
    uint16_t reg_val     = 0;
    int32_t  ret         = 0;
    uint32_t boostv_left = 0;

    if (CHG_CHIP_PN_BQ24296 != g_chg_chip_pn)
    {
        chg_print_level_message(CHG_MSG_INFO, 
            "BoostV set isN'T supported on %s[%d] charger IC.\n",
            CHG_CHIP_PN_BQ24196 == g_chg_chip_pn ? "BQ24196" : "UNKNOWN",
            (uint8_t)g_chg_chip_pn);
        return TRUE;
    }

    reg = BQ24x96_BOOSTV_THERMREG_REG;
    ret = boot_bq24192_i2c_read(reg, &reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, 
            "Read BOOSTV_REG[0x06] error with 0x%x!\n", ret, 0, 0);
        return FALSE;
    }

    chg_print_level_message(CHG_MSG_DEBUG, 
        "CHG_DRV_DBG:chg_boot_set_boost_volt() reg[0x%02x], value 0x%x before!\n", 
        reg, reg_val,0);

    reg_val &= ~(BQ24296_BOOSTV_MASK);

    if (boostv >= BQ24296_BOOSTV_MAX)
    {
        reg_val |= (BQ24296_BOOSTV_512MV | BQ24296_BOOSTV_256MV
                  | BQ24296_BOOSTV_128MV | BQ24296_BOOSTV_64MV);
    }
    else if (boostv <= BQ24296_BOOSTV_OFFSET)
    {
        /*NOP*/
        reg_val = reg_val;
    }
    else
    {
        boostv_left = boostv - BQ24296_BOOSTV_OFFSET;
        if (boostv_left >= CHG_BOOSTV_OFFSET_512MV)
        {
            reg_val |= BQ24296_BOOSTV_512MV;            
            boostv_left -= CHG_BOOSTV_OFFSET_512MV;
        }

        if (boostv_left >= CHG_BOOSTV_OFFSET_256MV)
        {
            reg_val |= BQ24296_BOOSTV_256MV;
            boostv_left -= CHG_BOOSTV_OFFSET_256MV;
        }
        
        if (boostv_left >= CHG_BOOSTV_OFFSET_128MV)
        {
            reg_val |= BQ24296_BOOSTV_128MV;
            boostv_left -= CHG_BOOSTV_OFFSET_128MV;
        }

        if (boostv_left >= CHG_BOOSTV_OFFSET_64MV)
        {
            reg_val |= BQ24296_BOOSTV_64MV;
        }
    }

    ret = boot_bq24192_i2c_write(reg, (uint8_t)reg_val);
    if (0 != ret)
    {
        chg_print_level_message(CHG_MSG_ERR, 
            "Write BOOSTV_REG[0x06] error with 0x%x!\n", ret, 0, 0);
        return FALSE;
    }

    chg_print_level_message(CHG_MSG_DEBUG, 
        "CHG_DRV_DBG:chg_boot_set_boost_volt() reg[0x%02x], value 0x%x after!\n", 
        reg, reg_val,0);

    return TRUE;
}

/******************************************************************************
  Function      chg_boot_get_part_nr
  Description   获取充电芯片的型号，目前支持bq24196与bq24296
  Input         N/A
  Output        pn        : 返回的芯片型号
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_get_part_nr(chg_chip_part_nr *pn)
{
    uint8_t  reg     = 0;
    uint16_t reg_val = 0;
    int32_t  ret     = 0;
    
    if (NULL == pn)
    {
        chg_print_level_message(CHG_MSG_ERR, 
            "chg_boot_get_part_nr: pn arg is NULL.\n", 0, 0, 0);
        return FALSE;
    }
    
    if (CHG_CHIP_PN_INVALID != g_chg_chip_pn)
    {
        *pn = g_chg_chip_pn;
        return TRUE;
    }
    else
    {
        reg = BQ24x96_REVISION_REG;
        ret = boot_bq24192_i2c_read(reg, &reg_val);
        if (0 != ret)
        {
            chg_print_level_message(CHG_MSG_ERR, 
                "Get Charge IC PN Error with 0x%x!\n", ret, 0, 0);
            return FALSE;
        }
        else
        {
            /*First, Check whether We have BQ24196 charger IC.*/
            if (BQ24196_PART_NR_VALUE == (reg_val & BQ2419x_PART_NR_MASK))
            {
                chg_print_level_message(CHG_MSG_INFO,
                    "BQ24196 Charger IC attached.\n", 0, 0, 0);
                g_chg_chip_pn = CHG_CHIP_PN_BQ24196;
            }
            /*Then, check BQ24296.*/
            else if (BQ24296_PART_NR_VALUE == (reg_val & BQ2429x_PART_NR_MASK))
            {
                chg_print_level_message(CHG_MSG_INFO,
                    "BQ24296 Charge IC attached.\n", 0, 0, 0);
                g_chg_chip_pn = CHG_CHIP_PN_BQ24296;
            }
            else
            {
                chg_print_level_message(CHG_MSG_ERR,
                    "Unknown Charge IC. Reg[0x0A] = 0x%x.\n", reg_val, 0, 0);                
                return FALSE;
            }
        }
    }
    
    *pn = g_chg_chip_pn;
    return TRUE;
}

/******************************************************************************
  Function      chg_boot_chip_init
  Description   充电芯片初始化
  Input         N/A
  Output        N/A
  Return        TRUE      : 函数执行成功
                FALSE     : 函数执行失败
  Others        N/A
******************************************************************************/
boolean chg_boot_chip_init(void)
{
    int32_t ret_code = 0;
    static int s_chg_chip_init_flag = 0;


    /*在所有对充电IC操作之前，先判断当前的充电IC型号。*/
    chg_chip_part_nr cur_chip_pn = CHG_CHIP_PN_INVALID;

    chg_print_level_message(CHG_MSG_ERR, "CHG:chg_boot_chip_init \n ",0,0,0);

    if(0 == s_chg_chip_init_flag)
    {
        s_chg_chip_init_flag = 1;
    }
    else
    {
        return TRUE;
    }
    /* BEGIN: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
    /*boot阶段的I2C注册*/
    if(0 != bsp_i2c_slave_register(I2C_MASTER0,bq24192_SLAVE_I2C_ADDR))
    {
        return FALSE;
    }
    /* END: PN:dongle121,chendongguo, 2013/6/26 added/deleted/modified*/
	#if (MBB_CHG_BQ27510 == FEATURE_ON)
	if(0 != bsp_i2c_slave_register(I2C_MASTER0,bq27510_SLAVE_I2C_ADDR))
    {
        return FALSE;
    }
	#endif


    /*在所有对充电IC操作之前，先获取当前的充电IC型号。*/
    if (FALSE == chg_boot_get_part_nr(&cur_chip_pn))
    {
        /*If error, treat as bq24196 as default.*/
        chg_print_level_message(CHG_MSG_ERR,
            "Get Charge IC PN error, treat as BQ24196.\n", 0, 0, 0);
        g_chg_chip_pn = CHG_CHIP_PN_BQ24196;
    }

    /*打开充电芯片CD*/
    if (FALSE == chg_boot_set_chip_cd(TRUE))
    {
        /*If error occured, set 0 bit of ret_code.*/
        ret_code |= (1 << 0);
    }
    /*关闭充电使能*/
    //chg_boot_set_charge_enable(FALSE);
    else if (FALSE == chg_boot_set_charge_enable(FALSE))
    {
        /*If error occured, set 1 bit of ret_code.*/
        ret_code |= (1 << 1);
    }
    /*设置充电电流*/
    //chg_boot_set_cur_level(CHG_CURRENT_SENSE_OFFSET);
    //mod by zhangfei
    else if (FALSE == chg_boot_set_cur_level(BQ24192_ICHARGE_OFFSET))
    {
        /*If error occured, set 2 bit of ret_code.*/
        ret_code |= (1 << 2);
    }
    /*设置输入限流*/
    //chg_boot_set_supply_limit(CHG_INPUT_LIMIT_1500MA);
    //mod by zhangfei
    else if (FALSE == chg_boot_set_supply_limit(CHG_INPUT_LIMIT_500MA))
    {
        /*If error occured, set 3 bit of ret_code.*/
        ret_code |= (1 << 3);
    }
    /*设置截止电流*/
    //chg_boot_set_term_current(CHG_DEFAULT_TERM_CURRENT);
    else if (FALSE == chg_boot_set_term_current(CHG_DEFAULT_TERM_CURRENT))
    {
        /*If error occured, set 4 bit of ret_code.*/
        ret_code |= (1 << 4);
    }
    /*设置恒压电压*/
    //chg_boot_set_vreg_level(CHG_DEFAULT_VREG);
    else if (FALSE == chg_boot_set_vreg_level(CHG_DEFAULT_VREG))
    {
        /*If error occured, set 5 bit of ret_code.*/
        ret_code |= (1 << 5);
    }
    /*设置DPM电压值*/
    //chg_boot_set_dpm_val(CHG_DPM_VOLT_OFFSET);
    else if (FALSE == chg_boot_set_dpm_val(CHG_BOOT_DEFAULT_DPM_VOLT))
    {
        /*If error occured, set 6 bit of ret_code.*/
        ret_code |= (1 << 6);
    }
    /*设置为非suspend模式*/
    //chg_boot_set_suspend_mode(FALSE);
    else if (FALSE == chg_boot_set_suspend_mode(FALSE))
    {
        /*If error occured, set 7 bit of ret_code.*/
        ret_code |= (1 << 7);
    }
    /*设置截止电流使能*/
    //chg_boot_set_te_enable(TRUE);
    else if (FALSE == chg_boot_set_te_enable(TRUE))
    {
        /*If error occured, set 8 bit of ret_code.*/
        ret_code |= (1 << 8);
    }
    /*关闭芯片温度保护功能*/
    //chg_boot_set_ts_enable(FALSE);
    else if (FALSE == chg_boot_set_ts_enable(FALSE))
    {
        /*If error occured, set 9 bit of ret_code.*/
        ret_code |= (1 << 9);
    }
    /*设置STAT使能.*/
    //chg_boot_set_stat_enable(TRUE);
    else if (FALSE == chg_boot_set_stat_enable(TRUE))
    {
        /*If error occured, set 10 bit of ret_code.*/
        ret_code |= (1 << 10);
    }
    /*设置LOW_CHG不使能，即芯片对电池以正常配置的充电电流进行充电.*/
    //chg_boot_set_charge_mode(FALSE);
    else if (FALSE == chg_boot_set_charge_mode(FALSE))
    {
        /*If error occured, set 11 bit of ret_code.*/
        ret_code |= (1 << 11);
    }
    /*关闭安全定时器功能，需要时可以设置打开*/
    //chg_boot_set_safety_timer_enable(CHG_DISABLE_SAFETY_TIMER);
    else if (FALSE == chg_boot_set_safety_timer_enable(CHG_DISABLE_SAFETY_TIMER))
    {
        /*If error occured, set 12 bit of ret_code.*/
        ret_code |= (1 << 12);
    }
    /*暂时先关闭wathdog*/
    else if (FALSE == chg_boot_set_tmr_setting(CHG_DISABLE_WATCHDOG_TIMER))
    {
        /*If error occured, set 13 bit of ret_code.*/
        ret_code |= (1 << 13);
    }
    /*对充电芯片进行踢狗操作.*/
    //chg_boot_set_tmr_rst();
    else if (FALSE == chg_boot_set_tmr_rst())
    {
        /*If error occured, set 14 bit of ret_code.*/
        ret_code |= (1 << 14);
    }

    //add by ligang
    else if (FALSE == chg_boot_set_prechg_cur_level(CHG_PRECHG_CURRENT_256MA))
    {
        /*If error occured, set 14 bit of ret_code.*/
        ret_code |= (1 << 15);
    }
    else if (FALSE == chg_boot_set_boost_therm_protect_threshold(FALSE))
    {
        /*If error occured, set 16 bit of ret_code.*/
        ret_code |= (1 << 16);
    }
    else
    {
        chg_print_level_message(CHG_MSG_ERR, "CHARGE INIT SUCCESS!\n", 0,0,0);
    }

    if (0 != ret_code)
    {
        chg_print_level_message(CHG_MSG_ERR, "CHARGE INIT: ERRNO %d!\n", ret_code,0,0);
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
//end of file
