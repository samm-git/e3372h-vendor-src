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
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/power_supply.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/notifier.h>
#include "huawei-charger.h"
#include "chg_config.h"
//#include "platform/v7r2/chg_chip_platform.h"
#include "chg_charge_api.h"
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include <linux/ioctl.h>
/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
#ifdef CONFIG_MBB_FAST_ON_OFF
extern struct blocking_notifier_head g_fast_on_off_notifier_list;
#endif
#if (MBB_CHG_EXTCHG == FEATURE_ON)
extern int32_t g_ui_choose_exchg_mode;
#endif/*defined(MBB_CHG_EXTCHG)*/

extern chg_stm_state_info_t chg_stm_state_info;

extern CHG_SHUTOFF_VOLT_PROTECT_NV_TYPE   g_chgBattVoltProtect;      //NV50386
/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
/* 接受USB各种事件用工作队列*/
struct workqueue_struct *mbb_chg_wq = NULL;

struct mbb_chg_work_stuct
{
    struct work_struct event_work;          /* 接受USB各种事件用处理任务*/
    unsigned long event;                    /* 事件类型*/
    int chg_type;                           /* 事件内容*/
};

struct mbb_chg_work_stuct mbb_chg_event_work;

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
/*=======================/dev/charger IOCTL控制命令字 BEGIN================*/
/*===================新增特性命令必须按照此定义方式递增====================*/
#define CHG_CMD_BASE 0X88
/*通知开始补电*/
#define CHG_CMD                         _IOWR(CHG_CMD_BASE, 0, unsigned long)
/*查询是否需要补电*/
#define CHG_INQUIRE_CMD                 _IOWR(CHG_CMD_BASE, 1, unsigned long)
/*查询电池电压实时值*/
#define INQUIRE_VOLTAGE_CMD             _IOWR(CHG_CMD_BASE, 2, unsigned long)
/*查询校准后的电池电压*/
#define INQUIRE_CALIBRATION_VOLTAGE_CMD _IOWR(CHG_CMD_BASE, 3, unsigned long)
/*查询当前是否充电*/
#define UI_INQUIRE_BATT_STATE           _IOWR(CHG_CMD_BASE, 4, unsigned long)
/*查询充电器类型*/
#define UI_INQUIRE_CHARGER_TYPE         _IOWR(CHG_CMD_BASE, 5, unsigned long)
/*PT工位使能充电*/
#define INQUIRE_START_CHG_TASK_CMD      _IOWR(CHG_CMD_BASE, 6, unsigned long)
/*禁止本次对外充电*/
#define UI_DIABLE_EXTCHG_ONCE           _IOWR(CHG_CMD_BASE, 7, unsigned long)
/*开启本次对外充电*/
#define UI_ENABLE_EXTCHG_ONCE           _IOWR(CHG_CMD_BASE, 8, unsigned long)
/*查询是否支持库伦计*/
#define INQUIRE_COULOMETER_SUPPORT_CMD  _IOWR(CHG_CMD_BASE, 9, unsigned long)
/*=======================/dev/charger IOCTL控制命令字 END==================*/

#define CHG_EVENT_WAKE_TIMEOUT            (6000)
#define CHG_EVENT_FTM_WAKE_TIMEOUT        (1000)

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
//#define pr_fmt(fmt) "%s: " fmt, __func__
/*默认打印ERR级别LOG*/
int32_t g_huawei_drv_debug = 1;

static struct class *g_charger_class = NULL;

struct i2c_client *g_i2c_client = NULL;
struct chargeIC_chip *g_chip = NULL;

/*BQ24196的I2C寄存器地址*/
#define I2C_CHARGER_IC_ADDR  0x6B

static enum power_supply_property battery_monitor_props[] = {
    POWER_SUPPLY_PROP_STATUS,
    POWER_SUPPLY_PROP_HEALTH,
    POWER_SUPPLY_PROP_PRESENT,
    POWER_SUPPLY_PROP_TECHNOLOGY,
    POWER_SUPPLY_PROP_VOLTAGE_AVG,
    POWER_SUPPLY_PROP_CAPACITY,
    POWER_SUPPLY_PROP_TEMP,
    POWER_SUPPLY_PROP_TIME_TO_FULL_AVG,
};

static enum power_supply_property battery_monitor_usb_props[] = {
    POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property battery_monitor_ac_props[] = {
    POWER_SUPPLY_PROP_ONLINE,
};

static enum power_supply_property battery_monitor_extchg_props[] = {
    POWER_SUPPLY_PROP_ONLINE,
    POWER_SUPPLY_PROP_EXTCHG_STA,
};
static enum power_supply_property battery_monitor_wireless_props[] = {
    POWER_SUPPLY_PROP_ONLINE,
};

/*===========================================================================
Function:       battery_monitor_get_property
Description:    battery
Input:          struct power_supply *psy
        enum power_supply_property psp
        union power_supply_propval *val
Return:         0 :successed
                other:failed
===========================================================================*/
static int battery_monitor_get_property(struct power_supply *psy,
                    enum power_supply_property psp,
                    union power_supply_propval *val)
{
    struct chargeIC_chip *chip = NULL;
    chip = container_of(psy, struct chargeIC_chip, bat);

    switch (psp)
    {
        case POWER_SUPPLY_PROP_STATUS:
            val->intval = chip->bat_stat;
            break;

        case POWER_SUPPLY_PROP_VOLTAGE_AVG:
            val->intval = chip->bat_avg_voltage;
            break;

        case POWER_SUPPLY_PROP_TEMP:
            val->intval = chip->bat_avg_temp;
            break;

        case POWER_SUPPLY_PROP_PRESENT:
            val->intval = chip->bat_present;
            break;
        case POWER_SUPPLY_PROP_HEALTH:
            val->intval = chip->bat_health;
            break;
        case POWER_SUPPLY_PROP_CAPACITY:
            val->intval = chip->bat_capacity;
            break;
        case POWER_SUPPLY_PROP_TECHNOLOGY:
            val->intval = chip->bat_technology;
            break;
        case POWER_SUPPLY_PROP_TIME_TO_FULL_AVG:
            val->intval = chip->bat_time_to_full;
            break;

        default:
            dev_err(&chip->client->dev, "%s defualt run.\n", __func__);
            return - EINVAL;
    }

    return 0;
}

/*===========================================================================
Function:       battery_monitor_ac_get_property
Description:    ac
Input:          struct power_supply *psy
        enum power_supply_property psp
        union power_supply_propval *val
Return:         0 :successed
                other:failed
===========================================================================*/
static int battery_monitor_ac_get_property(struct power_supply *psy,
                    enum power_supply_property psp,
                    union power_supply_propval *val)
{
    struct chargeIC_chip *chip = NULL;
    chip = container_of(psy, struct chargeIC_chip, ac);

    switch (psp)
    {
        case POWER_SUPPLY_PROP_ONLINE:
            val->intval = chip->ac_online;
            break;
        default:
            return - EINVAL;
    }

    return 0;
}

/*===========================================================================
Function:       battery_monitor_usb_get_property
Description:    usb
Input:          struct power_supply *psy
        enum power_supply_property psp
        union power_supply_propval *val
Return:         0 :successed
                other:failed
===========================================================================*/
static int battery_monitor_usb_get_property(struct power_supply *psy,
                    enum power_supply_property psp,
                    union power_supply_propval *val)
{
    struct chargeIC_chip *chip = NULL;
    chip = container_of(psy, struct chargeIC_chip, usb);

    switch (psp)
    {
        case POWER_SUPPLY_PROP_ONLINE:
            val->intval = chip->usb_online;
            break;
        default:
            return - EINVAL;
    }

    return 0;
}

/*===========================================================================
Function:       battery_monitor_extchg_get_property
Description:    extchg
Input:          struct power_supply *psy
        enum power_supply_property psp
        union power_supply_propval *val
Return:         0 :successed
                other:failed
===========================================================================*/
static int battery_monitor_extchg_get_property(struct power_supply *psy,
                    enum power_supply_property psp,
                    union power_supply_propval *val)
{
    struct chargeIC_chip *chip = NULL;
    chip = container_of(psy, struct chargeIC_chip, extchg);

    switch (psp)
    {
        case POWER_SUPPLY_PROP_ONLINE:
            val->intval = chip->extchg_online;
            break;
        case POWER_SUPPLY_PROP_EXTCHG_STA:
            val->intval = chip->extchg_status;
            break;
        default:
            return - EINVAL;
    }

    return 0;
}

/*===========================================================================
Function:       battery_monitor_wireless_get_property
Description:    wireless
Input:          struct power_supply *psy
        enum power_supply_property psp
        union power_supply_propval *val
Return:         0 :successed
                other:failed
===========================================================================*/
static int battery_monitor_wireless_get_property(struct power_supply *psy,
                    enum power_supply_property psp,
                    union power_supply_propval *val)
{
    struct chargeIC_chip *chip = NULL;
    chip = container_of(psy, struct chargeIC_chip, wireless);

    switch (psp)
    {
        case POWER_SUPPLY_PROP_ONLINE:
            val->intval = chip->wireless_online;
            break;
        default:
            return - EINVAL;
    }

    return 0;
}

/*******************************函数实现***************************************/


/******************************************************************************
  Function      chargeIC_i2c_read
  Description   读取I2C设备指定寄存器地址的值
  Input         reg   :寄存器地址
  Output        *pval :用于将寄存器的值写入该整型数据所在地址
  Return        0     :函数执行成功
                -1    :函数执行失败
  Others        N/A
******************************************************************************/
static int chargeIC_i2c_read(uint8_t reg, uint8_t *pval)
{
    int ret = 0;

    if(NULL == g_i2c_client)
    {
        pr_err("%s failed due to g_i2c_client doesn't exist@~\n", __FUNCTION__);
        return -1;
    }


    ret = i2c_smbus_read_byte_data(g_i2c_client, reg);

    if(ret < 0)
    {
        pr_err("%s fail to i2c_smbus_read_byte_data reg=0x%02X, ret=%d\n",
            __FUNCTION__, reg, ret);
        return -1;
    }
    else
    {
        *pval = ret;
        pr_info("read(0x%02X)=0x%02X\n", reg, *pval);
        return 0;
    }
}

/******************************************************************************
  Function      chargeIC_i2c_write
  Description   对I2C设备寄存器指定地址写入数值
  Input         reg  : 寄存器地址
                val  : 希望写入上述寄存器地址的值
  Output        N/A
  Return        0    : 函数执行成功
                -1   : 函数执行失败
  Others        N/A
******************************************************************************/
static int chargeIC_i2c_write(uint8_t reg, uint8_t val)
{
    int32_t ret = 0;
    if(NULL == g_i2c_client)
    {
        pr_err("%s failed due to g_i2c_client doesn't exist@~\n", __FUNCTION__);
        return -1;
    }


    ret = i2c_smbus_write_byte_data(g_i2c_client, reg, val);
    if(0 == ret)
    {
        pr_info("write(0x%02X)=0x%02X\n", reg, val);
        return 0;
    }
    else
    {
        pr_err("%s fail to i2c_smbus_write_byte_data reg=0x%02X, ret=%d\n",
            __FUNCTION__, reg, ret);
        return -1;
    }
}

/******************************************************************************
  Function      chargeIC_convert_atoi
  Description   将字符数组中字符'0'至字符'9'组成的数据段转换为整型
  Input         *name: 需要进行转换的字符数组的地址
  Output        N/A
  Return        val  : 将转换后生成的整形变量返回
  Others        N/A
******************************************************************************/
static int chargeIC_convert_atoi(char *name)
{
    int val = 0;

    for (;; name++)
    {
        switch (*name)
        {
            case '0' ... '9':
            val = ATOI_CONVERT_NUM * val + (*name - '0');
            break;
            default:
            return val;
        }
    }
}

/******************************************************************************
  Function      chargeIC_proc_read
  Description   通过proc文件系统读取BQ24161设备所有寄存器的值
  Input         *filp  : 函数指向的文件节点
  Output        *buffer: 将读取到的I2C寄存器的值拷贝到用户空间的分配的空间
  Return        0      : 函数执行成功
                -1     : 函数执行失败
  Others        N/A
******************************************************************************/
static ssize_t chargeIC_proc_read(struct file *filp,
    char *buffer, size_t length, loff_t *offset)
{
    int ret  = 0;
    int ret2 = 0;
    int pval = 0;

    ret = chargeIC_i2c_read(0, (uint8_t *)&pval); //reg num
    pr_info("REG 0x00 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(1, (uint8_t *)&pval); //reg num
    pr_info("REG 0x01 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(2,(uint8_t *)&pval); //reg num
    pr_info("REG 0x02 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(3,(uint8_t *)&pval); //reg num
    pr_info("REG 0x03 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(4,(uint8_t *)&pval); //reg num
    pr_info("REG 0x04 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(5,(uint8_t *)&pval); //reg num
    pr_info("REG 0x05 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(6,(uint8_t *)&pval); //reg num
    pr_info("REG 0x06 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(7,(uint8_t *)&pval); //reg num
    pr_info("REG 0x07 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(8,(uint8_t *)&pval); //reg num
    pr_info("REG 0x08 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(9,(uint8_t *)&pval); //reg num
    pr_info("REG 0x09 gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }

    ret = chargeIC_i2c_read(10,(uint8_t *)&pval); //reg num
    pr_info("REG 0x0a gets pval %d\n", pval);
    ret2 = copy_to_user(buffer, (uint8_t *)&pval, sizeof(pval));
    if(0 != ret || 0 != ret2)
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
    }


    return (ssize_t)ret;
}

/******************************************************************************
  Function      chargeIC_proc_write
  Description   通过proc文件系统向指定BQ24161寄存器地址写入值
  Input         *filp  : 函数指向的文件节点
                *buffer: 用户空间传入的字符数组指针
  Output        N/A
  Return        0      : 函数执行成功
                -1     : 函数执行失败
  Others        N/A
******************************************************************************/
static ssize_t chargeIC_proc_write(struct file *filp,
    const char *buff, size_t len, loff_t *off)
{
    uint8_t reg = 0;

    uint8_t val = 0;

    int32_t ret = 0;

    char tmp_buff[ATOI_MAX_LENGTH] = {0};

    char Reg_val[ATOI_MAX_LENGTH] = {0};

    char Reg_Num[ATOI_MAX_LENGTH] = {0};

    size_t cnt = ATOI_REG_VALUE_CNT;

    ret = copy_from_user(tmp_buff,buff,strlen(buff));
    if (ret < 0)
    {
        pr_err("chargeIC_proc_write fail to copy from usr!\n");
    }

    Reg_Num[ATOI_REG_ADDR_CNT] = tmp_buff[ATOI_REG_ADDR_CNT];

    for(; cnt < len; cnt++)
    {
        Reg_val[cnt - ATOI_REG_VALUE_CNT] = tmp_buff[cnt];
    }

    reg = (uint8_t)chargeIC_convert_atoi(Reg_Num);

    val = (uint8_t)chargeIC_convert_atoi(Reg_val);

    /*write to i2c register*/
    ret = chargeIC_i2c_write(reg, val);
    if (0 != ret)
    {
        pr_err("<1> ERROR write to i2c register %d !\n",reg);
    }
    /*write successfully*/
    pr_info("Write to chargeIC REGISTER %d successfully \n", reg);
    pr_info("The Value of this REGISTER is %d\n", val);

    return len;
}
#if (FEATURE_ON == MBB_HWTEST)
#define TEST_REG    0x03
#define TEST_REG_VAL    0x55
int chg_is_chargeIC_communication_normal(void)
{
    int ret = -1;
    uint8_t reg = 0;

    ret = chargeIC_i2c_write(TEST_REG, TEST_REG_VAL);
    if (0 != ret)
    {
        pr_err("<1> ERROR write to i2c register %d !\n",TEST_REG);
        return FALSE;
    }
    ret = chargeIC_i2c_read(TEST_REG,(uint8_t *)&reg); 
    if(0 != ret )
    {
        pr_err("KERN_ERR CHG_DEBUG: READ chargeIC Registers Failed!\n");
        return FALSE;
    }
    pr_err("CHG_TEST: reg:%d,reg_val:0x%02X\n",TEST_REG,reg);
    if(TEST_REG_VAL == reg)
    {   
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}
#endif
/******************************************************************************
  Function      dload_chg_proc_read
  Description   通过proc文件系统读取电池电压格数
  Input         *filp  : 函数指向的文件节点
  Output        *buffer: 将读取到的I2C寄存器的值拷贝到用户空间的分配的空间
  Return        0      : 函数执行成功
                -1     : 函数执行失败
  Others        N/A
******************************************************************************/
static ssize_t dload_chg_proc_read(struct file *filp,
    char *buffer, size_t length, loff_t *offset)
{
    int32_t batt_volt_value = 0;
    int len = 0;
    BATT_LEVEL_ENUM bat_volt_level = BATT_LEVEL_MAX;

    batt_volt_value = chg_get_batt_volt_value();

    pr_info("CHG_DEBUG: dload_chg_proc_read batt_volt %d!\n", batt_volt_value);

    if(batt_volt_value < g_chgBattVoltProtect.VbatLevelLow_MAX)
    {
        bat_volt_level = BATT_LOW_POWER;
    }

    else if(batt_volt_value < g_chgBattVoltProtect.VbatLevel1_MAX)
    {
        bat_volt_level = BATT_LEVEL_1;
    }
    else if(batt_volt_value < g_chgBattVoltProtect.VbatLevel2_MAX)
    {
        bat_volt_level = BATT_LEVEL_2;
    }
    else if(batt_volt_value < g_chgBattVoltProtect.VbatLevel3_MAX)
    {
        bat_volt_level = BATT_LEVEL_3;
    }
    else
    {
        bat_volt_level = BATT_LEVEL_4;
    }

    pr_info("CHG_DEBUG: dload_chg_proc_read bat_volt_level %d!\n", bat_volt_level);

    //ret = copy_to_user(buffer, (uint32_t*)&bat_volt_level, sizeof(uint32_t));
    len = snprintf(buffer, sizeof(bat_volt_level), "%d", bat_volt_level);
    pr_info("CHG_DEBUG: dload_chg_proc_read len %d\n", len);

    if(0 == len)
    {
        pr_err("CHG_DEBUG: READ dload chg Failed!\n");
    }

    return (ssize_t)length;
}

/******************************************************************************
  Function      poweron_chg_proc_read
  Description   通过proc文件系统读取电池电压格数
  Input         *filp  : 函数指向的文件节点
  Output        *buffer: 将读取到的I2C寄存器的值拷贝到用户空间的分配的空间
  Return        0      : 函数执行成功
                -1     : 函数执行失败
  Others        N/A
******************************************************************************/
static ssize_t poweron_chg_proc_read(struct file *filp,
    char *buffer, size_t length, loff_t *offset)
{
    int len = 0;
    boolean is_powdown_charging = FALSE;
    int start_mode = DRV_START_MODE_NORMAL;

    is_powdown_charging = chg_is_powdown_charging();
    pr_info("CHG_DEBUG: poweron_chg_proc_read poweron state %d!\n", is_powdown_charging);
    if (TRUE == is_powdown_charging)
    {
        start_mode = DRV_START_MODE_CHARGING;
    }
    else
    {
        start_mode = DRV_START_MODE_NORMAL;
    }

    len = copy_to_user(buffer, (void *)&start_mode, sizeof(int));

    if(0 != len)
    {
        pr_err("CHG_DEBUG: READ poweron chg Failed!\n");
    }

    return (ssize_t)len;
}

static struct file_operations chargeIC_proc_ops = {
    .read  = chargeIC_proc_read,
    .write = chargeIC_proc_write,
};

static struct file_operations dload_chg_proc_ops = {
    .read  = dload_chg_proc_read,
};

static struct file_operations poweron_chg_proc_ops = {
    .read  = poweron_chg_proc_read,
};

/******************************************************************************
  Function      create_chargeIC_proc_file
  Description   创建chargeIC的proc文件节点
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static void create_chargeIC_proc_file(void)
{
    struct proc_dir_entry *proc_file = NULL;
    /*创建文件节点，并设置节点权限为644*/
    proc_file = proc_create(CHARGEIC_PROC_FILE, 0644, NULL,&chargeIC_proc_ops);

    if(NULL == proc_file)

    {
        pr_err("%s: create proc entry for chargeIC failed\n", __FUNCTION__);
    }
}

/******************************************************************************
  Function      create_dload_chg_proc_file
  Description   创建dload_chg的proc文件节点
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static void create_dload_chg_proc_file(void)
{
    struct proc_dir_entry *proc_file = NULL;
    /*创建文件节点，并设置节点权限为644*/
    proc_file = proc_create(DLOAD_CHG_PROC_FILE, 0644, NULL,&dload_chg_proc_ops);

    if(NULL == proc_file)

    {
        pr_err("%s: create proc entry for dload_chg failed\n", __FUNCTION__);
    }
}

/******************************************************************************
  Function      create_poweron_chg_proc_file
  Description   创建poweron的proc文件节点
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static void create_poweron_chg_proc_file(void)
{
    struct proc_dir_entry *proc_file = NULL;
    /*创建文件节点，并设置节点权限为644*/
    proc_file = proc_create(POWERON_CHG_PROC_FILE, 0644, NULL,&poweron_chg_proc_ops);

    if(NULL == proc_file)

    {
        pr_err("%s: create proc entry for poweron failed\n", __FUNCTION__);
    }
}

/******************************************************************************
*  Function:  charge_event_work
*  Description: 充电模块的消息处理work，用于接受处理各种插入拔出事件
*  Called by  :
*  Input      : struct work_struct *w
*  Output     : None
*  Return     : None
*  Note       : 接收USB那边发过来的消息
*  History    : 	1. Created by x00202188.(2013-11-05)
********************************************************************************/
static void charge_event_work(struct work_struct *w)
{
#if ( FEATURE_ON == MBB_MLOG)
    static unsigned long old_plug = 0xFFFFFF;
    static int old_chg_type = -1;

    if ((old_plug == mbb_chg_event_work.event) &&
        (old_chg_type == mbb_chg_event_work.chg_type))
    {
        pr_err("[MBB CHG]%s: plug event report same plug=%d,chg_type=%d!!!\r\n",
            __func__,old_plug,old_chg_type);
        return;
    }
    old_plug = mbb_chg_event_work.event;
    old_chg_type = mbb_chg_event_work.chg_type;
#endif
    pr_err("[MBB CHG]%s :%ld %d.\r\n", __func__, mbb_chg_event_work.event, mbb_chg_event_work.chg_type);
    get_chgr_type_from_usb(mbb_chg_event_work.event, mbb_chg_event_work.chg_type);
}
#if (MBB_CHG_EXTCHG == FEATURE_ON)
/******************************************************************************
*  Function:  chg_extchg_monitor_work
*  Description: 对外充电监控任务
*  Called by  :
*  Input      : struct work_struct *work
*  Output     : None
*  Return     : None
*  Note       : 对外充电状态监控
********************************************************************************/
static void chg_extchg_monitor_work(struct work_struct *work)
{
/*
    struct chargeIC_chip *chip = container_of(work,
				struct chargeIC_chip,
				extchg_monitor_work.work);
*/
    /*对外充电监控*/
    chg_extchg_monitor_func();

}
#endif

#if 0
static int battery_monitor_charger_event(struct notifier_block *nb, unsigned long event,
                void *_data)
{
    int ret = 0;

    mbb_chg_event_work.event = event;
    mbb_chg_event_work.chg_type = *((int *)_data);
    pr_err("[MBB CHG]%s :%ld %d.\r\n", __func__, mbb_chg_event_work.event, mbb_chg_event_work.chg_type);
    /*由于事件通知可能在中断上下文，因此将工作任务加入到工作队列中处理*/
    wake_lock_timeout(&g_chip->alarm_wake_lock, (long)msecs_to_jiffies(CHG_EVENT_WAKE_TIMEOUT));
    queue_work(mbb_chg_wq, &(mbb_chg_event_work.event_work));

    return ret;
}
#endif

int battery_monitor_blocking_notifier_call_chain(unsigned long val, unsigned long v)
{
    if (NULL == g_chip)
    {
        return -1;
    }
    
    //return blocking_notifier_call_chain(&g_chip->notifier_list_bat, val,(void *)&v);

    mbb_chg_event_work.event = (int)val;
    mbb_chg_event_work.chg_type = (int)v;
    pr_err("[MBB CHG]%s :%ld %d.\r\n", __func__, mbb_chg_event_work.event, mbb_chg_event_work.chg_type);
    /*由于事件通知可能在中断上下文，因此将工作任务加入到工作队列中处理*/
    if (FALSE == chg_is_ftm_mode())
    {
        /*如果不是工厂模式，超时锁为6s*/
        wake_lock_timeout(&g_chip->alarm_wake_lock, (long)msecs_to_jiffies(CHG_EVENT_WAKE_TIMEOUT));
        pr_info("CHG_DEBUG: wake lock timeout is 6s!\n");
    }
    else
    {
        /*如果是工厂模式，超时锁为1s*/
        wake_lock_timeout(&g_chip->alarm_wake_lock, (long)msecs_to_jiffies(CHG_EVENT_FTM_WAKE_TIMEOUT));
        pr_info("CHG_DEBUG: wake lock timeout is 1s!\n");
    }
    queue_work(mbb_chg_wq, &(mbb_chg_event_work.event_work));
    
    return 0;
}

EXPORT_SYMBOL_GPL(battery_monitor_blocking_notifier_call_chain);

#if 0
int battery_monitor_register_notifier(struct chargeIC_chip *chip,
        struct notifier_block *nb)
{
    if ((NULL == chip) || (NULL == nb))
    {
        return -1;
    }

    return blocking_notifier_chain_register(&chip->notifier_list_bat, nb);
}
EXPORT_SYMBOL_GPL(battery_monitor_register_notifier);

int battery_monitor_unregister_notifier(struct chargeIC_chip *chip,
        struct notifier_block *nb)
{
    if ((NULL == chip) || (NULL == nb))
    {
        return -1;
    }

    return blocking_notifier_chain_unregister(&chip->notifier_list_bat, nb);
}
EXPORT_SYMBOL_GPL(battery_monitor_unregister_notifier);
#endif

#ifdef CONFIG_MBB_FAST_ON_OFF
static int suspend_monitor_charger_event(struct notifier_block *nb,
                                            unsigned long val, void *data)
{
    int ret = 0;
    pr_info("%s +++suspend_monitor_charger_event+++ %ld \n", __func__, val);
    chg_get_system_suspend_status(val);
    return ret;
}

static struct notifier_block suspend_monitor_charger_block = {
    .notifier_call = suspend_monitor_charger_event
};
#endif/*CONFIG_MBB_FAST_ON_OFF*/

/******************************************************************************
  Function      chg_fact_enable_chg_supplement
  Description   启动补电任务
  Input         N/A
  Output        N/A
  Return        TRUE   : 成功
                FALSE : 失败
  Others        N/A
******************************************************************************/
boolean chg_fact_enable_chg_supplement(void)
{
    boolean ret = FALSE;

    ret = chg_tbat_chg_sply();

    return ret;
}

/******************************************************************************
  Function      chargerdev_read
  Description   目前未使用该函数
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static int   chargerdev_read (struct file *file, char __user *buf,
    size_t count,loff_t *offset)
{
    return 0;
}

/******************************************************************************
  Function      chargerdev_write
  Description   目前未使用该函数
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static int  chargerdev_write (struct file *file, const char __user *buf,
            size_t count,loff_t *offset)
{
    return 0;
}

/******************************************************************************
  Function      chargerdev_ioctl
  Description   给UI提供的查询电池格数、开关机状态、是否充电的接口。
                通过读取chg_info全局变量的值，可以获取当前电池格数以及是否充电
                通过SMEM 可以获取关机充电状态
  Input         N/A
  Output        N/A
  Return        N/A
  Others        N/A
******************************************************************************/
static long chargerdev_ioctl(struct file *file, unsigned int cmd,
    unsigned long arg)
{
    boolean batt_state = FALSE;
    boolean batt_fact_chg = 0;
    boolean batt_coulometer_support = NO;
    chg_chgr_type_t cur_chgr_type = CHG_CHGR_INVALID;
    int32_t batt_volt = 0;
    unsigned long ret = 0;

    switch(cmd)
    {
        /*0:开始补电*/
        case CHG_CMD:
        {
            batt_fact_chg = chg_fact_enable_chg_supplement();

            ret = copy_to_user((void*)arg, &batt_fact_chg, sizeof(batt_fact_chg));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot start fact_chg!\n");

                return -1;
            }

            break;
        }
        /*1:查询是否需要补电*/
        case CHG_INQUIRE_CMD:
        {
            batt_fact_chg = chg_tbat_status_get();

            ret = copy_to_user((void*)arg, &batt_fact_chg, sizeof(batt_fact_chg));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain fact_chg state!\n");

                return -1;
            }

            break;
        }
        /*2:查询电池电压实时值*/
        case INQUIRE_VOLTAGE_CMD:
        {
            batt_volt = (int32_t)chg_get_volt_from_adc(CHG_PARAMETER__BATTERY_VOLT);

            ret = copy_to_user((void*)arg, &batt_volt, sizeof(batt_volt));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain orig bat_volt!\n");

                return -1;
            }

            break;
        }
        /*3:查询校准后的电池电压*/
        case INQUIRE_CALIBRATION_VOLTAGE_CMD:
        {
            batt_volt = chg_get_batt_volt_value();

            ret = copy_to_user((void*)arg, &batt_volt, sizeof(batt_volt));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain bat_volt!\n");

                return -1;
            }

            break;
        }
         /*4:查询当前是否充电*/
        case UI_INQUIRE_BATT_STATE:
        {
            if(TRUE == chg_get_charging_status())
            {
                batt_state = TRUE;
            }
            else
            {
                batt_state = FALSE;
            }

            ret = copy_to_user((void*)arg, &batt_state, sizeof(batt_state));
            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain batt_state!\n");

                return -1;
            }

            break;
        }
         /*5:查询充电器类型*/
        case UI_INQUIRE_CHARGER_TYPE:
        {
            cur_chgr_type = chg_stm_get_chgr_type();

            ret = copy_to_user((void*)arg, &cur_chgr_type, sizeof(cur_chgr_type));
            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain cur_chgr_type!\n");

                return -1;
            }

            break;
        }
        /*6:PT工位使能充电*/
        case INQUIRE_START_CHG_TASK_CMD:
        {
            chg_pt_mmi_test_proc();
            //set 0 as success flag
            batt_fact_chg = 0;
            ret = copy_to_user((void*)arg, &batt_fact_chg, sizeof(batt_fact_chg));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot start chg task!\n");

                return -1;
            }

            break;
        }
#if (MBB_CHG_EXTCHG == FEATURE_ON)
        /* 7:禁止本次对外充电*/
        case UI_DIABLE_EXTCHG_ONCE:
        {
            printk("CHG_DEBUG: UI_DIABLE_EXTCHG_ONCE!\n");
            //chg_set_extchg_chg_enable(FALSE);
            /*禁止对外充电输出*/
            chg_set_charge_otg_enable(FALSE);
            /*EN1=1, EN2=0 防止对外充电设置停止本次充电后放到无线充电器上拔出USB ID线后无
                 硬件中断*/
#if (FEATURE_ON != MBB_CHG_PLATFORM_V7R2)
            extchg_gpio_control(EXTCHG_CONTROL_GPIO_EN1,TRUE); /* */
            extchg_gpio_control(EXTCHG_CONTROL_GPIO_EN2,FALSE); /* */
#endif
            chg_stm_state_info.extchg_status = POWER_SUPPLY_EXTCHGSTA_UNKNOWN;
            chg_send_stat_to_app((uint32_t)DEVICE_ID_EXTCHG,(uint32_t)CHG_EVENT_NONEED_CARE);
            break;
        }
        /*8:开启本次对外充电*/
        case UI_ENABLE_EXTCHG_ONCE:
        {
            printk("CHG_DEBUG: UI_ENABLE_EXTCHG!\n");
            chg_set_extchg_chg_enable(TRUE);
            chg_stm_state_info.extchg_status = POWER_SUPPLY_EXTCHGSTA_START_CHARGING;
            chg_send_stat_to_app((uint32_t)DEVICE_ID_EXTCHG,(uint32_t)CHG_EVENT_NONEED_CARE);
            ret = copy_from_user(&g_ui_choose_exchg_mode, (void __user *)arg, sizeof(int32_t));
            printk("CHG_DEBUG: g_ui_choose_exchg_mode=%d\n",g_ui_choose_exchg_mode);
            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain g_ui_choose_exchg_mode!\n");
                return -1;
            }
            break;
        }
#endif/*defined(MBB_CHG_EXTCHG)*/

        /*9:查询是否支持库仑计*/
        case INQUIRE_COULOMETER_SUPPORT_CMD:
        {
            batt_coulometer_support = chg_get_coulometer_support();

            ret = copy_to_user((void*)arg, &batt_coulometer_support, sizeof(batt_coulometer_support));

            if(0 != ret)
            {
                printk("<1> CHG_DEBUG: Cannot obtain batt_coulometer_support!\n");

                return -1;
            }

            break;
        }
        default:
            break;
    }
    return 0;
}
static const struct file_operations g_chargerdev_fops =
{
        .owner          = THIS_MODULE,
        .read           = chargerdev_read,
        .write          = chargerdev_write,
        .unlocked_ioctl = chargerdev_ioctl,
};

static u8 g_usbotg_value = 0;
int chg_set_usb_otg(void)
{
    u8 value;
    int ret;

    ret = chargeIC_i2c_read(0x1, &value);
    if(ret){
        return -EIO;
    }

    g_usbotg_value = value & (3<<4);

    value &= ~(3<<4);
    value |= (2<<4);

    ret = chargeIC_i2c_write(0x1, value);
    if(ret){
        return -EIO;
    }

    printk(KERN_DEBUG"%s line %d:value=%#x\n",__FUNCTION__,__LINE__,value);

    return 0;
}
EXPORT_SYMBOL(chg_set_usb_otg);

int chg_clr_usb_otg(void)
{
    u8 value;
    int ret;

    ret = chargeIC_i2c_read(0x1, &value);
    if(ret){
        return -EIO;
    }

    value &= ~(3<<4);
    value |= g_usbotg_value;

    ret = chargeIC_i2c_write(0x1, value);
    if(ret){
        return -EIO;
    }

    printk(KERN_DEBUG"%s line %d:value=%#x\n",__FUNCTION__,__LINE__,value);

    return 0;
}
EXPORT_SYMBOL(chg_clr_usb_otg);

struct i2c_board_info  i2c_charger_info __initdata =
{
    .type = "bq24192",
    .addr = I2C_CHARGER_IC_ADDR,
};

static int __init i2c_charger_init(void)
{
    return i2c_register_board_info(0,&i2c_charger_info,1);
}


static int chargeIC_probe(struct i2c_client *client,
                const struct i2c_device_id *id)
{
    struct chargeIC_chip *chip;
    struct device_node *node = client->dev.of_node;
    int pn_rev, devreg_rev;
    uint8_t dev_id;
    int32_t ret = 0;
    int32_t charger_major = 0;

    dev_info(&client->dev, "%s: Begin:*******chargeIC_probe*********\n", __func__);

    if (!node) {
        dev_err(&client->dev, "%s: device tree information missing\n", __func__);
        //return - ENODEV;
    }

    if (!i2c_check_functionality(client->adapter,
            I2C_FUNC_SMBUS_BYTE_DATA)) {
        dev_err(&client->dev, "%s: SMBUS_BYTE_DATA unsupported\n", __func__);
        return - EIO;
    }

    chip = devm_kzalloc(&client->dev, sizeof(*chip), GFP_KERNEL);
    if (!chip) {
        dev_err(&client->dev, "%s: devm_kzalloc failed\n", __func__);
        return - ENOMEM;
    }

    chip->client = client;

    chip->bat.name = "battery";
    chip->bat.type = POWER_SUPPLY_TYPE_BATTERY;
    chip->bat.properties = battery_monitor_props;
    chip->bat.num_properties = ARRAY_SIZE(battery_monitor_props);
    chip->bat.get_property = battery_monitor_get_property;
    chip->bat_health = POWER_SUPPLY_HEALTH_GOOD;
    chip->bat_avg_temp = 0;
    chip->bat_present = 0; //ready to add

    chip->usb.name = "usb_test";
    chip->usb.type = POWER_SUPPLY_TYPE_USB;
    chip->usb.properties = battery_monitor_usb_props;
    chip->usb.num_properties = ARRAY_SIZE(battery_monitor_usb_props);
    chip->usb.get_property = battery_monitor_usb_get_property;
    chip->usb_online = OFFLINE;

    chip->ac.name = "ac";
    chip->ac.type = POWER_SUPPLY_TYPE_MAINS;
    chip->ac.properties = battery_monitor_ac_props;
    chip->ac.num_properties = ARRAY_SIZE(battery_monitor_ac_props);
    chip->ac.get_property = battery_monitor_ac_get_property;
    chip->ac_online = OFFLINE;

    chip->extchg.name = "extchg";
    chip->extchg.type = POWER_SUPPLY_TYPE_EXTCHG;
    chip->extchg.properties = battery_monitor_extchg_props;
    chip->extchg.num_properties = ARRAY_SIZE(battery_monitor_extchg_props);
    chip->extchg.get_property = battery_monitor_extchg_get_property;

    chip->wireless.name = "wireless";
    chip->wireless.type = POWER_SUPPLY_TYPE_WIRELESS;
    chip->wireless.properties = battery_monitor_wireless_props;
    chip->wireless.num_properties = ARRAY_SIZE(battery_monitor_wireless_props);
    chip->wireless.get_property = battery_monitor_wireless_get_property;

    /* 防止给应用上报事件过程中休眠的超时锁 */
    wake_lock_init(&(chip->alarm_wake_lock), WAKE_LOCK_SUSPEND, "MBB_CHG_ALARM");
    /* 任务处理过程中防止休眠的永久锁 */
    wake_lock_init(&(chip->chg_wake_lock), WAKE_LOCK_SUSPEND, "MBB_CHG_WORK");

    i2c_set_clientdata(client, chip);

    ret = power_supply_register(&client->dev, &chip->bat);
    if (0 != ret)
    {
        dev_err(&client->dev, "failed to register main battery\n");
        goto batt_failed;
    }

    ret = power_supply_register(&client->dev, &chip->usb);
    if (0 != ret)
    {
        dev_err(&client->dev, "failed to register usb power supply\n");
        goto usb_failed;
    }

    ret = power_supply_register(&client->dev, &chip->ac);
    if (0 != ret)
    {
        dev_err(&client->dev, "failed to register ac power supply\n");
        goto ac_failed;
    }

    ret = power_supply_register(&client->dev, &chip->extchg);
    if (0 != ret)
    {
        dev_err(&client->dev, "failed to register extchg power supply\n");
        goto extchg_failed;
    }

    ret = power_supply_register(&client->dev, &chip->wireless);
    if (0 != ret)
    {
        dev_err(&client->dev, "failed to register wireless power supply\n");
        goto wireless_failed;
    }

    g_i2c_client = chip->client;
    g_chip = chip;
    create_chargeIC_proc_file();
    create_dload_chg_proc_file();
    create_poweron_chg_proc_file();

    printk("<1> %s: ********creat proc filesystem********\n", __func__);

    //registering dev file node
    charger_major = register_chrdev(0, "charger", &g_chargerdev_fops);
    if(charger_major < 0)
    {
        printk("<1> CHG_DEBUG REGISTER CHARGE DEVICE FAILED %d!\n", charger_major);
        //do not quit this probe function
        //return -1;
    }
    else
    {
        g_charger_class = class_create(THIS_MODULE, "charger");
        device_create(g_charger_class, NULL,MKDEV(charger_major,0),"%s",
            "charger");
    }
    ret = chargeIC_i2c_read(I2C_CHARGER_IC_ADDR, &dev_id);

    if (0 != ret)
    {
        dev_err(&client->dev, "failed to chargeIC_i2c_read\n");
        //do not quit this probe function
        //return ret;
    }

    pn_rev = (dev_id & DEV_ID_PN_MASK) >> DEV_ID_PN_SHIFT;
    devreg_rev = (dev_id & DEV_ID_DEV_REG_MASK)
            >> DEV_ID_DEV_REG_SHIFT;

    dev_info(&client->dev, "%s: chargeIC probed successfully, pn_rev=%d, devreg_rev=%d\n",
        __func__, pn_rev, devreg_rev);

    //charge current limmit

    dev_info(&client->dev, "%s: g_i2c_client->addr %d\n", __func__, g_i2c_client->addr);

    /* 创建充电工作队列，用来处理各种插入拔出事件 */
    mbb_chg_wq = create_workqueue("MBB_CHG_WQ");
    if( NULL == mbb_chg_wq )
    {  
        pr_err("[MBB CHG]kernel create MBB_CHG_WQ workqueue failed.\r\n");
        return -EIO;
    }

	INIT_WORK(&(mbb_chg_event_work.event_work), charge_event_work);
#if (MBB_CHG_EXTCHG == FEATURE_ON)
    INIT_DELAYED_WORK(&chip->extchg_monitor_work,chg_extchg_monitor_work);
    schedule_delayed_work(&chip->extchg_monitor_work, msecs_to_jiffies(0));
#endif
#if 0
    //for register notifier chains
    BLOCKING_INIT_NOTIFIER_HEAD(&chip->notifier_list_bat);
    chip->nb.notifier_call = battery_monitor_charger_event;
    ret = battery_monitor_register_notifier(chip, &chip->nb);
    if(0 != ret)
    {
        //Impossible to go here, because the kernel always returns zero.
    }
#endif

    #ifdef CONFIG_MBB_FAST_ON_OFF
    ret = blocking_notifier_chain_register(&g_fast_on_off_notifier_list,
        &suspend_monitor_charger_block);
    if ( ret < 0 )
    {
        pr_err("suspend_monitor_charger_block register g_fast_on_off_notifier fail \n");
    }
    #endif/*CONFIG_MBB_FAST_ON_OFF*/

    return 0;

wireless_failed:
    power_supply_unregister(&chip->extchg);
extchg_failed:
    power_supply_unregister(&chip->ac);
ac_failed:
    power_supply_unregister(&chip->usb);
usb_failed:
    power_supply_unregister(&chip->bat);
batt_failed:
    i2c_set_clientdata(client, NULL);

    kfree(chip);
    chip = NULL;

    return ret;
}

static int chargeIC_remove(struct i2c_client *client)
{
    return 0;
}

static const struct i2c_device_id chargeIC_id[] = {
    { .name = "bq24192", },
    {},
};

static const struct of_device_id chargeIC_match[] = {
    { .compatible = "ti,bq24192", },
    { },
};

static struct i2c_driver chargeIC_driver = {
    .driver    = {
        .name           = "bq24192",
        .owner          = THIS_MODULE,
        .of_match_table = chargeIC_match,
    },
    .probe          = chargeIC_probe,
    .remove         = chargeIC_remove,
    .id_table       = chargeIC_id,
};

static int huawei_print_debug_set(void *data, u64 val)
{
    g_huawei_drv_debug = (int32_t)val;

    printk("<1> g_huawei_drv_debug = %d\n", g_huawei_drv_debug);

    return 0;
}

static int huawei_print_debug_get(void *data, u64 *val)
{

    *val = g_huawei_drv_debug;

    return 0;
}
DEFINE_SIMPLE_ATTRIBUTE(huawei_print_debug_fops, huawei_print_debug_get,
    huawei_print_debug_set, "%llu\n");

static int __init huawei_print_debug_init(void)
{
    struct dentry *den = NULL;
    printk("<1> begin creat debugfs\n");
    /* Set This File Attribute as 0644 */
    den = debugfs_create_file("drv_print", 0644, NULL, &g_huawei_drv_debug,
        &huawei_print_debug_fops);
    printk("<1> end creat debugfs den %d \n", (int32_t)den);

    return 0;
}

module_init(huawei_print_debug_init);

static int __init chargeIC_init(void)
{
    return i2c_add_driver(&chargeIC_driver);
}
module_init(chargeIC_init);

static void __exit chargeIC_exit(void)
{
    return i2c_del_driver(&chargeIC_driver);
}
module_exit(chargeIC_exit);

postcore_initcall(i2c_charger_init);

MODULE_DESCRIPTION("BQ24192 Charger");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("i2c:bq24192");
