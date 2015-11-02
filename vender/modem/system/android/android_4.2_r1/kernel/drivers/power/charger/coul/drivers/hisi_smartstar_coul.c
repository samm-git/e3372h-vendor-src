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


******************************************************************************************/
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/power_supply.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <asm/irq.h>
#include <linux/module.h>
#include <linux/io.h>
#include <asm/bug.h>
#include <mach/gpio.h>
#include <linux/delay.h>
#include "hisi_battery_data.h"
#include <hisi_coul_drv.h>
#include "../device/dev_smartstar_coul.h"
#include "product_config.h"
#include <bsp_hkadc.h>
#include <bsp_coul.h>
#include <bsp_nvim.h>
#include "bsp_sram.h"
#include "../../chg_config.h"

#if defined CONFIG_MACH_HI6620SFT
#include <mach/gpio.h>
#endif

#define RBATT_ADJ 1
#define BOOST_ENABLE 0

#define DEFAULT_V_OFF_A 1000000
#define DEFAULT_V_OFF_B 0
#define DEFAULT_C_OFF_A 1000000
#define DEFAULT_C_OFF_B 5600
#define DEFAULT_RPCB 13000 /*uohm*/

int v_offset_a = 1000000;		// 995729; /* need div 1000000 */
int v_offset_b = 0;			// 3258; /* uv */
int c_offset_a = 1000000;		// 977704; /* need div 1000000 */
int c_offset_b = 5600;			// 6071; /* ua */

#define NV_HUAWEI_COUL_INFO_I    (50462)
#define NV_BATT_VOLT_CALI_ID    (58639)
#define BATT_CALI_MIN_VOLT  (3400)
#define BATT_CALI_MAX_VOLT  (4200)

static int coul_debug_mask = 3;
int coul_debug_set(int mask)
{
    if(COUL_MSG_MAX > mask)
    {
        coul_debug_mask = mask;
    }
    return 0;
}
#ifdef _DRV_LLT_
#define SMARTSTAR_COUL_ERR(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_ERR) { \
        printf("[smartstar]" fmt, ## args); \
    } \
} while (0)
#define SMARTSTAR_COUL_EVT(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_DEBUG) { \
        printf("[smartstar]" fmt, ## args); \
    } \
} while (0)
#define SMARTSTAR_COUL_INF(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_INFO) { \
        printf("[smartstar]" fmt, ## args); \
    } \
} while (0)
#else
#define SMARTSTAR_COUL_ERR(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_ERR) { \
        printk(KERN_ERR "[smartstar]" fmt, ## args); \
    } \
} while (0)
#define SMARTSTAR_COUL_EVT(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_DEBUG) { \
        printk(KERN_ERR "[smartstar]" fmt, ## args); \
    } \
} while (0)
#define SMARTSTAR_COUL_INF(fmt,args...) do { \
if(coul_debug_mask > COUL_MSG_INFO) { \
        printk(KERN_ERR "[smartstar]" fmt, ## args); \
    } \
} while (0)
#endif

#define DI_LOCK()    do {mutex_lock(&di->soc_mutex);} while (0)
#define DI_UNLOCK()  do {mutex_unlock(&di->soc_mutex);} while (0)


#define ABNORMAL_BATT_TEMPERATURE_LOW   (-40)
#define ABNORMAL_BATT_TEMPERATURE_HIGH  (80)

#define BATTERY_EXIST_FLAG  0x1

#define BATTERY_MOVE_MAGIC_NUM        0xc3

#define TEMP_TOO_HOT            (60)
#define TEMP_TOO_COLD          (-20)


#define CHARGING_STATE_CHARGE_START         (1)
#define CHARGING_STATE_CHARGE_DONE          (2)
#define CHARGING_STATE_CHARGE_STOP          (3)
#define CHARGING_STATE_CHARGE_RECHARGE      (4)
#define CHARGING_STATE_CHARGE_NOT_CHARGE    (5)
#define CHARGING_STATE_CHARGE_UNKNOW        (0x0)

#define SMARTSTAR_DEBUG

#define IAVG_SAMPLES 10
#define CHARGING_IAVG_MA 250
#define R_COUL_MOHM     10		/* resisitance mohm */

#define DELTA_FCC_PERCENT 20
#define CALCULATE_SOC_MS    (20*1000)

#define CALIBRATE_INTERVAL (5*60*1000)	/* 5 min */
#define CHARGING_CURRENT_OFFSET (-10*1000)
#define MIN_CHARGING_CURRENT_OFFSET (10)
#define IMPOSSIBLE_IAVG            (9999)
#define IMPOSSIBLE_VOL            (2400)
/************************************************************
    coul register of smartstar
************************************************************/
static char  pmussi_reg_read(int reg_addr);

static char  pmussi_reg_write(int reg_addr,char value);

static int  pmussi_recv_data (int reg_addr, char *buffer, int len);

static int  pmussi_send_data (int reg_addr, char *buffer, int len);

#define PMUSSI_DELAY()                          udelay(500)
#define SMARTSTAR_REG_READ(regAddr)             pmussi_reg_read(regAddr)
#define SMARTSTAR_REG_WRITE(regAddr,regval)     pmussi_reg_write((unsigned int)(regAddr),(char)regval)

#define SMARTSTAR_REGS_READ(regAddr,buf,size)   pmussi_recv_data((unsigned int)(regAddr),(char*)(buf),(int)(size))
#define SMARTSTAR_REGS_WRITE(regAddr,buf,size)  pmussi_send_data((unsigned int)(regAddr),(char*)(buf),(int)(size))

#define COUL_CTRL_ENABLE     (1<<7)
#define COUL_CALI_ENABLE     (1<<6)
#define COUL_ECO_FLT_20MS    (0)
#define COUL_ECO_FLT_25MS    (0x1<<4)
#define COUL_ECO_FLT_30MS    (0x2<<4)
#define COUL_ECO_FLT_35MS    (0x3<<4)
#define COUL_ALL_REFLASH    (0x0)
#define COUL_ECO_REFLASH    (0x1<<3)
#define COUL_ECO_ENABLE     (0x1<<2)
#define COUL_ECO_PMU_EN     (0x3)   /*enable ECO with PMU, and enable signal filter*/
#define COUL_ECO_DISABLE    (0x0)

#define DEFAULT_COUL_CTRL_VAL   (COUL_CTRL_ENABLE | COUL_ECO_FLT_20MS  \
                                | COUL_ALL_REFLASH |COUL_ECO_DISABLE)


#define  COUL_VBAT_LOW_INT      (0x1<<5)
#define  COUL_LOAD_TIMER_INT    (0x1<<4)
#define  COUL_CHG_TIMER_INT     (0x1<<3)
#define  COUL_CCIN_CNT_INT      (0x1<<2)
#define  COUL_CCOUT_CNT_INT     (0x1<<1)
#define  COUL_CCOUT_BIG_INT     (0x1<<0)

#define DEFAULT_INT_MASK_VAL    (0x0)

#define SMARTSTAR_COUL_IRQ_REG      0x007
#define SMARTSTAR_COUL_IRQ_MASK_REG 0x00c

#define COUL_CALI_ING     (0x33)

#define SMARTSTAR_COUL_CTRL_REG 0x133
#define SMARTSTAR_CL_OUT_BASE   0x139
#define SMARTSTAR_CL_IN_BASE    0x135
#define SMARTSTAR_CHG_TIMER_BASE 0x141
#define SMARTSTAR_LOAD_TIMER_BASE 0x13d
#define SMARTSTAR_CL_INT_BASE 0x149
#define SMARTSTAR_VOL_INT_BASE    0x14d
#define SMARTSTAR_OFFSET_CURRENT 0x14f
#define SMARTSTAR_OFFSET_VOLTAGE 0x151
#define SMARTSTAR_OCV_REG_BASE  0x153
#define SMARTSTAR_VOL_FIFO_BASE 0x155
#define SMARTSTAR_CUR_FIFO_BASE 0x17d
#define SMARTSTAR_COUL_STATE_REG 0x1A6
#define SMARTSTAR_FIFO_CLEAR 0x1a7         //use bit 0
#define SMARTSTAR_NV_SAVE_SUCCESS 0x1a7   //use bit 1
#define SMARTSTAR_NV_READ_SUCCESS 0x1a7 //use bit 2

#define SMARTSTAR_BATTERY_MOVE_ADDR 0x1a8
#define SMARTSTAR_SAVE_OCV_ADDR 0x1a9
#define SMARTSTAR_DEBUG_REG 0x1a5

#define SMARTSTAR_LOW_VOL_INT_LVL1_SOC      (15)
#define SMARTSTAR_LOW_VOL_INT_LVL2_SOC      (2)
#define SMARTSTAR_LOW_VOL_INT_LVL1_FLAG    (1<<0)
#define SMARTSTAR_LOW_VOL_INT_LVL2_FLAG    (1<<1)
#define SMARTSTAR_LOW_VOL_INT_MASK   (SMARTSTAR_LOW_VOL_INT_LVL1_FLAG | SMARTSTAR_LOW_VOL_INT_LVL2_FLAG)

#ifdef SMARTSTAR_DEBUG
#define DBG_CNT_INC(xxx)     di->xxx++
#else
#define DBG_CNT_INC(xxx)
#endif

#define BATTERY_CC_WARNING_LEV  10
#define BATTERY_CC_LOW_LEV      3

#define BATTERY_VOL_WARNING 3550
#define BATTERY_VOL_LOW 3350
#define BATTERY_SWITCH_ON_VOLTAGE        (3250)

#define LOW_INT_NOT_SET 0
#define LOW_INT_PROMPT 1
#define LOW_INT_ALARM 2
#define LOW_INT_ZERO 3

#define ZERO_VOLTAGE 3200

#define MAX_TEMPS 10

#define INVALID_BATT_ID_VOL  -999
#define IAVG_TIME_2MIN   6 //6*20s

#define SR_ARR_LEN             100
#define SR_MAX_RESUME_TIME     90         // 90 s
#define SR_DELTA_SLEEP_TIME    (4 * 60)   // 4 min
#define SR_DELTA_WAKEUP_TIME   30         // 30 s
#define SR_TOTAL_TIME          (30 * 60)  // 30 min
#define SR_DUTY_RATIO          95
#define SR_DEVICE_WAKEUP       1
#define SR_DEVICE_SLEEP        2

static unsigned int hand_chg_capacity_flag = 0;
static unsigned int input_capacity = 50;
static int disable_temperature_debounce = 1;

static long sr_time_sleep[SR_ARR_LEN];
static long sr_time_wakeup[SR_ARR_LEN];
static int sr_index_sleep = 0;
static int sr_index_wakeup = 0;
static int sr_cur_state = 0;    // 1:wakeup  2:sleep

struct smartstar_coul_device
{
    int batt_exist;
    int prev_pc_unusable;
    int irqs[IRQ_MAX];
    unsigned char irq_mask;
    int batt_ocv_cur_limit;
    int batt_ocv_curreglimit;
    int batt_ocv; // ocv in uv
    int batt_ocv_temp;
    int batt_ocv_valid_to_refresh_fcc;
    int batt_changed_flag;
    int batt_vol_low_flag;
    int soc_limit_flag;
    int batt_uf_temp;
    int batt_temp; // temperature in degree*10
    int batt_fcc;
    int batt_limit_fcc;
    int batt_rm;
    int batt_ruc;
    int batt_uuc;
    int batt_delta_rc;
    int rbatt;
    int rbatt_ratio;
    int r_pcb;
    int soc_work_interval;
    int charging_begin_soc;
    int charging_state;
    long charging_stop_time;
    int batt_soc;
    int batt_soc_real;
    int batt_soc_with_uuc;
    int batt_soc_est;
    unsigned int batt_id_vol;
    unsigned int batt_chargecycles; //chargecycle in percent
    int last_cali_temp; // temperature in degree*10
    long cc_end_value;
    long cc_start_value;
    unsigned int v_cutoff;
    unsigned int v_low_int_value;
    unsigned long	get_cc_start_time;
    unsigned long 	get_cc_end_time;
    unsigned long   suspend_time; // time when suspend
    long   sr_suspend_time;  // sr time when suspend
    long   sr_resume_time;   // sr time when resume
    long charging_begin_cc;  /*the unit is uah*/
    long suspend_cc; // cc when suspend
    unsigned long last_time;
    long last_cc;
    int last_iavg_ma;
    int last_fifo_iavg_ma;
    struct mutex soc_mutex;
    struct hisi_smartstar_coul_battery_data *batt_data;
    struct single_row_lut	*adjusted_fcc_temp_lut;
    struct single_row_lut	adjusted_fcc_temp_lut_tbl1;
    struct single_row_lut	adjusted_fcc_temp_lut_tbl2;
    int fcc_real_mah;
    struct delayed_work	notifier_work;
    struct delayed_work	calculate_soc_delayed_work;
    struct delayed_work	boost_check_delayed_work;
    struct hisi_coul_ops *   ops;
    struct ss_coul_nv_info nv_info;
    int is_nv_read;
    int is_nv_need_save;

#ifdef SMARTSTAR_DEBUG
    unsigned int dbg_ocv_cng_0; /*ocv change count by wake up*/
    unsigned int dbg_ocv_cng_1; /*ocv change count by full charged*/
    unsigned int dbg_valid_vol; /*vaild voltage from FIFO vol registers*/
    unsigned int dbg_invalid_vol; /*invaild voltage from FIFO vol registers*/
    unsigned int dbg_ocv_fc_failed; /*full charged can't update OCV*/
#endif
};

#ifdef _DRV_LLT_
struct smartstar_coul_device *g_smartstar_coul_dev = NULL;
#else
static struct smartstar_coul_device *g_smartstar_coul_dev = NULL;
#endif

#define SS_COUL_NV_NAME  "SS_COUL"
#define SS_COUL_NV_NUM   NV_HUAWEI_COUL_INFO_I 

#define MIN_BEGIN_PERCENT_FOR_LEARNING 60

static int smartstar_battery_voltage_uv(void);

int delta_sleep_time = 10*60; // sleep time bigger could update ocv, in s
int delta_sleep_current = 50; // sleep current less could updat eocv, in mA
//int delta_ocv_update_time = 2*60; // min time between ocv update, in s

#ifndef _DRV_LLT_
module_param(delta_sleep_time, int, 0644);
//module_param(delta_ocv_update_time, int, 0644);
module_param(delta_sleep_current, int, 0644);

module_param(v_offset_a, int, 0644);
module_param(v_offset_b, int, 0644);
module_param(c_offset_a, int, 0644);
module_param(c_offset_b, int, 0644);

static int do_save_offset;
static int do_save_offset_ret;
static int save_nv_info(struct smartstar_coul_device *di);
static int read_nv_info_real(struct smartstar_coul_device *di);
static int do_save_offset_ops_set(const char *buffer,
							  const struct kernel_param *kp)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;
    if (!di){
        do_save_offset_ret = -EINVAL;
        return do_save_offset_ret;
    }
    do_save_offset_ret = save_nv_info(di);
    read_nv_info_real(di);
    return do_save_offset_ret;
}

static int do_save_offset_ops_get(char *buffer, const struct kernel_param *kp)
{
    sprintf(buffer, "%d", do_save_offset_ret);
	return strlen(buffer);
}

static struct kernel_param_ops do_save_offset_ops = {
	.set = do_save_offset_ops_set,
	.get = do_save_offset_ops_get,
};

module_param_cb(do_save_offset, &do_save_offset_ops, &do_save_offset, 0644);

static int batt_id_voltage; // in mV

static int ro_ops_set(const char *buffer, const struct kernel_param *kp)
{
    return -EINVAL;
}

static int batt_id_voltage_ops_get(char *buffer, const struct kernel_param *kp)
{
    // TODO: HKADC batt_id_voltage

    sprintf(buffer, "%d mV", batt_id_voltage);
	return strlen(buffer);
}
static struct kernel_param_ops batt_id_voltage_ops={
    .set = ro_ops_set,
    .get = batt_id_voltage_ops_get,
};

module_param_cb(batt_id_voltage, &batt_id_voltage_ops,&batt_id_voltage, 0444);
static int batt_voltage_uv;			// in uv
static int batt_voltage_uv_ops_get(char *buffer, const struct kernel_param *kp)
{
    int vol = smartstar_battery_voltage_uv();
	sprintf(buffer, "%d", vol);
	return strlen(buffer);
}

static struct kernel_param_ops batt_voltage_uv_ops = {
	.set = ro_ops_set,
	.get = batt_voltage_uv_ops_get,
};

module_param_cb(batt_voltage_uv, &batt_voltage_uv_ops, &batt_voltage_uv, 0444);
static int convert_regval2uv(short reg_val);
static int batt_init_ocv;		// in uv
static int batt_init_ocv_ops_get(char *buffer, const struct kernel_param *kp)
{
	short ocvreg, volreg_offset;
	int ocv;
	/* read ocv and offset */
	SMARTSTAR_REGS_READ(SMARTSTAR_OCV_REG_BASE, &ocvreg, 2);
	SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_VOLTAGE, &volreg_offset, 2);
	ocv = convert_regval2uv(ocvreg - volreg_offset);
	sprintf(buffer, "%d uv", ocv);
	return strlen(buffer);
}

static struct kernel_param_ops batt_init_ocv_ops = {
	.set = ro_ops_set,
	.get = batt_init_ocv_ops_get,
};

module_param_cb(batt_init_ocv, &batt_init_ocv_ops, &batt_init_ocv, 0444);
static struct platform_device *g_pdev = NULL;
static int hisi_smartstar_coul_suspend(struct platform_device *pdev,
									   pm_message_t state);
static int hisi_smartstar_coul_resume(struct platform_device *pdev);
static int coul_running = 1;	// 1 is running, 0 is suspend
static int coul_state_ops_set(const char *buffer,
							  const struct kernel_param *kp)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;
	int run;
	run = buffer[0] - '0';
	if (run && !coul_running)
	{
		coul_running = 1;
		hisi_smartstar_coul_resume(g_pdev);
	}
	else if (!run && coul_running)
	{
		pm_message_t pm;
		coul_running = 0;
		hisi_smartstar_coul_suspend(g_pdev, pm);
	}
	return 0;
}

static int coul_state_ops_get(char *buffer, const struct kernel_param *kp)
{
	sprintf(buffer, "%d", coul_running);
	return strlen(buffer);
}

static struct kernel_param_ops coul_state_ops = {
	.set = coul_state_ops_set,
	.get = coul_state_ops_get,
};

module_param_cb(coul_running, &coul_state_ops, &coul_running, 0644);
static int batt_soc_with_uuc;	// in uv
static int batt_soc_with_uuc_ops_get(char *buffer,
										const struct kernel_param *kp)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;
	sprintf(buffer, "%d", di->batt_soc_with_uuc);
	return strlen(buffer);
}

static struct kernel_param_ops batt_soc_with_uuc_ops = {
	.set = ro_ops_set,
	.get = batt_soc_with_uuc_ops_get,
};

module_param_cb(batt_soc_with_uuc, &batt_soc_with_uuc_ops,
				&batt_soc_with_uuc, 0444);
#endif

static char  pmussi_reg_read(int reg_addr)
{
    char regval;

    bsp_hi6551_reg_read(reg_addr, &regval);
    PMUSSI_DELAY();

    return regval;
}

static char  pmussi_reg_write(int reg_addr,char value)
{
    char regval;

    bsp_hi6551_reg_write(reg_addr, value);
    PMUSSI_DELAY();

    return 0;
}

static int  pmussi_recv_data (int reg_addr, char *buffer, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        //*(buffer + i) =  *(volatile char*)PMUSSI_REG(reg_addr+i);
        bsp_hi6551_reg_read((reg_addr + i), (buffer + i));
        PMUSSI_DELAY();
    }

    return 0;
}

static int  pmussi_send_data (int reg_addr, char *buffer, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        //*(volatile char*)PMUSSI_REG(reg_addr+i) = *(buffer + i);
        bsp_hi6551_reg_write((reg_addr + i), *(buffer + i));
        PMUSSI_DELAY();
    }

    return 0;
}

/**
 * clear_cc_register
 *
 * clear the cc register
 */
static void clear_cc_register(void)
{
	long ccregval = 0;
    SMARTSTAR_REGS_WRITE(SMARTSTAR_CL_IN_BASE,&ccregval,4);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_CL_OUT_BASE,&ccregval,4);
}

/**
 * convert_regval2uah
 * 1bit = 1bit current * 0.11 c = 5/10661 * 11/100 c = 5/10661 * 11/100 * 1000/3600 mAh
 *       = 11 / (10661*2*36) mAh = 11 * 1000/ (10661 *2 *36) uAh
 *       = 11 * 125/ (10661* 9) uAh
 * convert battery voltage to uah
 */
static long convert_regval2uah(unsigned long reg_val)
{
    long ret;
    s64 temp;

    temp = reg_val;
    temp = temp * 11 *125;
    temp = div_s64(temp, (10661*9));

    ret = temp / (R_COUL_MOHM/10);

    return ret;
}

#if 1 /*debug*/
static unsigned long g_cc_in = 0;
static unsigned long g_cc_out = 0;
void hisi_smartstar_print_cc_reg(int c)
{
    static int cnt = 0;
    if (c==0 && cnt>0)
        cnt --;
    else
        cnt = c;

    if (cnt)
        SMARTSTAR_COUL_ERR("reg: cc_in = 0x%x, cc_out = 0x%x\n", g_cc_in, g_cc_out);
}
#endif
/**
 * smartstar_battery_voltage
 *
 * battery voltage in milivolts
 */
static long calculate_cc_uah(void)
{
    unsigned long cc_in = 0;
    unsigned long cc_out = 0;
    unsigned long cl_in_time, cl_out_time;
    long cc_uah_in = 0;
    long cc_uah_out = 0;
    long cc_uah = 0;
    s64 temp = 0;
    SMARTSTAR_REGS_READ(SMARTSTAR_CL_IN_BASE,&cc_in,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_CL_OUT_BASE,&cc_out,4);
#if 1 /*debug*/
    g_cc_in = cc_in;
    g_cc_out = cc_out;
    hisi_smartstar_print_cc_reg(0);
#endif
    cc_uah_out = convert_regval2uah(cc_out);
    cc_uah_in = convert_regval2uah(cc_in);

    temp = (s64) c_offset_a *cc_uah_in;
    cc_uah_in = div_s64(temp, 1000000);
    temp = (s64) c_offset_a *cc_uah_out;
    cc_uah_out = div_s64(temp, 1000000);
    SMARTSTAR_REGS_READ(SMARTSTAR_CHG_TIMER_BASE, &cl_in_time, 4);
    SMARTSTAR_REGS_READ(SMARTSTAR_LOAD_TIMER_BASE, &cl_out_time, 4);
    /* uah = uas/3600 = ua*s/3600 */
    cc_uah_in = cc_uah_in - div_s64((s64) cl_in_time * c_offset_b, 3600);
    cc_uah_out =
        cc_uah_out + div_s64((s64) cl_out_time * c_offset_b, 3600);
    cc_uah = cc_uah_out - cc_uah_in;

    return cc_uah;
}

/*
 *	from coul get battery cl_in or cl_out.
 */
static unsigned long get_coul_time(void)
{
    unsigned long cl_in_time, cl_out_time;

    SMARTSTAR_REGS_READ(SMARTSTAR_CHG_TIMER_BASE,&cl_in_time,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_LOAD_TIMER_BASE,&cl_out_time,4);

    return (cl_in_time + cl_out_time);
}

/*
 * clear sleep and wakeup global variable
 */
static void clear_sr_time_array(void)
{
    memset(&sr_time_sleep, 0, sizeof(sr_time_sleep));
    memset(&sr_time_wakeup, 0, sizeof(sr_time_wakeup));
    sr_index_sleep = 0;
    sr_index_wakeup = 0;
}

/*
 *	from coul get battery cl_in or cl_out.
 */
static void clear_coul_time(void)
{
    unsigned long cl_time = 0;
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    unsigned long time_now = get_coul_time();

    if (di) {
        if (SR_DEVICE_WAKEUP == sr_cur_state) {
            di->sr_resume_time -= time_now;
            if (di->sr_resume_time > 0) {
                SMARTSTAR_COUL_ERR("[SR]%s(%d): di->sr_resume_time = %d\n", __func__, __LINE__, di->sr_resume_time);
                di->sr_resume_time = 0;
            }
            di->sr_suspend_time = 0;
        }
        else {
            di->sr_resume_time = 0;
            di->sr_suspend_time = 0;
            SMARTSTAR_COUL_ERR("[SR]%s(%d): sr_cur_state = %d\n", __func__, __LINE__, sr_cur_state);
        }
    }
    else {
        SMARTSTAR_COUL_ERR("[SR]%s(%d): di is NULL\n", __func__, __LINE__);
    }

    di->charging_stop_time -= time_now;

    SMARTSTAR_REGS_WRITE(SMARTSTAR_CHG_TIMER_BASE,&cl_time ,4);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_LOAD_TIMER_BASE,&cl_time ,4);

    clear_sr_time_array();
}

/**
 * convert_regval2uv
 * 1 bit = 225.1196 uv = 4.8/21322 V = 4.8/21322 * 1000 * 1000 uV = 24 * 1000 * 100/ 10661 uV
 * convert regval to uv
 */
static  int convert_regval2uv(short reg_val)
{
    s64 temp;

    if (reg_val & 0x8000) {
        return -1;
    }

    temp = (s64)((s64)(reg_val)  * (s64)(24 * 1000 * 100));

    temp = div_s64(temp, 10661);

    temp = (s64) v_offset_a *temp;
    temp = div_s64(temp, 1000000);
    temp += v_offset_b;

    return (int)temp;
}

/**
 * convert_uv2regval
 * 1 uv = 10661 / 24 * 1000 * 100 bit
 * convert uv to regval
 */
static  short convert_uv2regval(int uv_val)
{
    short ret;
    s64 temp;
    
    temp = uv_val - v_offset_b;
    temp = temp * 1000000;
    uv_val = div_s64(temp, v_offset_a);

    temp = (s64)((s64)uv_val * (s64)10661);
    temp = div_s64(temp,2400000);

    ret = (short)temp;

    return ret;
}

/**
 * smartstar_battery_voltage_uv
 *
 * battery voltage in uv
 */
int smartstar_battery_voltage_uv(void)
{
    short regval;

    SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE, &regval, 2);
    return(convert_regval2uv(regval));
}

/**
 * convert_regval2ua
 * 10 mohm resistance: 1 bit = 5/10661 A = 5*1000*1000 / 10661 uA
 * 20 mohm resistance: 1 bit = 10 mohm / 2
 * 30 mohm resistance: 1 bit = 10 mohm / 3
 * ...
 * high bit = 0 is in, 1 is out
 * convert regval to ua
 */
static  int convert_regval2ua(short reg_val)
{
    int ret;
    s64 temp;

    ret = reg_val;
    temp = (s64)(ret) * (s64)(1000 * 1000 * 5);
    temp = div_s64(temp, 10661);

    ret = temp / (R_COUL_MOHM/10);

    ret = -ret;

    temp = (s64) c_offset_a *ret;
    ret = div_s64(temp, 1000000);
    ret += c_offset_b;

    return ret;
}
/**
 * convert_ua2regval
 *
 * 10 mohm resistance: 1 bit = 5/10661 A = 5*1000*1000 / 10661 uA
 * 20 mohm resistance: 1 bit = 10 mohm / 2
 * 30 mohm resistance: 1 bit = 10 mohm / 3
 */
static  short convert_ua2regval(int ua)
{
    s64 temp;
    short regval;

    temp = ((s64)(ua) * (s64)(10661 * (R_COUL_MOHM/10)));
    temp = div_s64(temp,5000000);

    regval = (short)(-temp);

    return regval;
}

/**
 * smartstar_battery_current_ua - return the current of battery.
 *
 * return the battery current in uA.
 */
int smartstar_battery_current_ua(void)
{
    short regval;

    SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE, &regval, 2);

    return convert_regval2ua(regval);
}


static int is_between(int left, int right, int value)
{
	if ((left >= right) && (left >= value) && (value >= right))
		return 1;
	if ((left <= right) && (left <= value) && (value <= right))
		return 1;

	return 0;
}

static int linear_interpolate(int y0, int x0, int y1, int x1, int x)
{
	if ((y0 == y1) || (x == x0))
		return y0;
	if ((x1 == x0) || (x == x1))
		return y1;

	return y0 + ((y1 - y0) * (x - x0) / (x1 - x0));
}

static int interpolate_single_lut(struct single_row_lut *lut, int x)
{
	int i, result;

	if (x < lut->x[0]) {
		return lut->y[0];
	}
	if (x > lut->x[lut->cols - 1]) {
		return lut->y[lut->cols - 1];
	}

	for (i = 0; i < lut->cols; i++)
		if (x <= lut->x[i])
			break;
	if (x == lut->x[i]) {
		result = lut->y[i];
	} else {
		result = linear_interpolate(
			lut->y[i - 1],
			lut->x[i - 1],
			lut->y[i],
			lut->x[i],
			x);
	}
	return result;
}

static int interpolate_single_y_lut(struct single_row_lut *lut, int y)
{
	int i, result;

	if (y < lut->y[0]) {
		return lut->x[0];
	}
	if (y > lut->y[lut->cols - 1]) {
		return lut->x[lut->cols - 1];
	}

	for (i = 0; i < lut->cols; i++)
		if (y <= lut->y[i])
			break;
	if (y == lut->y[i]) {
		result = lut->x[i];
	} else {
		result = linear_interpolate(
			lut->x[i - 1],
			lut->y[i - 1],
			lut->x[i],
			lut->y[i],
			y);
	}
	return result;
}

static int interpolate_scalingfactor(struct sf_lut *sf_lut,
				int row_entry, int pc)
{
	int i, scalefactorrow1, scalefactorrow2, scalefactor;
	int rows, cols;
	int row1 = 0;
	int row2 = 0;

	/*
	 * sf table could be null when no battery aging data is available, in
	 * that case return 100%
	 */
	if (!sf_lut)
		return 100;

	rows = sf_lut->rows;
	cols = sf_lut->cols;
	if (pc > sf_lut->percent[0]) {
		//SMARTSTAR_COUL_EVT("pc %d greater than known pc ranges for sfd\n", pc);
		row1 = 0;
		row2 = 0;
	}
	if (pc < sf_lut->percent[rows - 1]) {
		//SMARTSTAR_COUL_EVT("pc %d less than known pc ranges for sf\n", pc);
		row1 = rows - 1;
		row2 = rows - 1;
	}
	for (i = 0; i < rows; i++) {
		if (pc == sf_lut->percent[i]) {
			row1 = i;
			row2 = i;
			break;
		}
		if (pc > sf_lut->percent[i]) {
			row1 = i - 1;
			row2 = i;
			break;
		}
	}

	if (row_entry < sf_lut->row_entries[0])
		row_entry = sf_lut->row_entries[0];
	if (row_entry > sf_lut->row_entries[cols - 1])
		row_entry = sf_lut->row_entries[cols - 1];

	for (i = 0; i < cols; i++)
		if (row_entry <= sf_lut->row_entries[i])
			break;
	if (row_entry == sf_lut->row_entries[i]) {
		scalefactor = linear_interpolate(
				sf_lut->sf[row1][i],
				sf_lut->percent[row1],
				sf_lut->sf[row2][i],
				sf_lut->percent[row2],
				pc);
		return scalefactor;
	}

	scalefactorrow1 = linear_interpolate(
				sf_lut->sf[row1][i - 1],
				sf_lut->row_entries[i - 1],
				sf_lut->sf[row1][i],
				sf_lut->row_entries[i],
				row_entry);

	scalefactorrow2 = linear_interpolate(
				sf_lut->sf[row2][i - 1],
				sf_lut->row_entries[i - 1],
				sf_lut->sf[row2][i],
				sf_lut->row_entries[i],
				row_entry);

	scalefactor = linear_interpolate(
				scalefactorrow1,
				sf_lut->percent[row1],
				scalefactorrow2,
				sf_lut->percent[row2],
				pc);

	return scalefactor;
}

static void get_simultaneous_battery_voltage_and_current(struct smartstar_coul_device *di, int *ibat_ua, int *vbat_uv)
{
    int vol[3], cur[3];
    int i;
    for (i=0; i<3; i++){
        vol[i] = smartstar_battery_voltage_uv();
        cur[i] = smartstar_battery_current_ua();
    }

    if (vol[0]==vol[1] && cur[0]==cur[1]){
        *ibat_ua = cur[0];
        *vbat_uv = vol[0];
    } else if (vol[1]==vol[2] && cur[1]==cur[2]){
        *ibat_ua = cur[1];
        *vbat_uv = vol[1];
    } else {
        *ibat_ua = cur[2];
        *vbat_uv = vol[2];
    }

    *vbat_uv += (*ibat_ua/1000)*(di->r_pcb/1000);

}



/**
 * interpolate_fcc
 *
 * look for fcc value by battery temperature
 */
static int interpolate_fcc(struct smartstar_coul_device *di, int batt_temp)
{
	/* batt_temp is in tenths of degC - convert it to degC for lookups */
	batt_temp = batt_temp/10;
	return interpolate_single_lut(di->batt_data->fcc_temp_lut, batt_temp);
}

/**
 * interpolate_scalingfactor_fcc
 *
 * look for fcc scaling factory value by battery charge cycles
 */
static int interpolate_scalingfactor_fcc(struct smartstar_coul_device *di,
								int cycles)
{
	/*
	 * sf table could be null when no battery aging data is available, in
	 * that case return 100%
	 */
	if (di->batt_data->fcc_sf_lut)
		return interpolate_single_lut(di->batt_data->fcc_sf_lut, cycles);
	else
		return 100;
}

static int interpolate_fcc_adjusted(struct smartstar_coul_device *di, int batt_temp)
{
	/* batt_temp is in tenths of degC - convert it to degC for lookups */
	batt_temp = batt_temp/10;
	return interpolate_single_lut(di->adjusted_fcc_temp_lut, batt_temp);
}

static int calculate_fcc_uah(struct smartstar_coul_device *di, int batt_temp,
							int chargecycles)
{
    int initfcc, result, scalefactor = 0;

    if (di->adjusted_fcc_temp_lut == NULL) {
        initfcc = interpolate_fcc(di, batt_temp);
        scalefactor = interpolate_scalingfactor_fcc(di, chargecycles);

        /* Multiply the initial FCC value by the scale factor. */
        result = (initfcc * scalefactor * 1000) / 100;

    } else {
		return 1000 * interpolate_fcc_adjusted(di, batt_temp);
    }

    return result;
}

static int interpolate_pc(struct pc_temp_ocv_lut *lut,
				int batt_temp, int ocv)
{
    int i, j, pcj, pcj_minus_one, pc;
    int rows = lut->rows;
    int cols = lut->cols;

    /* batt_temp is in tenths of degC - convert it to degC for lookups */
    batt_temp = batt_temp/10;

    if (batt_temp < lut->temp[0]) {
    	SMARTSTAR_COUL_ERR("batt_temp %d < known temp range for pc\n", batt_temp);
    	batt_temp = lut->temp[0];
    }
    if (batt_temp > lut->temp[cols - 1]) {
    	SMARTSTAR_COUL_ERR("batt_temp %d > known temp range for pc\n", batt_temp);
    	batt_temp = lut->temp[cols - 1];
    }

    for (j = 0; j < cols; j++)
    	if (batt_temp <= lut->temp[j])
    		break;
    if (batt_temp == lut->temp[j]) {
    	/* found an exact match for temp in the table */
    	if (ocv >= lut->ocv[0][j])
    		return lut->percent[0]*10;
    	if (ocv <= lut->ocv[rows - 1][j])
    		return lut->percent[rows - 1]*10;
    	for (i = 0; i < rows; i++) {
    		if (ocv >= lut->ocv[i][j]) {
    			if (ocv == lut->ocv[i][j])
    				return lut->percent[i]*10;
    			pc = linear_interpolate(
    				lut->percent[i]*10,
    				lut->ocv[i][j],
    				lut->percent[i - 1]*10,
    				lut->ocv[i - 1][j],
    				ocv);
    			return pc;
    		}
    	}
    }

    /*
     * batt_temp is within temperature for
     * column j-1 and j
     */
    if (ocv >= lut->ocv[0][j])
    	return lut->percent[0]*10;
    if (ocv <= lut->ocv[rows - 1][j - 1])
    	return lut->percent[rows - 1]*10;

    pcj_minus_one = 0;
    pcj = 0;
    for (i = 0; i < rows-1; i++) {
    	if (pcj == 0
    		&& is_between(lut->ocv[i][j],
    			lut->ocv[i+1][j], ocv)) {
    		pcj = linear_interpolate(
    			lut->percent[i]*10,
    			lut->ocv[i][j],
    			lut->percent[i + 1]*10,
    			lut->ocv[i+1][j],
    			ocv);
    	}

    	if (pcj_minus_one == 0
    		&& is_between(lut->ocv[i][j-1],
    			lut->ocv[i+1][j-1], ocv)) {

    		pcj_minus_one = linear_interpolate(
    			lut->percent[i]*10,
    			lut->ocv[i][j-1],
    			lut->percent[i + 1]*10,
    			lut->ocv[i+1][j-1],
    			ocv);
    	}

    	if (pcj && pcj_minus_one) {
    		pc = linear_interpolate(
    			pcj_minus_one,
    			lut->temp[j-1],
    			pcj,
    			lut->temp[j],
    			batt_temp);
    		return pc;
    	}
    }

    if (pcj)
    	return pcj;

    if (pcj_minus_one)
    	return pcj_minus_one;

    SMARTSTAR_COUL_ERR("%d ocv wasn't found for temp %d in the LUT returning 100%%\n",
                                            ocv, batt_temp);
    return 1000;
}


static int calculate_pc(struct smartstar_coul_device *di, int ocv_uv, int batt_temp,
							int chargecycles)
{
    int pc, scalefactor;

    pc = interpolate_pc(di->batt_data->pc_temp_ocv_lut, batt_temp, ocv_uv / 1000);

    scalefactor = interpolate_scalingfactor(di->batt_data->pc_sf_lut, chargecycles, pc/10);

    /* Multiply the initial FCC value by the scale factor. */
    pc = (pc * scalefactor) / 100;
    return pc;
}

static int calculate_remaining_charge_uah(struct smartstar_coul_device *di,
						int fcc_uah, int chargecycles)
{
    int  ocv, pc;
    int fcc_mah = fcc_uah/1000;
    int temp;
    temp = di->batt_ocv_temp;

    ocv = di->batt_ocv;
    pc = calculate_pc(di, ocv, temp, chargecycles);
    
    return fcc_mah * pc;
}


static int get_rbatt(struct smartstar_coul_device *di, int soc_rbatt, int batt_temp)
{
	int rbatt, scalefactor;

	rbatt = di->batt_data->default_rbatt_mohm;
	if (di->batt_data->rbatt_sf_lut == NULL)  {
		return rbatt;
	}
	/* Convert the batt_temp to DegC from deciDegC */
	batt_temp = batt_temp / 10;
	scalefactor = interpolate_scalingfactor(di->batt_data->rbatt_sf_lut,
							batt_temp, soc_rbatt);
	rbatt = (rbatt * scalefactor) / 100;

	if (is_between(20, 10, soc_rbatt))
		rbatt = rbatt
			+ ((20 - soc_rbatt) * di->batt_data->delta_rbatt_mohm) / 10;
	else
		if (is_between(10, 0, soc_rbatt))
			rbatt = rbatt + di->batt_data->delta_rbatt_mohm;

	return rbatt;
}

/* get ocv given a soc  -- reverse lookup */
static int interpolate_ocv(struct pc_temp_ocv_lut *lut,
				int batt_temp_degc, int pc)
{
	int i, ocvrow1, ocvrow2, ocv;
	int rows, cols;
	int row1 = 0;
	int row2 = 0;

	rows = lut->rows;
	cols = lut->cols;
	if (pc > lut->percent[0]*10) {
		//SMARTSTAR_COUL_EVT("pc %d greater than known pc ranges for sfd\n", pc);
		row1 = 0;
		row2 = 0;
	}
	if (pc < lut->percent[rows - 1]*10) {
		//SMARTSTAR_COUL_EVT("pc %d less than known pc ranges for sf\n", pc);
		row1 = rows - 1;
		row2 = rows - 1;
	}
	for (i = 0; i < rows; i++) {
		if (pc == lut->percent[i]*10) {
			row1 = i;
			row2 = i;
			break;
		}
		if (pc > lut->percent[i]*10) {
			row1 = i - 1;
			row2 = i;
			break;
		}
	}

	if (batt_temp_degc < lut->temp[0])
		batt_temp_degc = lut->temp[0];
	if (batt_temp_degc > lut->temp[cols - 1])
		batt_temp_degc = lut->temp[cols - 1];

	for (i = 0; i < cols; i++)
		if (batt_temp_degc <= lut->temp[i])
			break;
	if (batt_temp_degc == lut->temp[i]) {
		ocv = linear_interpolate(
				lut->ocv[row1][i],
				lut->percent[row1]*10,
				lut->ocv[row2][i],
				lut->percent[row2]*10,
				pc);
		return ocv;
	}

	ocvrow1 = linear_interpolate(
				lut->ocv[row1][i - 1],
				lut->temp[i - 1],
				lut->ocv[row1][i],
				lut->temp[i],
				batt_temp_degc);

	ocvrow2 = linear_interpolate(
				lut->ocv[row2][i - 1],
				lut->temp[i - 1],
				lut->ocv[row2][i],
				lut->temp[i],
				batt_temp_degc);

	ocv = linear_interpolate(
				ocvrow1,
				lut->percent[row1]*10,
				ocvrow2,
				lut->percent[row2]*10,
				pc);

	return ocv;
}


static int calculate_termination_uuc(struct smartstar_coul_device *di,
				 int batt_temp, int chargecycles,
				int fcc_uah, int i_ma,
				int *ret_pc_unusable)
{
	int unusable_uv, pc_unusable, uuc;
	int i = 0;
	int ocv_mv;
	int batt_temp_degc = batt_temp / 10;
	int rbatt_mohm;
	int delta_uv;
	int prev_delta_uv = 0;
	int prev_rbatt_mohm = 0;
	int prev_ocv_mv = 0;
	int uuc_rbatt_uv;
    int fcc_mah = fcc_uah / 1000;
    int ratio = 100;

#if RBATT_ADJ
    if (di->rbatt_ratio){
        ratio = di->rbatt_ratio;
        i_ma = di->last_fifo_iavg_ma;
    }
#endif

	for (i = 0; i <= 100; i++) {
		ocv_mv = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, i*10);
		rbatt_mohm = get_rbatt(di, i, batt_temp);
        rbatt_mohm = rbatt_mohm*ratio/100;
		unusable_uv = (rbatt_mohm * i_ma) + (ZERO_VOLTAGE * 1000);
		delta_uv = ocv_mv * 1000 - unusable_uv;

		if (delta_uv > 0)
			break;

		prev_delta_uv = delta_uv;
		prev_rbatt_mohm = rbatt_mohm;
		prev_ocv_mv = ocv_mv;
	}

	uuc_rbatt_uv = linear_interpolate(rbatt_mohm, delta_uv,
					prev_rbatt_mohm, prev_delta_uv,
					0);

	unusable_uv = (uuc_rbatt_uv * i_ma) + (ZERO_VOLTAGE * 1000);

	pc_unusable = calculate_pc(di, unusable_uv, batt_temp, chargecycles);
        uuc =  fcc_mah * pc_unusable;
	*ret_pc_unusable = pc_unusable;
	return uuc;
}

static int adjust_uuc(struct smartstar_coul_device *di, int fcc_uah,
			int new_pc_unusable,
			int new_uuc,
			int batt_temp,
			int rbatt,
			int *iavg_ma)
{
	int new_unusable_mv;
	int batt_temp_degc = batt_temp / 10;
    int fcc_mah = fcc_uah / 1000;

	if (di->prev_pc_unusable == -EINVAL
		|| abs(di->prev_pc_unusable - new_pc_unusable) <= 50) {
		di->prev_pc_unusable = new_pc_unusable;
		return new_uuc;
	}

	/* the uuc is trying to change more than 2% restrict it */
	if (new_pc_unusable > di->prev_pc_unusable)
		di->prev_pc_unusable += 20;
	else
		di->prev_pc_unusable -= 20;

	new_uuc = fcc_mah * di->prev_pc_unusable;

	return new_uuc;
}

static int calculate_unusable_charge_uah(struct smartstar_coul_device *di,
				int rbatt, int fcc_uah, int cc_uah,
				 int batt_temp, int chargecycles,
				int iavg_ua)
{
	int uuc_uah_iavg;
	int i;
	int iavg_ma = iavg_ua / 1000;
	static int iavg_samples[IAVG_SAMPLES];
	static int iavg_index;
	static int iavg_num_samples;
	int pc_unusable;

	/*
	 * if we are charging use a nominal avg current so that we keep
	 * a reasonable UUC while charging
	 */
	if (iavg_ma < 0)
		iavg_ma = CHARGING_IAVG_MA;
	iavg_samples[iavg_index] = iavg_ma;
	iavg_index = (iavg_index + 1) % IAVG_SAMPLES;
	iavg_num_samples++;
	if (iavg_num_samples >= IAVG_SAMPLES)
		iavg_num_samples = IAVG_SAMPLES;

	/* now that this sample is added calcualte the average */
	iavg_ma = 0;
	if (iavg_num_samples != 0) {
		for (i = 0; i < iavg_num_samples; i++) {
			iavg_ma += iavg_samples[i];
		}

		iavg_ma = DIV_ROUND_CLOSEST(iavg_ma, iavg_num_samples);
	}

	uuc_uah_iavg = calculate_termination_uuc(di,
					batt_temp, chargecycles,
					fcc_uah, iavg_ma,
					&pc_unusable);
    
    SMARTSTAR_COUL_INF("RBATT_ADJ:UUC =%d uAh, pc=%d.%d\n", 
        uuc_uah_iavg, pc_unusable/10, pc_unusable%10);

    di->rbatt_ratio = 0;

	/* restrict the uuc such that it can increase only by one percent */
	uuc_uah_iavg = adjust_uuc(di, fcc_uah, pc_unusable, uuc_uah_iavg,
					batt_temp, rbatt, &iavg_ma);

    uuc_uah_iavg += fcc_uah/1000*15;

	di->batt_uuc = uuc_uah_iavg;

	return uuc_uah_iavg;
}

/**
 * recalc_chargecycles - recalculate the chargecycle
 * @di: stmartstar coul device
 *
 */
static unsigned int recalc_chargecycles(struct smartstar_coul_device *di)
{
    int cc_end, real_fcc, fcc, temp, pc, new_chargecycles;
    unsigned int retval = 0;

    if (di->batt_soc==100 && di->charging_begin_soc/10<MIN_BEGIN_PERCENT_FOR_LEARNING) {
        cc_end = calculate_cc_uah();
        temp = di->batt_temp;
        real_fcc = (cc_end - di->charging_begin_cc)*1000/(1000 - di->charging_begin_soc);
        fcc = interpolate_fcc(di,temp);
        pc = real_fcc *100 / fcc;
        new_chargecycles = interpolate_single_y_lut(di->batt_data->fcc_sf_lut, pc);
        new_chargecycles -= 40*100;
        retval = (unsigned int)(new_chargecycles>0?new_chargecycles:0);

        SMARTSTAR_COUL_EVT("trigger battery charge cycle reclac, val = %d!\n", new_chargecycles);
    }

    return retval;
}

int fastboot_save_nv_info_error()
{
    unsigned short ocvreg;
    SMARTSTAR_REGS_READ(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
    if(ocvreg == 0){
        return 1;
    }
    else{
        return 0;
    }
}

static int read_nv_info(struct smartstar_coul_device *di)
{
    int ret, i;
    struct ss_coul_nv_info *pinfo = &di->nv_info;
    struct single_row_lut *preal_fcc_lut = &di->adjusted_fcc_temp_lut_tbl1;
    memcpy(&di->nv_info, (unsigned int *)SRAM_COUL_ADDR, sizeof(struct ss_coul_nv_info));

    /*由于校准NV存放在原有NV id,且格式不是斜率和偏差，因此第一次使用需要转换，后续从NV读出即可使用*/
    if((0 == pinfo->v_offset_a) || (0 == pinfo->v_offset_b))
    {
        VBAT_CALIBRATION_TYPE vbatt_calib_value;
        
        memset((void *)&vbatt_calib_value, 0, sizeof(VBAT_CALIBRATION_TYPE));
        ret = bsp_nvm_read(NV_BATT_VOLT_CALI_ID, &vbatt_calib_value, sizeof(VBAT_CALIBRATION_TYPE));
        if(!ret)
        {
            pinfo->v_offset_a = (BATT_CALI_MAX_VOLT - BATT_CALI_MIN_VOLT) * DEFAULT_V_OFF_A\
                                / (vbatt_calib_value.max_value - vbatt_calib_value.min_value);
            pinfo->v_offset_b = BATT_CALI_MIN_VOLT - vbatt_calib_value.min_value * pinfo->v_offset_a / DEFAULT_V_OFF_A;
        }
    }

    di->batt_chargecycles = pinfo->charge_cycles;
    di->r_pcb = pinfo->r_pcb==0?DEFAULT_RPCB:pinfo->r_pcb;
    v_offset_a = pinfo->v_offset_a==0?DEFAULT_V_OFF_A:pinfo->v_offset_a;
    v_offset_b = pinfo->v_offset_b==0?DEFAULT_V_OFF_B:pinfo->v_offset_b;
    c_offset_a = pinfo->c_offset_a==0?DEFAULT_C_OFF_A:pinfo->c_offset_a;
    c_offset_b = pinfo->c_offset_b==0?DEFAULT_C_OFF_B:pinfo->c_offset_b;

    SMARTSTAR_COUL_INF("read nv partion ok\n");
    SMARTSTAR_COUL_INF("v_a=%d,v_b=%d,c_a=%d,c_b=%d,cycles=%d,r_pcb=%d, reg_c=%d, reg_v=%d, start_cc=%d\n"
        ,pinfo->v_offset_a, pinfo->v_offset_b, pinfo->c_offset_a, pinfo->c_offset_b, pinfo->charge_cycles 
        ,pinfo->r_pcb, pinfo->calc_ocv_reg_c, pinfo->calc_ocv_reg_v, pinfo->start_cc);

    for (i=0; i<MAX_TEMPS; i++)
    {
        if (pinfo->real_fcc[i] == 0){
            break;
        }
        
        if (pinfo->real_fcc[i] < 100)
        {
            SMARTSTAR_COUL_INF("real fcc in nv is not currect!\n");
            return 0;
        }
    
        preal_fcc_lut->x[i] = pinfo->temp[i];
        preal_fcc_lut->y[i] = pinfo->real_fcc[i];
    }

    if (i == 0){
        SMARTSTAR_COUL_INF("no real fcc data in nv\n");
        return 0;
    }

    preal_fcc_lut->cols = i;

    di->adjusted_fcc_temp_lut = preal_fcc_lut;

    SMARTSTAR_COUL_INF("temp:real_fcc %d:%d %d:%d %d:%d %d:%d %d:%d %d:%d %d:%d\n"
        ,pinfo->temp[0], pinfo->real_fcc[0]
        ,pinfo->temp[1], pinfo->real_fcc[1]
        ,pinfo->temp[2], pinfo->real_fcc[2]
        ,pinfo->temp[3], pinfo->real_fcc[3]
        ,pinfo->temp[4], pinfo->real_fcc[4]
        ,pinfo->temp[5], pinfo->real_fcc[5]
        ,pinfo->temp[6], pinfo->real_fcc[6]
        );
    return 0;
}

static int read_nv_info_real(struct smartstar_coul_device *di)
{
    int ret, i;
    struct ss_coul_nv_info *pinfo = &di->nv_info;
    struct single_row_lut *preal_fcc_lut = &di->adjusted_fcc_temp_lut_tbl1;

    memcpy(&di->nv_info, (unsigned int *)SRAM_COUL_ADDR, sizeof(struct ss_coul_nv_info));

    /*由于校准NV存放在原有NV id,且格式不是斜率和偏差，因此第一次使用需要转换，后续从NV读出即可使用*/
    if((0 == pinfo->v_offset_a) || (0 == pinfo->v_offset_b))
    {
        VBAT_CALIBRATION_TYPE vbatt_calib_value;
        
        memset((void *)&vbatt_calib_value, 0, sizeof(VBAT_CALIBRATION_TYPE));
        ret = bsp_nvm_read(NV_BATT_VOLT_CALI_ID, &vbatt_calib_value, sizeof(VBAT_CALIBRATION_TYPE));
        if(!ret)
        {
            pinfo->v_offset_a = (BATT_CALI_MAX_VOLT - BATT_CALI_MIN_VOLT) * DEFAULT_V_OFF_A\
                                / (vbatt_calib_value.max_value - vbatt_calib_value.min_value);
            pinfo->v_offset_b = BATT_CALI_MIN_VOLT - vbatt_calib_value.min_value * pinfo->v_offset_a / DEFAULT_V_OFF_A;
        }
    }
    di->batt_chargecycles = pinfo->charge_cycles;
    di->r_pcb = pinfo->r_pcb==0?DEFAULT_RPCB:pinfo->r_pcb;
    v_offset_a = pinfo->v_offset_a==0?DEFAULT_V_OFF_A:pinfo->v_offset_a;
    v_offset_b = pinfo->v_offset_b==0?DEFAULT_V_OFF_B:pinfo->v_offset_b;
    c_offset_a = pinfo->c_offset_a==0?DEFAULT_C_OFF_A:pinfo->c_offset_a;
    c_offset_b = pinfo->c_offset_b==0?DEFAULT_C_OFF_B:pinfo->c_offset_b;

    SMARTSTAR_COUL_INF("read nv partion ok\n");
    SMARTSTAR_COUL_INF("v_a=%d,v_b=%d,c_a=%d,c_b=%d,cycles=%d,r_pcb=%d, reg_c=%d, reg_v=%d, start_cc=%d\n"
        ,pinfo->v_offset_a, pinfo->v_offset_b, pinfo->c_offset_a, pinfo->c_offset_b, pinfo->charge_cycles 
        ,pinfo->r_pcb, pinfo->calc_ocv_reg_c, pinfo->calc_ocv_reg_v, pinfo->start_cc);

    for (i=0; i<MAX_TEMPS; i++)
    {
        if (pinfo->real_fcc[i] == 0){
            break;
        }
    
        if (pinfo->real_fcc[i] < 100)
        {
            SMARTSTAR_COUL_INF("real fcc in nv is not currect!\n");
            return 0;
        }
    
        preal_fcc_lut->x[i] = pinfo->temp[i];
        preal_fcc_lut->y[i] = pinfo->real_fcc[i];
    }

    if (i == 0){
        SMARTSTAR_COUL_INF("no real fcc data in nv\n");
        return 0;
    }

    preal_fcc_lut->cols = i;

    di->adjusted_fcc_temp_lut = preal_fcc_lut;

    SMARTSTAR_COUL_INF("temp:real_fcc %d:%d %d:%d %d:%d %d:%d %d:%d %d:%d %d:%d\n"
        ,pinfo->temp[0], pinfo->real_fcc[0]
        ,pinfo->temp[1], pinfo->real_fcc[1]
        ,pinfo->temp[2], pinfo->real_fcc[2]
        ,pinfo->temp[3], pinfo->real_fcc[3]
        ,pinfo->temp[4], pinfo->real_fcc[4]
        ,pinfo->temp[5], pinfo->real_fcc[5]
        ,pinfo->temp[6], pinfo->real_fcc[6]
        );
    return 0;
}

static int save_nv_info(struct smartstar_coul_device *di)
{
    int ret, i;
    char val;
    int refresh_fcc_success = 1;
    struct ss_coul_nv_info *pinfo = &di->nv_info;

    if (!di->is_nv_read){
        SMARTSTAR_COUL_ERR("save nv before read, error\n");
        val = SMARTSTAR_REG_READ(SMARTSTAR_FIFO_CLEAR);
        SMARTSTAR_REG_WRITE(SMARTSTAR_NV_SAVE_SUCCESS, val & 0xfc);
        return -1;
    }

    pinfo->charge_cycles = di->batt_chargecycles;
    pinfo->v_offset_a = v_offset_a;
    pinfo->v_offset_b = v_offset_b;
    pinfo->c_offset_a = c_offset_a;
    pinfo->c_offset_b = c_offset_b;
    pinfo->r_pcb = di->r_pcb;
    pinfo->start_cc = di->cc_start_value;
    pinfo->ocv_temp = di->batt_ocv_temp;
    pinfo->limit_fcc = di->batt_limit_fcc;

    if (di->adjusted_fcc_temp_lut){
        for(i = 0; i < di->adjusted_fcc_temp_lut->cols; i++)
        {
            if(di->adjusted_fcc_temp_lut->y[i] < 100)
            {
                refresh_fcc_success = 0;
                break;
            }
        }
        if(refresh_fcc_success){
            for(i = 0; i < di->adjusted_fcc_temp_lut->cols; i++){
                pinfo->temp[i] = di->adjusted_fcc_temp_lut->x[i];
                pinfo->real_fcc[i] = di->adjusted_fcc_temp_lut->y[i];
            }
        }
    }
    else{
        for(i = 0; i < MAX_TEMPS; i++){
            pinfo->temp[i] = 0;
            pinfo->real_fcc[i] = 0;
        }
    }
    ret = bsp_nvm_write(NV_HUAWEI_COUL_INFO_I, pinfo, sizeof(*pinfo));
    if(ret)
    {
        SMARTSTAR_COUL_ERR("write nv %s(%d) failed !\n", SS_COUL_NV_NAME, SS_COUL_NV_NUM);
        val = SMARTSTAR_REG_READ(SMARTSTAR_FIFO_CLEAR);
        SMARTSTAR_REG_WRITE(SMARTSTAR_NV_SAVE_SUCCESS, val & 0xfc);
        return -1;
    }
    else
    {
        val = SMARTSTAR_REG_READ(SMARTSTAR_FIFO_CLEAR);
        SMARTSTAR_REG_WRITE(SMARTSTAR_NV_SAVE_SUCCESS, val | 0x02);
    }
    return ret;
}

static void update_chargecycles(struct smartstar_coul_device *di)
{
    if (di->batt_soc - di->charging_begin_soc/10 > 0) {
        di->batt_chargecycles += di->batt_soc - di->charging_begin_soc/10;
        SMARTSTAR_COUL_INF("new chargecycle=%d, added=%d\n", di->batt_chargecycles, di->batt_soc - di->charging_begin_soc/10);
    }
    else{
        SMARTSTAR_COUL_INF("chargecycle not updated, soc_begin=%d, soc_current=%d\n", di->charging_begin_soc/10, di->batt_soc);
    }

	di->charging_begin_soc = 1000;
}

/**
 * is_smartstar_coul_ready -
 *
 * 0:not ready, 1: ready
 */
int is_smartstar_coul_ready(void)
{
    // TODO: do something here ?
    return 1;
}

/**
 * hisi_smartstar_temperature
 *
 * return the battery temperature in centigrade.
 */
int smartstar_battery_temperature_tenth_degree(void)
{

    static int use_bq27510 = 0;
    static int inited = 0;
	static int once_read_ok = 0;
    static int pre_temperature = -999;
    short temperature;

    temperature = chg_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
    return temperature * 10;

}
/**
 * hisi_smartstar_temperature
 *
 * return the battery temperature in centigrade.
 */
int smartstar_battery_uf_temperature(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    return di->batt_uf_temp;
}

/**
 * hisi_smartstar_temperature
 *
 * return the battery temperature in centigrade.
 */
int smartstar_battery_temperature(void)
{
    int temp = smartstar_battery_temperature_tenth_degree();

    return temp/10;
}


/**
 * is_smartstar_battery_exist -
 *
 * 0:battery isn't exist, 1: exist
 */
extern unsigned char chg_get_batt_id_valid(void);

int is_smartstar_battery_exist(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    int temp;
    return chg_get_batt_id_valid();

}

/**
 * is_smartstar_battery_reach_thresholds
 */
int is_smartstar_battery_reach_threshold(void)
{

    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }
	
    if (di->batt_soc > BATTERY_CC_WARNING_LEV)
        return 0;
    else if (di->batt_soc > BATTERY_CC_LOW_LEV)
        return (1<<2); // BQ27510_FLAG_SOC1
    else
        return (3<<1); // BQ27510_FLAG_LOCK
}

/**
 * smartstar_battery_voltage
 *
 * battery voltage in milivolts
 */
int smartstar_battery_voltage(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    int ibat_ua = 0, vbat_uv = 0;
    get_simultaneous_battery_voltage_and_current(di, &ibat_ua, &vbat_uv);

    return vbat_uv/1000;
}

/**
 * smartstar_battery_current - return the current of battery.
 *
 * return the battery current in mA.
 */
short smartstar_battery_current(void)
{
    int cur = smartstar_battery_current_ua();
    return (short)(cur / 1000);
}

/**
 * is_smartstar_battery_full
 */
int is_smartstar_battery_full(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
	int ibat_ma = 0;
    int iterm_ma = 0;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    iterm_ma = di->batt_data->terminal_currentmA;

    ibat_ma = -smartstar_battery_current();

	if (di->last_iavg_ma > MIN_CHARGING_CURRENT_OFFSET
        && di->last_iavg_ma < iterm_ma
        && ibat_ma > MIN_CHARGING_CURRENT_OFFSET
        && ibat_ma < iterm_ma)
	{
		SMARTSTAR_COUL_INF("battery_full:ibat_ma=%d, when soc_real=%d, last_iavg_ma=%d\n",
						   ibat_ma, di->batt_soc_real, di->last_iavg_ma);
		return 1;
	}
    else
        return 0;
}
/**
 * smartstar_battery_brand
 *
 *
 * return the battery brand in string.
 */
char* smartstar_battery_brand(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    return di->batt_data->batt_brand;
}
/**
 * smartstar_battery_current_avg - return the avg_current of battery
 *   in at least 2MIN.
 * return the battery avg_current in mA.
 */
int smartstar_battery_current_avg(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    return di->last_iavg_ma;
}

/**
 * smartstar_battery_unfilter_capacity - return the unfilter capacity of battery.
 *
 * return the battery unfiltered capacity in percent.
 */
int smartstar_battery_unfiltered_capacity(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_soc_real/10;
}

/**
 * smartstar_battery_capacity - return the capacity of battery.
 *
 * return the battery capacity in percent.
 */
int smartstar_battery_capacity(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

   if(hand_chg_capacity_flag == 1)
        return input_capacity;

    if(NULL == di) 
    {
        SMARTSTAR_COUL_ERR("failed to get batt soc\n");
        return;
    }
    return di->batt_soc;
}


/**
 * smartstar_battery_rm
 *
 * return the remaining capacity in mAh.
 */
int smartstar_battery_rm(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_ruc/1000;
}

/**
 * smartstar_battery_fcc
 *
 * return battery FullChargeCapacity,the reture value is mAh Or < 0 if something fails.
 */
int smartstar_battery_fcc (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_fcc/1000;
}

int smartstar_battery_uuc (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_uuc/1000;
}

int smartstar_battery_cc (void)
{
    long cc;
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    cc = calculate_cc_uah();
    cc -= di->cc_start_value;

    return cc/1000;
}

int smartstar_battery_delta_rc (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_delta_rc/1000;
}

int smartstar_battery_ocv (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->batt_ocv;
}

int smartstar_battery_resistance (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    return di->rbatt;
}

/**
 * smartstar_battery_tte
 *
 * return time to empty, the reture value is min, or -1 in charging;
 */
int smartstar_battery_tte (void)
{
    int cc, cur;
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    cur = smartstar_battery_current_ua();

    if (cur < 0){
        return -1; /* charging now */
    }

    cc = di->batt_ruc - di->batt_uuc;

	if (cc < 0) {
        return -1;
    }

    return cc * 60 / cur;
}

/**
 * smartstar_battery_ttf
 *
 * return time to full, the reture value is min, or -1 in discharging.
 */
int smartstar_battery_ttf (void)
{
    int cc, cur;
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    cur = smartstar_battery_current_ua();

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    if (cur >= 0){
        return -1; /* discharging now */
    }

    cc = di->batt_fcc - di->batt_ruc;

    return cc * 60 / (-cur); /* cur is < 0 */
}

/**
 * smartstar_battery_health
 *
 * return 0->"Unknown", 1->"Good", 2->"Overheat", 3->"Dead", 4->"Over voltage",
 *			    5->"Unspecified failure", 6->"Cold",
 */
int smartstar_battery_health (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    int status = POWER_SUPPLY_HEALTH_GOOD;
    int temp = di->batt_temp/10;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    if (temp < TEMP_TOO_COLD)
        status = POWER_SUPPLY_HEALTH_COLD;
    else if (temp > TEMP_TOO_HOT)
        status = POWER_SUPPLY_HEALTH_OVERHEAT;

    return status;
}

/**
 * smartstar_battery_capacity_level
 *
 * return battery FullChargeCapacity,the reture value is mAh Or < 0 if something fails.
 */
int smartstar_battery_capacity_level (void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    int data_capacity = di->batt_soc;
    int status;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    if(data_capacity > 100 || data_capacity < 0)
        return POWER_SUPPLY_CAPACITY_LEVEL_UNKNOWN;
    else if ((data_capacity >= 0) && (data_capacity <= 5))
        status = POWER_SUPPLY_CAPACITY_LEVEL_CRITICAL;
    else if ((data_capacity > 5) && (data_capacity <= 15))
        status = POWER_SUPPLY_CAPACITY_LEVEL_LOW;
    else if ((data_capacity >= 95) && (data_capacity < 100))
        status = POWER_SUPPLY_CAPACITY_LEVEL_HIGH;
    else if (100 == data_capacity)
        status = POWER_SUPPLY_CAPACITY_LEVEL_FULL;
    else
        status = POWER_SUPPLY_CAPACITY_LEVEL_NORMAL;

    return status;
}

/**
 * smartstar_battery_technology
 */
int smartstar_battery_technology (void)
{
    /*Default technology is "Li-poly"*/
    return POWER_SUPPLY_TECHNOLOGY_LIPO;
}

int smartstar_battery_charge_param(struct battery_charge_param_s *param)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    if (di->batt_data == NULL)
        return 0;

    param->max_cin_currentmA = di->batt_data->max_cin_currentmA;
    param->max_currentmA= di->batt_data->max_currentmA;
    param->max_voltagemV= di->batt_data->max_voltagemV;
    param->charge_in_temp_5 = di->batt_data->charge_in_temp_5;
    param->charge_in_temp_10 = di->batt_data->charge_in_temp_10;

    return 1;
}

void save_ocv(int ocv)
{
    short ocvreg = convert_uv2regval(ocv);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
    SMARTSTAR_COUL_ERR("save ocv, ocv=%d,reg=%d", ocv, ocvreg);
}

/**
 * get_ocv_by_fcc - interpolate ocv value by full charge capacity.
 * @di: stmartstar coul device
 * @batt_temp: temperature
 * called when receive charging complete event
 */
static void get_ocv_by_fcc(struct smartstar_coul_device *di,int batt_temp)
{
    unsigned int new_ocv;
    int batt_temp_degc = batt_temp/10;

    /*looking for ocv value in the OCV-FCC table*/
    new_ocv = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, 1000);

    new_ocv *=1000;

    if ((new_ocv - di->batt_ocv) > 0) {
        DBG_CNT_INC(dbg_ocv_cng_1);
        SMARTSTAR_COUL_EVT("full charged, and OCV change, "
                            "new_ocv = %d, old_ocv = %d \n",new_ocv,di->batt_ocv);
        di->batt_ocv = new_ocv;
        di->batt_ocv_temp = di->batt_temp;
        di->batt_ocv_valid_to_refresh_fcc = 1;
        save_ocv(new_ocv);
        /*clear cc register*/
        clear_cc_register();
        clear_coul_time();
        di->get_cc_start_time = 0;
        if (di->cc_start_value != 0){
            di->cc_start_value = 0;
            di->is_nv_need_save = save_nv_info(di);
        }
    } else {
        DBG_CNT_INC(dbg_ocv_fc_failed);
        SMARTSTAR_COUL_ERR("full charged, but OCV don't change,\
                            new_ocv = %d, old_ocv = %d \n",new_ocv,di->batt_ocv);
    }
}

#define CURRENT_LIMIT (20*1000)
#define DEFAULT_BATTERY_OHMIC_RESISTANCE 100 /* mohm */
/**
 * get_ocv_by_vol - update the ocv from the value of vol FIFO.
 * @di: stmartstar coul device
 *
 * should be called when AP exit from deep sleep
 */
static void get_ocv_by_vol(struct smartstar_coul_device *di)
{
	int i, used, current_ua, voltage_uv;
    short regval,curreglimit;
    int totalvol, totalcur;

    curreglimit = (short)di->batt_ocv_curreglimit;

    totalvol = 0;
    totalcur = 0;
    used = 0;
#if 0
    for (i=0; i<20; i++) {
        SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE + i*2,&regval,2);
        if ((regval >= curreglimit) || (regval <= 0)) {
            DBG_CNT_INC(dbg_invalid_vol);
            continue;
        }
        DBG_CNT_INC(dbg_valid_vol);
        SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE + i*2,&regval,2);
        totolvol += regval;
        used++;
    }
#else
	for (i = 0; i < 20; i++)
	{
		SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE + i * 2, &regval, 2);
		current_ua = convert_regval2ua(regval);
		if (current_ua >= CURRENT_LIMIT
			|| current_ua < CHARGING_CURRENT_OFFSET)
		{
			DBG_CNT_INC(dbg_invalid_vol);
			SMARTSTAR_COUL_INF("invalid current = %d ua\n", current_ua);
			continue;
		}
		DBG_CNT_INC(dbg_valid_vol);
		SMARTSTAR_COUL_INF("valid current = %d ua\n", current_ua);
		SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE + i * 2, &regval, 2);
        voltage_uv = convert_regval2uv(regval);
        if (voltage_uv < ZERO_VOLTAGE)
        {
            DBG_CNT_INC(dbg_invalid_vol);
            SMARTSTAR_COUL_INF("invalid voltage = %d uv\n", voltage_uv);
            continue;
        }
        totalvol += voltage_uv;
              totalcur += current_ua;
		used++;
	}
#endif
	SMARTSTAR_COUL_INF("used = %d\n", used);
	if (used > 0)
	{
		voltage_uv = totalvol / used;
              current_ua = totalcur / used;
              voltage_uv += current_ua/1000*(di->r_pcb/1000 + DEFAULT_BATTERY_OHMIC_RESISTANCE);

	    if( voltage_uv<3200000
            || voltage_uv>3670000 && voltage_uv<3690000
            || voltage_uv>3730000 && voltage_uv<3800000){
    		SMARTSTAR_COUL_EVT("do not update OCV(%d)\n",
						   voltage_uv);
            return;
        }

		SMARTSTAR_COUL_EVT("awake from deep sleep, old OCV = %d \n",
						   di->batt_ocv);
		di->batt_ocv = voltage_uv;
        di->batt_ocv_temp = di->batt_temp;
        di->batt_ocv_valid_to_refresh_fcc = 1;
		clear_cc_register();
		clear_coul_time();
              save_ocv(voltage_uv);
		di->get_cc_start_time = 0;
        if (di->cc_start_value != 0){
            di->cc_start_value = 0;
            di->is_nv_need_save = save_nv_info(di);
        }
		SMARTSTAR_COUL_EVT("awake from deep sleep, new OCV = %d \n",
						   di->batt_ocv);
		DBG_CNT_INC(dbg_ocv_cng_0);
	}
}



static int get_calc_ocv(struct smartstar_coul_device *di)
{
    int ocv;
    int batt_temp = di->batt_temp;
    int chargecycles = di->batt_chargecycles/100;
    int soc_rbatt;
    int fcc_uah;
    int remaining_charge_uah;
    int rbatt;
    int vbatt_uv, ibatt_ua;

    vbatt_uv = convert_regval2uv(di->nv_info.calc_ocv_reg_v);

    ibatt_ua = convert_regval2ua(di->nv_info.calc_ocv_reg_c);

    soc_rbatt = calculate_pc(di, vbatt_uv, batt_temp, chargecycles);

    rbatt = get_rbatt(di, soc_rbatt/10, batt_temp);

    ocv =  vbatt_uv + ibatt_ua*rbatt/1000;

    SMARTSTAR_COUL_INF("calc ocv, v_uv=%d, i_ua=%d, soc_rbatt=%d, rbatt=%d, ocv=%d\n", 
        vbatt_uv, ibatt_ua, soc_rbatt/10, rbatt, ocv);

    return ocv;
}

#define FLAG_USE_CLAC_OCV 0xABCD
#define FLAG_USE_PMU_OCV 0xCDEF
/**
 * get_initial_ocv - get first ocv from register, hardware record it during system reset.
 */
static void get_initial_ocv(struct smartstar_coul_device *di)
{
    unsigned short ocvreg, volreg_offset;
    int ocv;

    struct ss_coul_nv_info *pinfo = &di->nv_info;

    SMARTSTAR_REGS_READ(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);

    SMARTSTAR_COUL_INF("ocvreg = 0x%4x\n", ocvreg);
    di->batt_ocv_valid_to_refresh_fcc = 1;

    if (ocvreg == (unsigned short)FLAG_USE_CLAC_OCV){
        ocv = get_calc_ocv(di);
        di->batt_ocv_valid_to_refresh_fcc = 0;
        ocvreg = convert_uv2regval(ocv);
        ocvreg = ocvreg | 0x8000;
        SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
        di->batt_ocv_temp = pinfo->hkadc_batt_temp*10;
        di->is_nv_need_save = 1;
    }
    else if (ocvreg==0 || ocvreg==FLAG_USE_PMU_OCV){
        SMARTSTAR_COUL_ERR("using pmu ocv.\n");

        SMARTSTAR_REGS_READ(SMARTSTAR_OCV_REG_BASE,&ocvreg,2);
        SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_VOLTAGE,&volreg_offset,2);
        ocvreg -= volreg_offset;
        ocv = convert_regval2uv(ocvreg);
        SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
    }
    else{
        if (ocvreg & 0x8000){
            ocvreg = ocvreg & ~0x8000;
            di->batt_ocv_valid_to_refresh_fcc = 0;
        }
        ocv = convert_regval2uv(ocvreg);
    }

    if(fastboot_save_nv_info_error()){
        di->batt_ocv_valid_to_refresh_fcc = 0;
        di->cc_start_value = 0;
        di->batt_ocv_temp = 250;
    }
    else{
        di->cc_start_value = pinfo->start_cc;
        di->batt_ocv_temp = pinfo->ocv_temp;
    }

    di->batt_limit_fcc = pinfo->limit_fcc;

    SMARTSTAR_COUL_INF("ocv = %d uv, start_cc = %d uAh\n", ocv, di->cc_start_value);

    di->batt_ocv = ocv;
    //di->cc_start_value = 0;

    SMARTSTAR_COUL_INF("initial OCV = %d\n", di->batt_ocv);
}

#define INT_OFFSET 10 /* mv */
/**
* calacute and set the low voltage interrupt value
*/
static void set_low_vol_int_reg_old(struct smartstar_coul_device *di)
{
    int batt_temp_degc, vol;
    short regval;

    batt_temp_degc = di->batt_temp/10;

#if 0
    if (di->v_low_int_value == LOW_INT_NOT_SET){
        if (di->batt_soc > BATTERY_CC_WARNING_LEV){
            vol = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, BATTERY_CC_WARNING_LEV);
            di->v_low_int_value = LOW_INT_PROMPT;
        }
        else if (di->batt_soc > BATTERY_CC_LOW_LEV){
            vol = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, BATTERY_CC_LOW_LEV);
            di->v_low_int_value = LOW_INT_ALARM;
        }
        else{
            vol = di->v_cutoff;
            di->v_low_int_value = LOW_INT_ZERO;
        }
    }
    else if (di->v_low_int_value == LOW_INT_PROMPT){
        vol = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, BATTERY_CC_LOW_LEV);
        di->v_low_int_value = LOW_INT_ALARM;
    }
    else if (di->v_low_int_value == LOW_INT_ALARM){
        vol = di->v_cutoff;
        di->v_low_int_value = LOW_INT_ZERO;
    }
    else if (di->v_low_int_value == LOW_INT_ZERO){
        SMARTSTAR_COUL_ERR("ERROR! don't set int when voltage == v_cutoff !\n");
    }
#else
    vol = smartstar_battery_voltage() - INT_OFFSET;

    if (di->v_low_int_value == LOW_INT_NOT_SET){
        if (vol > BATTERY_VOL_WARNING){
            vol = BATTERY_VOL_WARNING;
            di->v_low_int_value = LOW_INT_PROMPT;
        }
        else if (vol > BATTERY_VOL_LOW){
            vol = BATTERY_VOL_LOW;
            di->v_low_int_value = LOW_INT_ALARM;
        }
        else{
            vol = di->v_cutoff;
            di->v_low_int_value = LOW_INT_ZERO;
        }
    }
    else if (di->v_low_int_value == LOW_INT_PROMPT){
        if (vol < BATTERY_VOL_WARNING){
            vol = BATTERY_VOL_LOW;
            di->v_low_int_value = LOW_INT_ALARM;
        }
        else{
            vol = BATTERY_VOL_WARNING;
        }
    }
    else if (di->v_low_int_value == LOW_INT_ALARM){
        if (vol < BATTERY_VOL_LOW){
            vol = di->v_cutoff;
            di->v_low_int_value = LOW_INT_ZERO;
        }
        else{
            vol = BATTERY_VOL_LOW;
        }
    }
    else if (di->v_low_int_value == LOW_INT_ZERO){
        SMARTSTAR_COUL_ERR("ERROR! don't set int when voltage == v_cutoff !\n");
    }
#endif

    vol -= INT_OFFSET;

    regval = convert_uv2regval(vol*1000);

    SMARTSTAR_COUL_INF("set low power vol is %d mv, reg=%d, and soc = %d\n",vol, regval, di->batt_soc);

    SMARTSTAR_REGS_WRITE(SMARTSTAR_VOL_INT_BASE, &regval, 2);
}

#define LOW_INT_STATE_RUNNING 1
#define LOW_INT_STATE_SLEEP 0
#define BATTERY_VOL_2_PERCENT    (3400)/*定义低电关机门限，用于低电中断上报*/
#define BATTERY_VOL_CUTOFF    (3200)/*定义放电截止电压门限*/
#define BATTERY_PERCENT_SHUTDOWN    (2)/*定义关机百分比*/
static void set_low_vol_int_reg(struct smartstar_coul_device *di, int state)
{
    int batt_temp_degc, vol;
    short regval;

    di->v_low_int_value = state;

    if (state == LOW_INT_STATE_RUNNING){
        vol = di->v_cutoff;
    }
    else{
        vol = BATTERY_VOL_2_PERCENT;
    }

    regval = convert_uv2regval(vol*1000);

    SMARTSTAR_COUL_INF("set low power vol is %d mv, reg=%d, and soc = %d\n",vol, regval, di->batt_soc);

    SMARTSTAR_REGS_WRITE(SMARTSTAR_VOL_INT_BASE, &regval, 2);
}

/**
 * is_smartstar_battery_moved - check whether user have changed a battery
 *
 * 0:not moved, 1: moved
 */
int is_smartstar_battery_moved(void)
{
    unsigned char val;
    val = SMARTSTAR_REG_READ(SMARTSTAR_BATTERY_MOVE_ADDR);

    if (val == BATTERY_MOVE_MAGIC_NUM){
        return 0;
    }
    else {
        SMARTSTAR_REG_WRITE(SMARTSTAR_BATTERY_MOVE_ADDR, BATTERY_MOVE_MAGIC_NUM);
        return 1;
    }

}


/**
 * get_battery_id_voltage - get voltage on ID pin by HKADC.
 * @di: stmartstar coul device
 * called in module initalization
 */
static void get_battery_id_voltage(struct smartstar_coul_device *di)
{
    // TODO: HKADC
    /*change ID get from NTC resistance by HKADC path*/
    if((int)di->nv_info.hkadc_batt_id_voltage == INVALID_BATT_ID_VOL)
        di->batt_id_vol= 0;
    else
    	di->batt_id_vol = (int)di->nv_info.hkadc_batt_id_voltage;
    SMARTSTAR_COUL_INF("get battery id voltage is %d mv\n",di->batt_id_vol);
}

static int bound_soc(int soc)
{
	soc = max(0, soc);
	soc = min(100, soc);
	return soc;
}

struct vcdata{
int avg_v; //mv
int avg_c; //ma
int min_c; //ma
int max_c; //ma
};

#define FIFO_DEPTH    (20)/*初始数据可能有误，丢弃最初5组数据*/
static void get_fifo_data(struct vcdata *vc)
{
    int i,used = 0;
    static short vol_fifo[FIFO_DEPTH];
    static short cur_fifo[FIFO_DEPTH];
    int vol,cur;
    int max_cur = 0, min_cur = 0;
    int vols = 0, curs = 0;

    for (i=0; i<FIFO_DEPTH; i++) {
        SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE+i*2, &vol_fifo[i], 2);
        SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE+i*2, &cur_fifo[i], 2);
    }
#if 0
    vol=convert_regval2uv(vol_fifo[0])/1000;
    cur=convert_regval2ua(cur_fifo[0])/1000;

    vols=vol;
    curs=cur;

    max_cur = min_cur = cur;
#endif
    for (i=0; i<FIFO_DEPTH; i++){
        vol = convert_regval2uv(vol_fifo[i])/1000;
        cur = convert_regval2ua(cur_fifo[i])/1000;

        if(vol < IMPOSSIBLE_VOL)
        {
            SMARTSTAR_COUL_INF("invalid voltage = %d mv\n", vol);
            continue;    
        }      
        vols += vol;
        curs += cur;
        used++;
        
        if (cur > max_cur){
            max_cur = cur;
        }
        else if (cur < min_cur){
            min_cur = cur;
        }
    }
    if(used > 0)
    {
        vol = vols/used;
        cur = curs/used;

        vc->avg_v = vol;
        vc->avg_c = cur;
        vc->max_c = max_cur;
        vc->min_c = min_cur;
    }
}

static int calculate_delta_rc(struct smartstar_coul_device *di, int soc,
		int batt_temp, int rbatt_tbl, int fcc_uah)
{
    int ibat_ua = 0, vbat_uv = 0;
    int pc_new = 0, delta_pc = 0, pc_new_100 = 0, delta_pc_100 = 0;
    int rc_new_uah = 0, delta_rc_uah = 0, delta_rc_uah_100 = 0, delta_rc_final = 0;
    int soc_new = -EINVAL;
    int ocv = 0, delta_ocv = 0, delta_ocv_100 = 0, ocv_new = 0;
    int rbatt_calc = 0, delta_rbatt = 0;
    int batt_temp_degc = batt_temp/10;
    int ratio = 0;
    struct vcdata vc = {0};

    get_simultaneous_battery_voltage_and_current(di,
                                                &ibat_ua, &vbat_uv);

    get_fifo_data(&vc);

    vc.avg_v += (di->r_pcb/1000)*(vc.avg_c)/1000;

    di->last_fifo_iavg_ma = vc.avg_c;

    if (vc.avg_c < 10) {
    	goto out;
    }
    if (vc.avg_v < ZERO_VOLTAGE) {
        goto out;
    }

    ocv = interpolate_ocv(di->batt_data->pc_temp_ocv_lut, batt_temp_degc, di->batt_soc_real);

    rbatt_calc = (ocv - vc.avg_v)*1000/vc.avg_c;

    ratio = rbatt_calc*100/rbatt_tbl;

    di->rbatt_ratio = ratio;

    delta_rbatt = rbatt_calc - rbatt_tbl;

    delta_ocv = delta_rbatt*vc.avg_c/1000;

    ocv_new = ocv - delta_ocv;

    pc_new = interpolate_pc(di->batt_data->pc_temp_ocv_lut, batt_temp, ocv_new);

    rc_new_uah = di->batt_fcc/1000 * pc_new;

    delta_pc = pc_new - di->batt_soc_real;

    delta_rc_uah = di->batt_fcc/1000 * delta_pc;

    if (ratio <= 0){
        delta_ocv_100 = -rbatt_tbl*vc.avg_c/1000;
        pc_new_100 = interpolate_pc(di->batt_data->pc_temp_ocv_lut, batt_temp, ocv-delta_ocv_100);
        delta_pc_100 = pc_new_100 - di->batt_soc_real;
        delta_rc_uah_100 = di->batt_fcc/1000 * delta_pc_100;

        delta_rc_final = delta_rc_uah - delta_rc_uah_100;
    }

    soc_new = (rc_new_uah)*100 / (fcc_uah);

    soc_new = bound_soc(soc_new);

out:
    SMARTSTAR_COUL_INF("RBATT_ADJ: soc_new=%d rbat_calc=%d rbat_btl=%d ratio=%d "
                       "c=%d u=%d last_ocv=%d ocv_temp=%d "
                       "soc=%d.%d, ocv=%d "
                       "cmin=%d cmax=%d cavg=%d vavg=%d "
                       "delta_ocv=%d delta_pc=%d.%d delta_rc_uah=%d "
                       "delta_ocv_100=%d delta_pc_100=%d.%d delta_rc_uah_100=%d "
                       "delta_rc_final=%d \n",
                       soc_new, rbatt_calc, rbatt_tbl, ratio,
                       ibat_ua, vbat_uv, di->batt_ocv, di->batt_ocv_temp, 
                       di->batt_soc_real/10, di->batt_soc_real%10, ocv,
                       vc.min_c, vc.max_c, vc.avg_c, vc.avg_v,
                       delta_ocv, delta_pc/10, abs(delta_pc%10), delta_rc_uah,
                       delta_ocv_100, delta_pc_100/10, abs(delta_pc_100%10), delta_rc_uah_100,
                       delta_rc_final
                       );

    di->batt_soc_est = soc_new;
    return delta_rc_final;

}

static int adjust_soc(struct smartstar_coul_device *di, int soc,
		int batt_temp, int chargecycles,
		int rbatt, int fcc_uah, int uuc_uah, int cc_uah)
{
    int ibat_ua = 0, vbat_uv = 0;
    int delta_soc = 0, n = 0;
    int soc_new = soc;
    int soc_est_avg = 0;
    short ocvreg = 0;
    static int soc_ests[3] = {100,100,100};
    static int i = 0;

    get_simultaneous_battery_voltage_and_current(di,
                                                &ibat_ua, &vbat_uv);

    if (ibat_ua < -CHARGING_CURRENT_OFFSET) {
        goto out;
    }

    if (di->batt_soc_est<0){
        goto out;
    }

    soc_ests[i%3] = di->batt_soc_est;
    i++;
    
    soc_est_avg = DIV_ROUND_CLOSEST(soc_ests[0]+soc_ests[1]+soc_ests[2], 3);

    if (soc_est_avg>2){
        goto out;
    }

    delta_soc = soc - soc_est_avg;

    if (delta_soc <= 0){
        goto out;
    }

    n = 3 - soc_est_avg;

    soc_new = soc - delta_soc*n/3;

    SMARTSTAR_COUL_EVT("delta_soc=%d, mark save ocv is invalid\n", delta_soc);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);            

out:
    SMARTSTAR_COUL_INF("soc_est_avg=%d delta_soc=%d n=%d\n",
                       soc_est_avg, delta_soc, n);
    soc_new = bound_soc(soc_new);
    return soc_new;
}

static int limit_soc(struct smartstar_coul_device *di,int input_soc)
{
    int output_soc = input_soc;
    static int power_on_cnt = 0;

    int last_soc = di->batt_soc;
    int current_ua, voltage_uv;
    get_simultaneous_battery_voltage_and_current(di, &current_ua, &voltage_uv);

    if (di->soc_limit_flag == 1){
        if(current_ua >= CHARGING_CURRENT_OFFSET) {
        	if(last_soc - input_soc >= 1)
        		output_soc = last_soc - 1;
        	else
        		output_soc = last_soc;
        }
        else {
        	if(input_soc - last_soc >= 1)
        		output_soc = last_soc + 1;
        	else
        		output_soc = last_soc;
        }
    }
    else if (di->soc_limit_flag == 2){
        if(current_ua >= CHARGING_CURRENT_OFFSET) {
        	if(last_soc < input_soc)
        		output_soc = last_soc;
        }
        else {
        	if(last_soc > input_soc)
        		output_soc = last_soc;
        }
    }
    SMARTSTAR_COUL_INF("soc_limit_flag=%d last_soc=%d output_soc=%d\n",
                       di->soc_limit_flag, last_soc, output_soc);    
    /*电池达到低电关机门限，迅速将soc平滑至关机门限*/
    if(chg_is_batt_in_state_of_emergency() && (last_soc > BATTERY_PERCENT_SHUTDOWN))//0%低电关机
    {
        output_soc = last_soc - 1;
        SMARTSTAR_COUL_ERR("emergency!!adjust Battery Capacity to %d Percents\n", output_soc);
    }
    /*电压未达到关机门限时，soc低于关机门限2%,则显示3%*/
    if ((output_soc <= BATTERY_PERCENT_SHUTDOWN) && (!chg_is_batt_in_state_of_emergency())) {
        output_soc = BATTERY_PERCENT_SHUTDOWN + 1;
        SMARTSTAR_COUL_ERR("no emergency!adjust Battery Capacity to %d Percents\n", output_soc);
    }
    /*开始充电没有满电停充，但是soc已经达到100%，则上报99%*/
#if 1
    if (di->charging_state == CHARGING_STATE_CHARGE_START && input_soc > 99){
        output_soc = 99;
    }
#endif
    /*开始充电到满电停充，但是soc未达到100%，则迅速平滑至100%*/
    if (di->charging_state == CHARGING_STATE_CHARGE_DONE){
        SMARTSTAR_COUL_INF("pre_chargedone output_soc = %d\n", output_soc);
        /*达到复充门限，平滑至复充门限下，充电模块开始充电*/
        if(95 == input_soc)
        {
            di->charging_state = CHARGING_STATE_CHARGE_NOT_CHARGE;
        }
    }
    /*电池电压未达到开机门限，但是高于3250mV，则上报1%*/
    if (di->charging_state == CHARGING_STATE_CHARGE_START &&
        voltage_uv/1000>BATTERY_SWITCH_ON_VOLTAGE &&
        output_soc==0 &&
        (current_ua<-CHARGING_CURRENT_OFFSET || power_on_cnt < 3)
        ){
        output_soc = 1;
    }

    power_on_cnt ++;

    return output_soc;
}

static int calculate_iavg_ma(struct smartstar_coul_device *di, int iavg_ua)
{
    int iavg_ma = iavg_ua / 1000;
    int i;
	static int iavg_samples[IAVG_SAMPLES];
	static int iavg_index;
	static int iavg_num_samples;

	iavg_samples[iavg_index] = iavg_ma;
	iavg_index = (iavg_index + 1) % IAVG_SAMPLES;
	iavg_num_samples++;
	if (iavg_num_samples >= IAVG_SAMPLES)
		iavg_num_samples = IAVG_SAMPLES;
	
	iavg_ma = 0;	
	for (i = 0; i < iavg_num_samples; i++) {
		iavg_ma += iavg_samples[i];
	}

	iavg_ma = DIV_ROUND_CLOSEST(iavg_ma, iavg_num_samples);

    if (iavg_num_samples > IAVG_TIME_2MIN)
        di->last_iavg_ma = -iavg_ma;
}

static void calculate_soc_params(struct smartstar_coul_device *di,
						int *fcc_uah,
						int *unusable_charge_uah,
						int *remaining_charge_uah,
						int *cc_uah,
						int *delta_rc_uah,
						int *rbatt)
{
    int soc_rbatt, iavg_ua, delta_time_s;
    int batt_temp = di->batt_temp;
    int chargecycles = di->batt_chargecycles/100;
    static int first_in = 1;

    *fcc_uah = calculate_fcc_uah(di, batt_temp, chargecycles);

    di->batt_fcc = *fcc_uah;

    /* calculate remainging charge */
    *remaining_charge_uah = calculate_remaining_charge_uah(di,
    				*fcc_uah, chargecycles);

    di->batt_rm = *remaining_charge_uah;

    /* calculate cc micro_volt_hour */
    di->cc_end_value = calculate_cc_uah();
    *cc_uah = di->cc_end_value - di->cc_start_value;

    di->batt_ruc = *remaining_charge_uah - *cc_uah;

    di->get_cc_end_time = get_coul_time();

	di->batt_soc_real = DIV_ROUND_CLOSEST((*remaining_charge_uah - *cc_uah), *fcc_uah/1000);

	SMARTSTAR_COUL_INF("SOC real = %d\n", di->batt_soc_real);

    soc_rbatt = di->batt_soc_real/10;
    if (soc_rbatt < 0)
    	soc_rbatt = 0;

    *rbatt = get_rbatt(di, soc_rbatt, batt_temp);

#if RBATT_ADJ
    *delta_rc_uah = calculate_delta_rc(di, di->batt_soc_real, di->batt_temp, *rbatt, *fcc_uah);
    di->batt_delta_rc = *delta_rc_uah;
#endif

    if (first_in){
        di->last_cc = di->cc_end_value;
        di->last_time = di->get_cc_end_time;
        iavg_ua = smartstar_battery_current_ua();
        first_in = 0;
    }
    else{
        int delta_cc = di->cc_end_value - di->last_cc;
        int delta_time = di->get_cc_end_time - di->last_time;
        di->last_cc = di->cc_end_value;
        di->last_time = di->get_cc_end_time;

        if(delta_time > 0)
            iavg_ua = div_s64((s64)delta_cc * 3600, delta_time);
        else
            iavg_ua = smartstar_battery_current_ua();

        SMARTSTAR_COUL_INF("delta_time=%ds, iavg_ua=%d\n", delta_time, iavg_ua);
    }

    calculate_iavg_ma(di,iavg_ua);

    *unusable_charge_uah = calculate_unusable_charge_uah(di, *rbatt,
    				*fcc_uah, *cc_uah, 
    				batt_temp, chargecycles, iavg_ua);
}

#ifdef _DRV_LLT_
int calculate_state_of_charge(struct smartstar_coul_device *di)
#else
static int calculate_state_of_charge(struct smartstar_coul_device *di)
#endif
{
    int remaining_usable_charge_uah, fcc_uah, unusable_charge_uah, delta_rc_uah;
    int remaining_charge_uah, soc;
    int cc_uah;
    int rbatt;
    int temp;

    if (!di->batt_exist){
        return 0;
    }
    
    /* check temperature */
    temp = smartstar_battery_temperature_tenth_degree();

    if (temp != di->batt_temp){
        di->batt_temp = temp;
    }

    SMARTSTAR_COUL_INF("battery temperature is %d\n", temp/10);

    calculate_soc_params(di,
                                    &fcc_uah,
                                    &unusable_charge_uah,
                                    &remaining_charge_uah,
                                    &cc_uah,
                                    &delta_rc_uah,
                                    &rbatt);

    SMARTSTAR_COUL_INF("FCC=%duAh, UUC=%duAh, RC=%duAh, CC=%duAh, delta_RC=%duAh, Rbatt=%dmOhm\n",
                       fcc_uah, unusable_charge_uah, remaining_charge_uah, cc_uah, delta_rc_uah, rbatt);

    di->rbatt = rbatt;

    if (di->batt_limit_fcc){
        fcc_uah = di->batt_limit_fcc;
        SMARTSTAR_COUL_INF("use limit_FCC! %duAh\n", fcc_uah);
    }

	soc = DIV_ROUND_CLOSEST((remaining_charge_uah - cc_uah), fcc_uah/100);
	SMARTSTAR_COUL_INF("SOC without UUC = %d\n", soc);
	di->batt_soc_with_uuc = soc;

    /* calculate remaining usable charge */
    remaining_usable_charge_uah = remaining_charge_uah
                                                - cc_uah - unusable_charge_uah + delta_rc_uah;

    if (fcc_uah - unusable_charge_uah <= 0) {
    	soc = 0;
    } else {
    	soc = DIV_ROUND_CLOSEST((remaining_usable_charge_uah),
    				(fcc_uah - unusable_charge_uah)/100);
    }
    if (soc > 100)
    	soc = 100;

    SMARTSTAR_COUL_INF("SOC before adjust = %d\n", soc);
    soc= adjust_soc(di, soc, di->batt_temp, di->batt_chargecycles/100, rbatt, fcc_uah, unusable_charge_uah, cc_uah);

    SMARTSTAR_COUL_INF("SOC before limit = %d\n", soc);

    soc = limit_soc(di, soc);
    SMARTSTAR_COUL_INF("SOC_NEW = %d\n", soc);

    di->batt_soc = soc;

    return soc;
}

void cali_adc(void)
{
	unsigned char reg_val = 0;
    reg_val = SMARTSTAR_REG_READ(SMARTSTAR_COUL_STATE_REG);

    if (COUL_CALI_ING == reg_val){
    	SMARTSTAR_COUL_EVT("cali ing, don't do it again!\n");
        return;
    }
	SMARTSTAR_COUL_INF("calibrate!\n");
	reg_val = SMARTSTAR_REG_READ(SMARTSTAR_COUL_CTRL_REG);
	/* Mode */
	reg_val = reg_val | COUL_CALI_ENABLE;
	SMARTSTAR_REG_WRITE(SMARTSTAR_COUL_CTRL_REG, reg_val);
}

void print_offset(void)
{
	unsigned short volreg_offset, curreg_offset;
	SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_VOLTAGE, &volreg_offset, 2);
	SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_CURRENT, &curreg_offset, 2);
	SMARTSTAR_COUL_INF("calibrated, volreg_offset=%d, curreg_offset=%d\n",
					   volreg_offset, curreg_offset);
}

#define CHARGED_OCV_UPDATE_INTERVAL (10*60*1000)

#if BOOST_ENABLE /* test boost */
void boost_init()
{
#define SMARTSTAR_BOOST_VOLTAGE_ADDR   0x0C2
    char regval;
	regval = SMARTSTAR_REG_READ(SMARTSTAR_BOOST_VOLTAGE_ADDR);

    /* 0x5 = 101 means 3.8V */
    regval = (regval & (~0x7)) | 0x5;

    SMARTSTAR_REG_WRITE(SMARTSTAR_BOOST_VOLTAGE_ADDR, regval);
}

int boost_enable_threshold = 3400;
int boost_disable_threshold = 3600;
module_param(boost_enable_threshold, int, 0644);
module_param(boost_disable_threshold, int, 0644);

void set_boost_enable(int enable)
{
#define SMARTSTAR_BOOST_BYPASS_ADDR   0x0B9

    static int state = 0;
    static int pre_state = 0;
    char regval;

    int vol = smartstar_battery_voltage_uv()/1000;

	SMARTSTAR_COUL_INF("boost, vol=%d, enable=%d, state=%d, pre_state=%d\n",
        vol, enable, state, pre_state);

    if (enable){
        if (state && vol>boost_disable_threshold){
            state = 0;
            /* set bypass */
            regval = 0x3;
            SMARTSTAR_REG_WRITE(SMARTSTAR_BOOST_BYPASS_ADDR, regval);
        }
        else if ((!state && vol<boost_enable_threshold) || pre_state){
            state = 1;
            pre_state = 0;
            /* set enable */
            regval = 0x1;
            boost_init();
            SMARTSTAR_REG_WRITE(SMARTSTAR_BOOST_BYPASS_ADDR, regval);
        }
    }
    else{
        /* set bypass */
        pre_state = state;
        state = 0;
        regval = 0x3;
        SMARTSTAR_REG_WRITE(SMARTSTAR_BOOST_BYPASS_ADDR, regval);
    }
}

static void boost_check_work(struct work_struct *work)
{
    struct smartstar_coul_device *di = container_of(work,
				struct smartstar_coul_device,
				boost_check_delayed_work.work);

    set_boost_enable(1);

    schedule_delayed_work(&di->boost_check_delayed_work,
    		round_jiffies_relative(msecs_to_jiffies(1000)) );

}
#endif
/**
 * calculate_soc_work - schedule every CALCULATE_SOC_MS.
 */
 static void calculate_soc_work(struct work_struct *work)
{
    struct smartstar_coul_device *di = container_of(work,
				struct smartstar_coul_device,
				calculate_soc_delayed_work.work);

    static int cali_cnt = 0;
    static int charged_cnt = 0;
    static long last_cc=0;
    static long last_time=0;
    int ret = -1;

    if(di->is_nv_need_save){
        ret = save_nv_info(di);
        if(!ret)
        {
            di->is_nv_need_save = 0;
        }
    }

    DI_LOCK();
    /* calc soc */
    di->batt_soc = calculate_state_of_charge(di);
#if 1
   cali_cnt++;
   if (cali_cnt % (CALIBRATE_INTERVAL / di->soc_work_interval) == 0)
   {
           cali_adc();
   }
   else if (cali_cnt % (CALIBRATE_INTERVAL / di->soc_work_interval) == 1)
   {
           print_offset();
   }
#else
    if (di->batt_temp - di->last_cali_temp > 50){/* 50 = 5 degree */
        di->last_cali_temp = di->batt_temp;
        cali_adc();
        calied = 1;
    }
    if (calied){
        print_offset();
        calied = 0;
    }
#endif

    if (di->charging_state == CHARGING_STATE_CHARGE_DONE){
        if(charged_cnt == 0){
            last_cc = calculate_cc_uah();
            last_time = get_coul_time();
        }
        charged_cnt++;

        if (charged_cnt%(CHARGED_OCV_UPDATE_INTERVAL/CALCULATE_SOC_MS) == 0){
        	long sleep_cc, sleep_current;
        	long sleep_time, time_now;

            sleep_cc = calculate_cc_uah();
            sleep_cc = sleep_cc - last_cc;
            time_now = get_coul_time();
            sleep_time = time_now - last_time;

        	SMARTSTAR_COUL_INF("sleep_cc=%d, sleep_time=%d\n", sleep_cc, sleep_time);

            if (sleep_time <= 0){
                charged_cnt --;
            }
            else {
            	sleep_current = (sleep_cc * 18) / (sleep_time * 5);

                if(sleep_current<0){
                    sleep_current = -sleep_current;
                }

            	SMARTSTAR_COUL_INF("sleep_current=%d\n", sleep_current);

                if (sleep_current < 20){
                    get_ocv_by_vol(di);
                }
            }
            last_cc = calculate_cc_uah();
            last_time = get_coul_time();
        }
        
    }
    else{
        charged_cnt = 0;
    }

    DI_UNLOCK();

    if (di->batt_soc>30){
        di->soc_work_interval = CALCULATE_SOC_MS;
    }
    else{
        di->soc_work_interval = CALCULATE_SOC_MS/2;
    }

    schedule_delayed_work(&di->calculate_soc_delayed_work,
    		round_jiffies_relative(msecs_to_jiffies(di->soc_work_interval)) );
}

static void make_cc_no_overload(struct smartstar_coul_device *di)
{
	long cc;
	unsigned long time;
	cc = calculate_cc_uah();
	cc = cc - di->cc_start_value;
	di->cc_start_value -= cc;
	time = get_coul_time();
	time = time - di->get_cc_start_time;
	di->get_cc_start_time = -time;
	clear_cc_register();
	clear_coul_time();
    di->is_nv_need_save = save_nv_info(di);
}

/**
 * interrupt_notifier_work - send a notifier event to battery monitor.
 */
 static void interrupt_notifier_work(struct work_struct *work)
{
    long evt;
    
    struct smartstar_coul_device *di = container_of(work,
				struct smartstar_coul_device,
				notifier_work.work);
	unsigned char intstat = di->irq_mask;
	int ibat_ua = 0, vbat_uv = 0;
    int delta_soc = 0;
	get_simultaneous_battery_voltage_and_current(di, &ibat_ua, &vbat_uv);

	if (intstat & COUL_VBAT_LOW_INT)
	{
        SMARTSTAR_COUL_EVT("IRQ: COUL_VBAT_LOW_INT, vbat=%duv, last vbat_int=%d\n", vbat_uv, di->v_low_int_value);

#if 0
        DI_LOCK();
        /* voltage interrupt */
        di->batt_soc = calculate_state_of_charge(di);
        DI_UNLOCK();
        if (di->batt_soc <= BATTERY_CC_LOW_LEV)
        {
            evt = BATTERY_LOW_SHUTDOWN;
        }
        else if (di->batt_soc <= BATTERY_CC_WARNING_LEV)
        {
            evt = BATTERY_LOW_WARNING;
        }

        if (di->v_low_int_value == LOW_INT_ZERO){
        if (vbat_uv/1000 < di->v_cutoff){
            SMARTSTAR_COUL_ERR("voltage too low =%dmv, soc force to 0!\n", vbat_uv);
            di->batt_soc = 0;
        }
        else{
            SMARTSTAR_COUL_ERR("voltage =%dmv,  is still bigger then v_cutoff(%dmv) !\n", vbat_uv/1000, di->v_cutoff);
        }
        }
        else{
            set_low_vol_int_reg(di);
        }
		blocking_notifier_call_chain(&notifier_list, evt, NULL);//hisi_coul_charger_event_rcv(evt);
#else
        vbat_uv /= 1000;
        vbat_uv -= INT_OFFSET;

        if (di->v_low_int_value == LOW_INT_STATE_SLEEP){
            if (vbat_uv < BATTERY_VOL_2_PERCENT){
                delta_soc = di->batt_soc - 2;
                di->batt_soc = 0;
            }
        }
        else if (di->v_low_int_value == LOW_INT_STATE_RUNNING){
            if (vbat_uv < di->v_cutoff){
                delta_soc = di->batt_soc;
                di->batt_soc = 0;
            }
        }

        if (delta_soc > 1){
            short ocvreg = 0;
            SMARTSTAR_COUL_EVT("delta_soc=%d, mark save ocv is invalid\n", delta_soc);
            SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);            
        }
        chg_low_battery_event_handler();
#endif
        di->irq_mask &= ~COUL_VBAT_LOW_INT;
	}
	if (intstat & COUL_LOAD_TIMER_INT)
	{
		SMARTSTAR_COUL_INF("IRQ: COUL_LOAD_TIMER_INT\n");
              di->irq_mask &= ~COUL_LOAD_TIMER_INT;

	}
	if (intstat & COUL_CHG_TIMER_INT)
	{
		SMARTSTAR_COUL_INF("IRQ: COUL_CHG_TIMER_INT\n");
              di->irq_mask &= ~COUL_CHG_TIMER_INT;
	}
	if (intstat & COUL_CCIN_CNT_INT)
	{
		SMARTSTAR_COUL_INF("IRQ: COUL_CCIN_CNT_INT\n");
		make_cc_no_overload(di);
              di->irq_mask &= ~COUL_CCIN_CNT_INT;
	}
	if (intstat & COUL_CCOUT_CNT_INT)
	{
		SMARTSTAR_COUL_INF("IRQ: COUL_CCOUT_CNT_INT\n");
		make_cc_no_overload(di);
              di->irq_mask &= ~COUL_CCOUT_CNT_INT;
	}
	if (intstat & COUL_CCOUT_BIG_INT)
	{
		SMARTSTAR_COUL_INF("IRQ: COUL_CCOUT_BIG_INT\n");
              di->irq_mask &= ~COUL_CCOUT_BIG_INT;
		// TODO: something ?
	}
}
/**
 * coul_irq_cb - coul irq handler
 */
/*库仑计相关中断*/
static irqreturn_t coul_irq_cb_COUL_INT(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[0]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_CCOUT_BIG_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}
static irqreturn_t coul_irq_cb_COUL_OUT(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[1]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_CCOUT_CNT_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}
static irqreturn_t coul_irq_cb_COUL_IN(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[2]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_CCIN_CNT_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}
static irqreturn_t coul_irq_cb_CHG_TIMER(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[3]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_CHG_TIMER_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}
static irqreturn_t coul_irq_cb_LOAD_TIMER(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[4]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_LOAD_TIMER_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}
static irqreturn_t coul_irq_cb_VBAT_INT(void *_di)
{
    struct smartstar_coul_device *di = _di;

    SMARTSTAR_COUL_INF("coul_irq_cb irq=%d\n", di->irqs[5]);

    if (NULL == di){
        SMARTSTAR_COUL_ERR("invalid irq!!\n");
        return IRQ_NONE;
    }

    di->irq_mask |= COUL_VBAT_LOW_INT;
    schedule_delayed_work(&di->notifier_work, msecs_to_jiffies(0));

    return IRQ_HANDLED;
}

typedef  void (*coul_irq_cb_func )(void);

static coul_irq_cb_func coul_irq_cb[IRQ_MAX] =
{
    (coul_irq_cb_func)coul_irq_cb_COUL_INT,
    (coul_irq_cb_func)coul_irq_cb_COUL_OUT,  
    (coul_irq_cb_func)coul_irq_cb_COUL_IN,
    (coul_irq_cb_func)coul_irq_cb_CHG_TIMER,
    (coul_irq_cb_func)coul_irq_cb_LOAD_TIMER,  
    (coul_irq_cb_func)coul_irq_cb_VBAT_INT
};

/**
 * aging_update_begin - begin aging update when charging beging.
 * @di: stmartstar coul device
 *
 * should be called after charging beging, and this can't be called
 * in atomic context
 */
static void smartstar_charging_begin (struct smartstar_coul_device *di)
{
    SMARTSTAR_COUL_INF("smartstar_charging_begin +\n");

    SMARTSTAR_COUL_INF("pre charging state is %d \n",di->charging_state);

    if (di->charging_state == CHARGING_STATE_CHARGE_START)
        return;

    di->charging_state = CHARGING_STATE_CHARGE_START;

    /*calculate soc again*/
    di->batt_soc = calculate_state_of_charge(di);

    /*record soc of charging begin*/
    di->charging_begin_soc = di->batt_soc_real;

    /*record cc value*/
    di->charging_begin_cc = calculate_cc_uah();

    SMARTSTAR_COUL_INF("smartstar_charging_begin -\n");

}


static void smartstar_charging_stop (struct smartstar_coul_device *di)
{
    int rm, cc;
    int fcc_101 = di->batt_fcc*101/100;
    int need_save = 1;

    if (CHARGING_STATE_CHARGE_UNKNOW == di->charging_state){
        return;
    }

    di->batt_soc = calculate_state_of_charge(di);

    if (CHARGING_STATE_CHARGE_START == di->charging_state){
        update_chargecycles(di);
        need_save = 1;
    }

    cc = calculate_cc_uah();

    cc = cc - di->cc_start_value;

    rm = di->batt_rm - cc;

    if (rm > fcc_101){
        di->cc_start_value -= rm-fcc_101;
        need_save = 1;
        di->batt_limit_fcc = 0;
    }
	else if (di->batt_soc == 100 && di->batt_soc_real > 950){
		di->batt_limit_fcc = rm*100/101;
	}

    if(need_save){
        di->is_nv_need_save = save_nv_info(di);
    }

    di->charging_state = CHARGING_STATE_CHARGE_STOP;
    di->charging_stop_time = get_coul_time();
    set_low_vol_int_reg(di, LOW_INT_STATE_RUNNING);
}

static int calculate_real_fcc_uah(struct smartstar_coul_device *di,
								  int *ret_fcc_uah)
{
    int fcc_uah, unusable_charge_uah, delta_rc;
    int remaining_charge_uah;
    int cc_uah;
    int real_fcc_uah;
    int rbatt;
    calculate_soc_params(di,
    					 &fcc_uah,
    					 &unusable_charge_uah,
    					 &remaining_charge_uah, &cc_uah, &delta_rc, &rbatt);
    real_fcc_uah = (-(cc_uah - di->charging_begin_cc))/(1000 - di->charging_begin_soc)*1000;
    //real_fcc_uah = remaining_charge_uah - cc_uah;
    //real_fcc_uah = real_fcc_uah*100/101;
    *ret_fcc_uah = fcc_uah;
    SMARTSTAR_COUL_INF("real_fcc=%d, RC=%d CC=%d fcc=%d charging_begin_soc=%d.%d\n",
    				   real_fcc_uah, remaining_charge_uah, cc_uah, fcc_uah, di->charging_begin_soc/10, di->charging_begin_soc);
    return real_fcc_uah;
}

static void readjust_fcc_table(struct smartstar_coul_device *di)
{
	struct single_row_lut *temp, *now;
	int i, fcc, ratio;
	int real_fcc_mah = di->fcc_real_mah;
	if (!di->batt_data->fcc_temp_lut)
	{
		SMARTSTAR_COUL_ERR("%s The static fcc lut table is NULL\n", __func__);
		return;
	}
    if (di->adjusted_fcc_temp_lut == NULL){
    	temp = &di->adjusted_fcc_temp_lut_tbl1;
        now = di->batt_data->fcc_temp_lut;
    }
    else if (di->adjusted_fcc_temp_lut == &di->adjusted_fcc_temp_lut_tbl1){
    	temp = &di->adjusted_fcc_temp_lut_tbl2;
        now = di->batt_data->fcc_temp_lut;
    }
    else{
    	temp = &di->adjusted_fcc_temp_lut_tbl1;
        now = di->batt_data->fcc_temp_lut;
    }

	fcc = di->batt_fcc/1000;
	temp->cols = now->cols;
	for (i = 0; i < now->cols; i++)
	{
		temp->x[i] = now->x[i];
		ratio = div_u64(now->y[i] * 1000, fcc);
		temp->y[i] = (ratio * real_fcc_mah);
		temp->y[i] /= 1000;
		SMARTSTAR_COUL_INF("temp=%d, staticfcc=%d, adjfcc=%d, ratio=%d\n",
						   temp->x[i], now->y[i],
						   temp->y[i], ratio);
	}
	di->adjusted_fcc_temp_lut = temp;
}

void refresh_fcc(struct smartstar_coul_device *di)
{
	if (di->charging_begin_soc/10 < MIN_BEGIN_PERCENT_FOR_LEARNING
        && di->batt_ocv_valid_to_refresh_fcc
	    && (di->batt_ocv>3200000 && di->batt_ocv<3670000
            || di->batt_ocv>3690000 && di->batt_ocv <3730000
            || di->batt_ocv>3800000 && di->batt_ocv <3900000
            )
        )
	{
		int fcc_uah, new_fcc_uah, delta_fcc_uah;
		new_fcc_uah = calculate_real_fcc_uah(di, &fcc_uah);
		delta_fcc_uah = new_fcc_uah - fcc_uah;
		if (delta_fcc_uah < 0)
			delta_fcc_uah = -delta_fcc_uah;
		if (delta_fcc_uah * 100 > (DELTA_FCC_PERCENT * fcc_uah))
		{
			/* new_fcc_uah is outside the scope limit it */
			if (new_fcc_uah > fcc_uah)
				new_fcc_uah = (fcc_uah + (DELTA_FCC_PERCENT * fcc_uah) / 100);
			else
				new_fcc_uah = (fcc_uah - (DELTA_FCC_PERCENT * fcc_uah) / 100);
			SMARTSTAR_COUL_INF("delta_fcc=%d > %d percent of fcc=%d"
							   "restring it to %d\n",
							   delta_fcc_uah, DELTA_FCC_PERCENT,
							   fcc_uah, new_fcc_uah);
		}
        di->fcc_real_mah = new_fcc_uah / 1000;
        SMARTSTAR_COUL_EVT("refresh_fcc, start soc=%d, new fcc=%d \n",
            di->charging_begin_soc, di->fcc_real_mah); 

		readjust_fcc_table(di);
	}
}

static void smartstar_charging_done (struct smartstar_coul_device *di)
{

    if (CHARGING_STATE_CHARGE_START != di->charging_state) {
        SMARTSTAR_COUL_ERR("smartstar_charging_done, but pre charge state is %d \n",
                            di->charging_state);
        return;
    }

    refresh_fcc(di);

    SMARTSTAR_COUL_EVT("smartstar_charging_done, adjust soc from %d to 100\n",di->batt_soc);

    di->batt_soc = 100;

    if (di->batt_changed_flag) {
        /*recalculate charge cycles*/
        recalc_chargecycles(di);
    }

    get_ocv_by_fcc(di, di->batt_temp);

    di->batt_fcc  = calculate_fcc_uah(di, di->batt_temp, di->batt_chargecycles/100);
    //di->cc_start_value = -di->batt_fcc/100;

    update_chargecycles(di);

    di->is_nv_need_save = save_nv_info(di);

    di->charging_state = CHARGING_STATE_CHARGE_DONE;

    SMARTSTAR_COUL_INF("new charging cycles = %d%%\n", di->batt_chargecycles);
}

#ifdef _DRV_LLT_
void charger_event_process(struct smartstar_coul_device *di,unsigned long event)
#else
static void charger_event_process(struct smartstar_coul_device *di,unsigned long event)
#endif
{
    switch (event) {
    	case VCHRG_START_USB_CHARGING_EVENT:
    	case VCHRG_START_AC_CHARGING_EVENT:
    	case VCHRG_START_CHARGING_EVENT:
            SMARTSTAR_COUL_EVT("receive charge start event = 0x%x\n",(int)event);
            /*record soc and cc value*/
            DI_LOCK();
            smartstar_charging_begin(di);
            DI_UNLOCK();
    		break;

    	case VCHRG_STOP_CHARGING_EVENT:
            SMARTSTAR_COUL_EVT("receive charge stop event = 0x%x\n",(int)event);
            DI_LOCK();
    	    smartstar_charging_stop(di);
    	    DI_UNLOCK();
    	    break;

    	case VCHRG_CHARGE_DONE_EVENT:
            SMARTSTAR_COUL_EVT("receive charge done event = 0x%x\n",(int)event);
            DI_LOCK();
    	    smartstar_charging_done(di);
    	    DI_UNLOCK();
    		break;

    	case VCHRG_NOT_CHARGING_EVENT:
    	    di->charging_state = CHARGING_STATE_CHARGE_NOT_CHARGE;
            SMARTSTAR_COUL_ERR("charging is stop by fault\n");
    	    break;

    	case VCHRG_POWER_SUPPLY_OVERVOLTAGE:
    	    di->charging_state = CHARGING_STATE_CHARGE_NOT_CHARGE;
            SMARTSTAR_COUL_ERR("charging is stop by overvoltage\n");
    		break;
            
    	case VCHRG_POWER_SUPPLY_WEAKSOURCE:
    	    di->charging_state = CHARGING_STATE_CHARGE_NOT_CHARGE;
            SMARTSTAR_COUL_ERR("charging is stop by weaksource\n");
    		break;
            
    	default:
            di->charging_state = CHARGING_STATE_CHARGE_NOT_CHARGE;
    	    SMARTSTAR_COUL_ERR("unknow event %d\n",(int)event);
    		break;
    }
}

/**
 * smartstar_battery_charger_event_rcv
 */
int smartstar_battery_charger_event_rcv (unsigned long evt)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if (!di || !di->batt_exist){
        return 0;
    }
    if (!is_smartstar_battery_exist()){
        return 0; 
    }

    charger_event_process(di,evt);

    return 0;
}



/**
 * smartstar_coul_config_power_on_reg - config smartstar registers.
 */
static void smartstar_coul_config_power_on_reg(struct smartstar_coul_device *di)
{
    short low_vol_reg;

    /* Mode */
    SMARTSTAR_REG_WRITE(SMARTSTAR_COUL_CTRL_REG,DEFAULT_COUL_CTRL_VAL);

    /* coulometer interrupt mask */
    SMARTSTAR_REG_WRITE(SMARTSTAR_COUL_IRQ_MASK_REG,DEFAULT_INT_MASK_VAL);
}


/**
 * smartstar_coul_config_power_off_reg - close coul when probe failed.
 */
static void smartstar_disable_coul(struct smartstar_coul_device *di)
{
    /* Mode */
    SMARTSTAR_REG_WRITE(SMARTSTAR_COUL_CTRL_REG, (~COUL_CTRL_ENABLE) );
}

#ifdef SMARTSTAR_DEBUG
void ss_cur_show(void)
{
    int i,ua;
    short regval;

    for (i=0; i<20; i++) {
        SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE+i*2, &regval, 2);
        ua = convert_regval2ua(regval);
        printk(KERN_ERR"[%02d]:regval=0x%04x,cur = %d ua \n",i,regval,ua);
    }
	SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE, &regval, 2);
	ua = convert_regval2ua(regval);
	printk(KERN_ERR "***CLJ ST*** CURRENT = %d ua ***CLJ ST***\n", ua);
}

void ss_vol_show(void)
{
    int i,uv;
    short regval;

    for (i=0; i<20; i++) {
        SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE+i*2, &regval, 2);
        uv = convert_regval2uv(regval);
        printk(KERN_ERR"[%02d]:regval=0x%04x,vol = %d uv \n",i,regval,uv);
    }
	SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE, &regval, 2);
	uv = convert_regval2uv(regval);
	printk(KERN_ERR "***CLJ ST*** VOLTAGE = %d uv ***CLJ ST***\n", uv);
}

void ss_ocv_show(void)
{
    short ocvreg,volreg_offset;
    int ocv;

    /*read ocv and offset */
    SMARTSTAR_REGS_READ(SMARTSTAR_OCV_REG_BASE,&ocvreg,2);
    SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_VOLTAGE,&volreg_offset,2);

    ocv = convert_regval2uv(ocvreg - volreg_offset);

    printk(KERN_ERR"regval=0x%04x,offset = 0x%04x,ocv = %d uv \n",\
            ocvreg,volreg_offset,ocv);
	printk(KERN_ERR "***CLJ ST*** OCV = %d uv ***CLJ ST***\n", ocv);
}

void ss_get_ocv(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    if (di)
        get_initial_ocv(di);
}

void ss_cc_show(void)
{
	unsigned long cc_in = 0;
	unsigned long cc_out= 0;
	long cc_uah_in = 0;
	long cc_uah_out = 0;
	long cc_uah = 0;
	long cc_uah_adjust = 0;
	s64 temp = 0;
	unsigned long cl_in_time, cl_out_time;

    SMARTSTAR_REGS_READ(SMARTSTAR_CL_IN_BASE,&cc_in,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_CL_OUT_BASE,&cc_out,4);
	cc_uah_out = convert_regval2uah(cc_out);
	cc_uah_in = convert_regval2uah(cc_in);
	cc_uah = cc_uah - cc_uah_in;

	temp = (s64) c_offset_a *cc_uah_in;
	cc_uah_in = div_s64(temp, 1000000);
	temp = (s64) c_offset_a *cc_uah_out;
	cc_uah_out = div_s64(temp, 1000000);
	SMARTSTAR_REGS_READ(SMARTSTAR_CHG_TIMER_BASE, &cl_in_time, 4);
	SMARTSTAR_REGS_READ(SMARTSTAR_LOAD_TIMER_BASE, &cl_out_time, 4);
	/* uah = uas/3600 = ua*s/3600 */
	cc_uah_in = cc_uah_in - div_s64((s64) cl_in_time * c_offset_b, 3600);
	cc_uah_out =
		cc_uah_out + div_s64((s64) cl_out_time * c_offset_b, 3600);
	cc_uah_adjust = cc_uah_out - cc_uah_in;

	printk(KERN_ERR "ccout_reg=0x%x,ccin_reg = 0x%x,cc = %d uah \n"
		   "cl_out_time=%d, cl_in_time=%d, cc_adjust=%d uah\n",
		   (unsigned int) cc_out, (unsigned int)cc_in, cc_uah,
		   cl_out_time, cl_in_time, cc_uah_adjust);
}

void ss_time_show(void)
{
    long cl_in_time,cl_out_time;

    SMARTSTAR_REGS_READ(SMARTSTAR_CHG_TIMER_BASE,&cl_in_time,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_LOAD_TIMER_BASE,&cl_out_time,4);

    printk(KERN_ERR"cl_in_time=%d,cl_out_time = %d,total = %d\n",\
            (unsigned int)cl_in_time,(unsigned int)cl_out_time,(unsigned int)(cl_in_time+cl_out_time));

}

void ss_soc_update(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    unsigned int old_soc;
    old_soc = di->batt_soc;
    di->batt_soc = calculate_state_of_charge(di);
    printk(KERN_ERR"batt_soc %d ---> %d \n",old_soc,di->batt_soc);
}

void ss_soc_show(void)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;
	printk(KERN_ERR "***CLJ ST*** SOC = %d ***CLJ ST***\n", di->batt_soc);
}

void ss_di_show(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    int i;

    printk(KERN_ERR"batt_exist = 0x%x\n", di->batt_exist);
    printk(KERN_ERR"prev_pc_unusable = %d\n", di->prev_pc_unusable);
    for (i=0; i<IRQ_MAX; i++){
        printk(KERN_ERR"irq[i] = %d\n", di->irqs[i]);
    }
    printk(KERN_ERR"irq_mask = 0x%x\n", di->irq_mask);
    printk(KERN_ERR"batt_ocv_cur_limit = %d\n", di->batt_ocv_cur_limit);
    printk(KERN_ERR"batt_ocv_curreglimit = %d\n", di->batt_ocv_curreglimit);
    printk(KERN_ERR"batt_ocv = %d\n", di->batt_ocv);
    printk(KERN_ERR"batt_changed_flag = %d\n", di->batt_changed_flag);
    printk(KERN_ERR"batt_vol_low_flag = %d\n", di->batt_vol_low_flag);
    printk(KERN_ERR"soc_limit_flag = %d\n", di->soc_limit_flag);
    printk(KERN_ERR"batt_temp = %d\n", di->batt_temp);
    printk(KERN_ERR"batt_fcc = %d\n", di->batt_fcc);
    printk(KERN_ERR"batt_limit_fcc = %d\n", di->batt_limit_fcc);
    printk(KERN_ERR"batt_rm = %d\n", di->batt_rm);
    printk(KERN_ERR"batt_ruc = %d\n", di->batt_ruc);
    printk(KERN_ERR"batt_uuc = %d\n", di->batt_uuc);
    printk(KERN_ERR"rbatt = %d\n", di->rbatt);
    printk(KERN_ERR"r_pcb = %d\n", di->r_pcb);
    printk(KERN_ERR"soc_work_interval = %d\n", di->soc_work_interval);
    printk(KERN_ERR"charging_begin_soc = %d\n", di->charging_begin_soc);
    printk(KERN_ERR"charging_state = %d\n", di->charging_state);
    printk(KERN_ERR"batt_soc = %d\n", di->batt_soc);
    printk(KERN_ERR"batt_soc_real = %d\n", di->batt_soc_real);
    printk(KERN_ERR"batt_soc_with_uuc = %d\n", di->batt_soc_with_uuc);
    printk(KERN_ERR"batt_soc_est = %d\n", di->batt_soc_est);
    printk(KERN_ERR"batt_id_vol = %d\n", di->batt_id_vol);
    printk(KERN_ERR"batt_chargecycles = %d\n", di->batt_chargecycles);
    printk(KERN_ERR"last_cali_temp = %d\n", di->last_cali_temp);
    printk(KERN_ERR"cc_end_value = %d\n", di->cc_end_value);
    printk(KERN_ERR"cc_start_value = %d\n", di->cc_start_value);
    printk(KERN_ERR"v_cutoff = %d\n", di->v_cutoff);
    printk(KERN_ERR"v_low_int_value = %d\n", di->v_low_int_value);
    printk(KERN_ERR"get_cc_start_time = %d\n", di->get_cc_start_time);
    printk(KERN_ERR"get_cc_end_time = %d\n", di->get_cc_end_time);
    printk(KERN_ERR"suspend_time = %d\n", di->suspend_time);
    printk(KERN_ERR"charging_begin_cc = %d\n", di->charging_begin_cc);
    printk(KERN_ERR"suspend_cc = %d\n", di->suspend_cc);
    printk(KERN_ERR"last_time = %d\n", di->last_time);
    printk(KERN_ERR"last_cc = %d\n", di->last_cc);
    printk(KERN_ERR"last_iavg_ma = %d\n", di->last_iavg_ma);
    printk(KERN_ERR"fcc_real_mah = %d\n", di->fcc_real_mah);
    printk(KERN_ERR"is_nv_read = %d\n", di->is_nv_read);
    printk(KERN_ERR"is_nv_need_save = %d\n", di->is_nv_need_save);
    printk(KERN_ERR"dbg_ocv_cng_0 = %d\n",di->dbg_ocv_cng_0);
    printk(KERN_ERR"dbg_ocv_cng_1 = %d\n",di->dbg_ocv_cng_1);
    printk(KERN_ERR"dbg_valid_vol = %d\n",di->dbg_valid_vol);
    printk(KERN_ERR"dbg_invalid_vol = %d\n",di->dbg_invalid_vol);
    printk(KERN_ERR"dbg_ocv_fc_failed = %d\n",di->dbg_ocv_fc_failed);

}

void ss_offset_show(void)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;
    struct ss_coul_nv_info *pinfo = &di->nv_info;
    SMARTSTAR_COUL_ERR("C_offset: a=%d, b=%d uA V_offset: a=%d, b=%d uV, r_pcb=%d uohm\n",
		 pinfo->c_offset_a, pinfo->c_offset_b, pinfo->v_offset_a, pinfo->v_offset_b, pinfo->r_pcb);
}

void ss_offset_set(int c_a, int c_b, int v_a, int v_b, int r_pcb)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;

	c_offset_a = c_a;
	c_offset_b = c_b;
	v_offset_a = v_a;
	v_offset_b = v_b;
    di->r_pcb = r_pcb;
    SMARTSTAR_COUL_ERR("C_offset: a=%d, b=%d uA V_offset: a=%d, b=%d uV, r_pcb=%d uohm\n",
		 c_offset_a, c_offset_b, v_offset_a, v_offset_b, di->r_pcb);

    save_nv_info(di);
}

int ss_realfcc_show(void)
{
	struct smartstar_coul_device *di = g_smartstar_coul_dev;
    int i;
    struct single_row_lut *preal_fcc_lut = di->adjusted_fcc_temp_lut;

    if (preal_fcc_lut == NULL){
        SMARTSTAR_COUL_INF("no real fcc data in nv\n");
        return 0;
    }

    for (i=0; i<preal_fcc_lut->cols; i++)
    {
        SMARTSTAR_COUL_INF("temp=%d real_fcc=%d\n", preal_fcc_lut->x[i], preal_fcc_lut->y[i]);
    }

	return di->fcc_real_mah;
}

void ss_realfcc_set(int fcc_mah)
{
    struct smartstar_coul_device *di = g_smartstar_coul_dev;

    SMARTSTAR_COUL_INF("ss_realfcc_set fcc_mah = %d\n", fcc_mah);

    if (fcc_mah == 0){
        di->adjusted_fcc_temp_lut = NULL;
        di->batt_limit_fcc = 0;
    }
    else{
    	di->fcc_real_mah = fcc_mah;
    	readjust_fcc_table(di);
    }

    save_nv_info(di);
}

#endif

static ssize_t smartstar_show_gaugelog(struct device_driver *driver, char *buf)
{
    int uf_temp = 0, temp = 0, voltage = 0, ufcapacity = 0, capacity = 100, afcapacity = 0, rm = 0, fcc = 0, delta_rc = 0;
    int cur = 0,uuc = 0,cc = 0, ocv=0, rbatt;
    u16 flag = 0,control_status = 0;
    int qmax = 0;
    u8 RaTable[80];
    char temp_buff[50];
    int i = 0,j = 0;
    int TRise = 0,TTimeConstant = 0;
    //struct bq27510_device_info* di = g_battery_measure_by_bq27510_device;
	struct smartstar_coul_device *di = g_smartstar_coul_dev;

    if(NULL == buf){
        return -1;
    }

    if (di == NULL)
        return sprintf(buf, "%s", "Smartstar coulometer probe failed!                                                          ");

    uf_temp = smartstar_battery_uf_temperature();
    temp =  smartstar_battery_temperature();
    voltage = smartstar_battery_voltage();
    cur = -(smartstar_battery_current());
    ufcapacity = smartstar_battery_unfiltered_capacity();
    capacity = smartstar_battery_capacity();
    rm =   smartstar_battery_rm();
    fcc =  smartstar_battery_fcc();
    uuc = smartstar_battery_uuc();
    cc = smartstar_battery_cc();
    delta_rc = smartstar_battery_delta_rc();
    ocv = smartstar_battery_ocv();
    rbatt = smartstar_battery_resistance();

    sprintf(buf, "%-6d  %-6d  %-8d  %-6d  %-3d  %-5d  %-6d  %-6d  %-5d  %-6d  %-5d  %-4d  %-7d  %-5d  %-5d  ",
                voltage,  (signed short)cur, ufcapacity, capacity, afcapacity, rm, fcc, uuc, cc, delta_rc, uf_temp, temp, ocv, rbatt, di->batt_limit_fcc/1000);

    return strlen(buf);
}

static ssize_t smartstar_set_hand_chg_capacity_flag(struct device_driver *driver, char *buf, size_t count)
{
    long val;
    int status = count;

    if ((strict_strtol(buf, 10, &val) < 0) || (val < 0) || (val > 1))
        return -EINVAL;
    hand_chg_capacity_flag = val;
    return status;
}

static ssize_t smartstar_show_hand_chg_capacity_flag(struct device_driver *driver, char *buf)
{
    unsigned long val;

    val = hand_chg_capacity_flag;
    return sprintf(buf, "%lu\n", val);
}

static ssize_t smartstar_set_input_capacity(struct device_driver *driver, char *buf, size_t count)
{
    long val;
    int status = count;

    if ((strict_strtol(buf, 10, &val) < 0) || (val < 0) || (val > 100))
        return -EINVAL;
    input_capacity = val;
    return status;
}

static ssize_t smartstar_show_input_capacity(struct device_driver *driver, char *buf)
{
    unsigned long val;

    val = input_capacity;
    return sprintf(buf, "%lu\n", val);
}

static DRIVER_ATTR(gaugelog, S_IWUSR | S_IWGRP | S_IRUGO, smartstar_show_gaugelog,NULL);
static DRIVER_ATTR(hand_chg_capacity_flag, S_IWUSR | S_IRUGO,
                  smartstar_show_hand_chg_capacity_flag,
                  smartstar_set_hand_chg_capacity_flag);
static DRIVER_ATTR(input_capacity, S_IWUSR | S_IRUGO,
                  smartstar_show_input_capacity,
                  smartstar_set_input_capacity);

static struct platform_driver hisi_smartstar_coul_driver;

static int hisi_get_irqs(struct platform_device *pdev, struct smartstar_coul_device *di)
{
    int i, j;
    int retval;
    struct resource *resource = NULL;
    /*get irq number*/
    for (i=0; i<IRQ_MAX; i++){
        resource = platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if (!resource) {
            SMARTSTAR_COUL_ERR("smartstar deviece haven't include IRQ\n");
            SMARTSTAR_COUL_ERR("disable smart star coul function\n");
            return -EPERM;
        }
        di->irqs[i] = resource->start;
        retval = bsp_coul_int_register(di->irqs[i], coul_irq_cb[i], (void*)di);
        if (retval) {
            SMARTSTAR_COUL_ERR("Failed to request coul handler[%d]!, ret=%d\n", di->irqs[i], retval);
            return retval;
        } else {
            //enable_irq_wake(di->irqs[i]);
            SMARTSTAR_COUL_INF("irq[%d] request ok\n", di->irqs[i]);
        }
    }

    return 0;
    
}

static struct wake_lock hisi_smartstar_wakelock;


#ifdef _DRV_LLT_
int hisi_smartstar_coul_probe(struct platform_device *pdev)
#else
static int __init hisi_smartstar_coul_probe(struct platform_device *pdev)
#endif
{
    struct hisi_coul_ops *coul_ops = NULL;
    struct smartstar_coul_device *di = NULL;
    int retval = 0, i;
    int val;
    SMARTSTAR_COUL_ERR("coul probe begin...\n");

#if defined CONFIG_MACH_HI6620SFT
    gpio_request(GPIO_4_1,"pmu");
    gpio_request(GPIO_4_2,"pmu");
    gpio_request(GPIO_7_4,"pmu");
    gpio_request(GPIO_6_2,"pmu");
    gpio_request(GPIO_9_0,"pmu");
    /*enable smartstar*/
    gpio_direction_output(GPIO_4_1,1);
    gpio_direction_output(GPIO_4_2,1);
    gpio_direction_output(GPIO_7_4,1);
    gpio_direction_output(GPIO_6_2,1);
    gpio_direction_output(GPIO_9_0,1);
#endif
    int tryloop = 0;
    do {
        retval = SMARTSTAR_REG_READ(0x0);
        SMARTSTAR_COUL_INF("do a dummy read, smartstar version is 0x%x\n",
                            retval);

        udelay(500);

        if ((tryloop++) > 5){
            SMARTSTAR_COUL_ERR("Smartstar version is not correct!\n");
            goto coul_failed;
        }

    } while(retval != 0x20);

    val = SMARTSTAR_REG_READ(SMARTSTAR_DEBUG_REG);
    udelay(500);
    if(val != 0x0){
        short ocvreg = 0x0;
        SMARTSTAR_COUL_ERR("smartstar debug reg is not 0x0\n");
        SMARTSTAR_REG_WRITE(SMARTSTAR_DEBUG_REG, 0x0);
        udelay(500);
        SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR, &ocvreg, 2);
        udelay(500);
        BUG();
    }

    retval = driver_create_file(&(hisi_smartstar_coul_driver.driver), &driver_attr_gaugelog);
        if (0 != retval) {
            printk("failed to create sysfs entry(gaugelog): %d\n", retval);
            return -1;
        }
    retval = driver_create_file(&(hisi_smartstar_coul_driver.driver), &driver_attr_hand_chg_capacity_flag);
        if (0 != retval) {
            printk("failed to create sysfs entry(hand_chg_capacity_flag): %d\n", retval);
            return -1;
        }
    retval = driver_create_file(&(hisi_smartstar_coul_driver.driver), &driver_attr_input_capacity);
        if (0 != retval) {
            printk("failed to create sysfs entry(input_capacity): %d\n", retval);
            return -1;
        }

    //wake_lock_init(&hisi_smartstar_wakelock, WAKE_LOCK_SUSPEND, "hisi_smartstar_wakelock");

    di = (struct smartstar_coul_device *)kzalloc(sizeof(*di), GFP_KERNEL);
    if (!di) {
		SMARTSTAR_COUL_ERR("%s failed to alloc di struct\n",__FUNCTION__);
		retval = -ENOMEM;
		goto coul_failed_0;
    }
    memset((void *)di,0,sizeof(*di));
    g_smartstar_coul_dev = di;

    platform_set_drvdata(pdev, di);

    /*set di element */
    di->prev_pc_unusable = -EINVAL;

    di->v_cutoff = BATTERY_VOL_CUTOFF;//3150;
    di->batt_ocv_cur_limit = 25000; /*ua*/
    di->batt_ocv_curreglimit = (short)convert_ua2regval(di->batt_ocv_cur_limit);
    di->v_low_int_value = LOW_INT_NOT_SET;
    di->last_cali_temp = -990; /* invalid temperature */
    di->soc_work_interval = CALCULATE_SOC_MS;
    di->last_iavg_ma = IMPOSSIBLE_IAVG;
    /* read nv info */
    di->is_nv_read = !read_nv_info(di);
    val = SMARTSTAR_REG_READ(SMARTSTAR_NV_READ_SUCCESS);
    val = val & 0x4;
    di->is_nv_read = !!val;

    di->is_nv_need_save = 0;

    mutex_init(&di->soc_mutex);

    di->sr_resume_time = get_coul_time();
    sr_cur_state = SR_DEVICE_WAKEUP;

    /*check battery is exist*/
    if (!is_smartstar_battery_exist()) {
        SMARTSTAR_COUL_ERR("%s: no battery, just registe callback\n",__FUNCTION__);
        //retval = -EPERM;
        di->batt_data = get_battery_data(di->batt_id_vol);
        di->batt_exist = 0;
        goto coul_no_battery;
    }

    di->batt_exist = 1;

    /*set battery data*/
    get_battery_id_voltage(di);
    di->batt_data = get_battery_data(di->batt_id_vol);
    if (NULL == di->batt_data) {
        SMARTSTAR_COUL_ERR("%s: batt ID(0x%x) is invalid\n",__FUNCTION__,di->batt_id_vol);
        retval = -EINVAL;
        goto coul_failed_1;
    }


    SMARTSTAR_COUL_INF("%s: batt ID is %d\n",__FUNCTION__,di->batt_id_vol);

    INIT_DELAYED_WORK(&di->calculate_soc_delayed_work,
        calculate_soc_work);

#if BOOST_ENABLE
    INIT_DELAYED_WORK(&di->boost_check_delayed_work,
        boost_check_work);
#endif
    
    di->batt_temp = smartstar_battery_temperature_tenth_degree();

    SMARTSTAR_COUL_INF("battery temperature is %d.%d\n", di->batt_temp/10, di->batt_temp%10);
    
    /*calculate first soc */
    get_initial_ocv(di);

    di->charging_stop_time = get_coul_time();

    /*config coul reg */
    smartstar_coul_config_power_on_reg(di);

    if(is_smartstar_battery_moved()){
        char val;
        di->batt_chargecycles = 0;
        di->batt_changed_flag = 1;
        di->batt_limit_fcc = 0;
        di->adjusted_fcc_temp_lut = NULL; /* enable it when test ok */
        di->is_nv_need_save = 1;
        val = SMARTSTAR_REG_READ(SMARTSTAR_FIFO_CLEAR);
        SMARTSTAR_REG_WRITE(SMARTSTAR_NV_SAVE_SUCCESS, val & 0xfd);
        SMARTSTAR_COUL_INF("battery changed, reset chargecycles!\n");
    } else {
        SMARTSTAR_COUL_INF("battery not changed, chargecycles = %d%%\n", di->batt_chargecycles);
    }

    /*get the first soc value*/
    DI_LOCK();
    di->soc_limit_flag = 0;
    di->batt_soc = calculate_state_of_charge(di);
    di->soc_limit_flag = 1;
    DI_UNLOCK();

    /*schedule calculate_soc_work*/
    schedule_delayed_work(&di->calculate_soc_delayed_work,
                        round_jiffies_relative(msecs_to_jiffies(di->soc_work_interval)));

#if BOOST_ENABLE
    schedule_delayed_work(&di->boost_check_delayed_work,
                        round_jiffies_relative(msecs_to_jiffies(1000)));
#endif

    INIT_DELAYED_WORK(&di->notifier_work,
                      interrupt_notifier_work);


    set_low_vol_int_reg(di, LOW_INT_STATE_RUNNING);

    if (hisi_get_irqs(pdev, di)){
        goto coul_failed_2;
    }

coul_no_battery:
    coul_ops = (struct hisi_coul_ops*) kzalloc(sizeof (*coul_ops), GFP_KERNEL);
    if (!coul_ops) {
		SMARTSTAR_COUL_ERR("failed to alloc coul_ops struct\n");
		retval = -ENOMEM;
        goto coul_failed_3;
    }

    coul_ops->is_coul_ready = is_smartstar_coul_ready;
    coul_ops->is_battery_exist = is_smartstar_battery_exist;
    coul_ops->is_battery_reach_threshold = is_smartstar_battery_reach_threshold;
    coul_ops->is_battery_full = is_smartstar_battery_full;
    coul_ops->battery_brand = smartstar_battery_brand;
    coul_ops->battery_voltage = smartstar_battery_voltage;
    coul_ops->battery_voltage_uv= smartstar_battery_voltage_uv;
    coul_ops->battery_current = smartstar_battery_current;
    coul_ops->battery_current_avg = smartstar_battery_current_avg;
    coul_ops->battery_unfiltered_capacity = smartstar_battery_unfiltered_capacity;
    coul_ops->battery_capacity = smartstar_battery_capacity;
    coul_ops->battery_uf_temperature = smartstar_battery_uf_temperature;
    coul_ops->battery_temperature = smartstar_battery_temperature;
    coul_ops->battery_rm = smartstar_battery_rm;
    coul_ops->battery_fcc = smartstar_battery_fcc;
    coul_ops->battery_tte = smartstar_battery_tte;
    coul_ops->battery_ttf = smartstar_battery_ttf;
    coul_ops->battery_health = smartstar_battery_health;
    coul_ops->battery_capacity_level = smartstar_battery_capacity_level;
    coul_ops->battery_technology = smartstar_battery_technology;
    coul_ops->battery_charge_param = smartstar_battery_charge_param;
    coul_ops->charger_event_rcv = smartstar_battery_charger_event_rcv;

    di->ops = coul_ops;
    retval = hisi_coul_ops_register(coul_ops,COUL_SMARTSTAR);
    if (retval) {
        SMARTSTAR_COUL_ERR("failed to register coul ops\n");
        goto coul_failed_4;
    }

    SMARTSTAR_COUL_EVT("coul probe ok!\n");
    coul_debug_set(1);
	g_pdev = pdev;				/* for debug */
    return retval;


coul_failed_4:
    kfree(coul_ops);
    di->ops = NULL;
coul_failed_3:
    for (i=0; i<IRQ_MAX; i++){
        free_irq(di->irqs[i], di);
    }
    cancel_delayed_work(&di->calculate_soc_delayed_work);
#if BOOST_ENABLE
    cancel_delayed_work(&di->boost_check_delayed_work);
#endif
coul_failed_2:
	cancel_delayed_work(&di->notifier_work);
coul_failed_1:
    platform_set_drvdata(pdev, NULL);
    kfree(di);
    g_smartstar_coul_dev = NULL;
coul_failed_0:
    smartstar_disable_coul(di);
coul_failed:
    SMARTSTAR_COUL_ERR("Smartstar porbe failed!\n");
    return retval;
}

static int __devexit hisi_smartstar_coul_remove(struct platform_device *pdev)
{
    struct smartstar_coul_device *di = platform_get_drvdata(pdev);

#if RBATT_ADJ
    if (di->batt_soc <= 2){
        int ocv_invalid = 0;
        short ocvreg = 0;
        if (di->rbatt_ratio >= 400){
            ocv_invalid = 1;
            SMARTSTAR_COUL_EVT("rbatt_ratio(%d) > 400, mark save ocv is invalid\n", di->rbatt_ratio);
        }
        if (di->batt_delta_rc){
            ocv_invalid = 1;
            SMARTSTAR_COUL_EVT("batt_delta_rc=%d, mark save ocv is invalid\n", di->batt_delta_rc);
        }

        if (ocv_invalid){
            SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
        }
    }
#endif

    //hisi_coul_ops_unregister(di->ops);
    kfree(di->ops);
    di->ops = NULL;
    kfree(di);
    g_smartstar_coul_dev = NULL;

    return 0;
}

#ifdef CONFIG_PM
static int hisi_smartstar_coul_suspend(struct platform_device *pdev,
	pm_message_t state)
{
    struct smartstar_coul_device *di = platform_get_drvdata(pdev);
    long current_sec = get_coul_time();
    long wakeup_time = 0;

    DI_LOCK();
    di->suspend_cc = calculate_cc_uah();
    di->suspend_time = current_sec;
    di->sr_suspend_time = current_sec;

    wakeup_time = current_sec - di->sr_resume_time;
    if (wakeup_time > SR_MAX_RESUME_TIME) {
        clear_sr_time_array();
        SMARTSTAR_COUL_INF("[SR]%s(%d): wakeup_time(%d) > SR_MAX_RESUME_TIME(%d)\n", __func__, __LINE__, wakeup_time, SR_MAX_RESUME_TIME);
    }
    else if (wakeup_time >= 0) {
        sr_time_wakeup[sr_index_wakeup] = wakeup_time;
        sr_index_wakeup++;
        sr_index_wakeup = sr_index_wakeup % SR_ARR_LEN;
    }
    else {
        SMARTSTAR_COUL_ERR("[SR]%s(%d): wakeup_time=%d, di->sr_suspend_time=%d, di->sr_resume_time=%d\n",
            __func__, __LINE__, wakeup_time, di->sr_suspend_time, di->sr_resume_time);
    }

    sr_cur_state = SR_DEVICE_SLEEP;
    DI_UNLOCK();
	SMARTSTAR_COUL_INF("SUSPEND! cc=%d, time=%d\n", di->suspend_cc,
					   di->suspend_time);
    if (di->batt_exist){
        cancel_delayed_work_sync(&di->calculate_soc_delayed_work);
#if BOOST_ENABLE
        set_boost_enable(0);
        cancel_delayed_work_sync(&di->boost_check_delayed_work);
#endif
    }
    cali_adc();
    set_low_vol_int_reg(di, LOW_INT_STATE_SLEEP);
    return 0;
}

/* calculate last SR_TOTAL_TIME seconds duty ratio */
static long sr_get_duty_ratio(void) {
    long total_sleep_time = 0;
    long total_wakeup_time = 0;
    int last_sleep_idx  = (sr_index_sleep - 1 < 0) ? SR_ARR_LEN - 1 : sr_index_sleep - 1;
    int last_wakeup_idx = (sr_index_wakeup - 1 < 0) ? SR_ARR_LEN - 1 : sr_index_wakeup - 1;
    int cnt = 0;
    long duty_ratio = 0;

    do {
        total_sleep_time += sr_time_sleep[last_sleep_idx];
        total_wakeup_time += sr_time_wakeup[last_wakeup_idx];

        last_sleep_idx = (last_sleep_idx - 1 < 0) ? SR_ARR_LEN - 1 : last_sleep_idx - 1;
        last_wakeup_idx = (last_wakeup_idx - 1 < 0) ? SR_ARR_LEN - 1 : last_wakeup_idx - 1;

        cnt++;
        if (cnt >= SR_ARR_LEN) {
            break;
        }
    } while (total_sleep_time + total_wakeup_time < SR_TOTAL_TIME);

    /* calculate duty ratio */
    if (total_sleep_time + total_wakeup_time >= SR_TOTAL_TIME) {
        duty_ratio = total_sleep_time * 100 / (total_sleep_time + total_wakeup_time);
        SMARTSTAR_COUL_INF("[SR]%s(%d): total_wakeup=%ds, total_sleep=%ds, duty_ratio=%d\n",
            __func__, __LINE__, total_wakeup_time, total_sleep_time, duty_ratio);
    }

    return duty_ratio;
}


static int sr_need_update_ocv(struct smartstar_coul_device *di) {
    long last_wakeup_time = 0;
    long last_sleep_time = 0;
    long duty_ratio = 0;

    /* get last wakeup time */
    if (sr_index_wakeup >= 0 && sr_index_wakeup < SR_ARR_LEN) {
        last_wakeup_time = (sr_index_wakeup - 1 < 0) ? sr_time_wakeup[SR_ARR_LEN - 1]: sr_time_wakeup[sr_index_wakeup - 1];
    }

    /* get last sleep time */
    if (sr_index_sleep >= 0 && sr_index_sleep < SR_ARR_LEN) {
        last_sleep_time = (sr_index_sleep - 1 < 0) ? sr_time_sleep[SR_ARR_LEN - 1]: sr_time_sleep[sr_index_sleep - 1];
    }

    /* get last SR_TOTAL_TIME seconds duty ratio */
    duty_ratio = sr_get_duty_ratio();

    /* judge if need update ocv */
    if (last_sleep_time > SR_DELTA_SLEEP_TIME &&
        last_wakeup_time < SR_DELTA_WAKEUP_TIME &&
        duty_ratio > SR_DUTY_RATIO ) {
        SMARTSTAR_COUL_INF("[SR]%s(%d): need_update, last_sleep=%ds, last_wakeup=%ds, duty_ratio=%d\n",
            __func__, __LINE__, last_sleep_time, last_wakeup_time, duty_ratio);
        return 1;
    }
    else {
        SMARTSTAR_COUL_INF("[SR]%s(%d): no_need_update, last_sleep=%ds, last_wakeup=%ds, duty_ratio=%d\n",
            __func__, __LINE__, last_sleep_time, last_wakeup_time, duty_ratio);
        return 0;
    }
}

static int ocv_could_update(struct smartstar_coul_device *di)
{
    long sleep_cc, sleep_current = 0;
    long sleep_time, time_now;

    sleep_cc = calculate_cc_uah();
    sleep_cc = sleep_cc - di->suspend_cc;
    time_now = get_coul_time();
    sleep_time = time_now - di->suspend_time;

    if (sleep_time < delta_sleep_time && !sr_need_update_ocv(di))
    {
        SMARTSTAR_COUL_INF("[SR]Can't update ocv, sleep_current=%d ma, sleep_time=%d s\n", sleep_current, sleep_time);
        return 0;
    }

    /* ma = ua/1000 = uas/s/1000 = uah*3600/s/1000 = uah*18/(s*5) */
    if (sleep_time > 0) {
        sleep_current = (sleep_cc * 18) / (sleep_time * 5);

        if (sleep_current > delta_sleep_current)
        {
            SMARTSTAR_COUL_INF("[SR]Can't update ocv, sleep_current=%d ma, sleep_time=%d s\n", sleep_current, sleep_time);
            return 0;
        }
    }
    else {
        SMARTSTAR_COUL_ERR("[SR]%s(%d): sleep_time = %d\n",  __func__, __LINE__, sleep_time);
        return 0;
    }

    SMARTSTAR_COUL_INF("[SR]going to update ocv, sleep_time=%ds, sleep_current=%d ma\n", sleep_time, sleep_current);
    return 1;
}

static int hisi_smartstar_coul_resume(struct platform_device *pdev)
{
    struct smartstar_coul_device *di = platform_get_drvdata(pdev);
    long current_sec = get_coul_time();
    long sr_sleep_time = current_sec - di->sr_suspend_time;
	SMARTSTAR_COUL_INF("RESUME!\n");
    disable_temperature_debounce = 0;
    set_low_vol_int_reg(di, LOW_INT_STATE_RUNNING);
    DI_LOCK();
    sr_cur_state = SR_DEVICE_WAKEUP;
    di->sr_resume_time = current_sec;
    if (sr_sleep_time >= 0) {
        sr_time_sleep[sr_index_sleep] = sr_sleep_time;
        sr_index_sleep++;
        sr_index_sleep = sr_index_sleep % SR_ARR_LEN;
    }
    else {
        SMARTSTAR_COUL_ERR("[SR]%s(%d): sr_sleep_time = %d\n", __func__, __LINE__, sr_sleep_time);
    }

    if (ocv_could_update(di)){
        get_ocv_by_vol(di);
    }
    else if (di->batt_delta_rc > di->batt_data->fcc*5*10
        && di->charging_state != CHARGING_STATE_CHARGE_START
        && (current_sec - di->charging_stop_time > 30*60)){
        int old_ocv = di->batt_ocv;
    	SMARTSTAR_COUL_EVT("Update ocv for delta_rc(%d)!\n", di->batt_delta_rc);
        get_ocv_by_vol(di);
        if (old_ocv != di->batt_ocv){
            short ocvreg = convert_uv2regval(di->batt_ocv);
            ocvreg = ocvreg | 0x8000;
            udelay(100);
            SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
            di->batt_ocv_valid_to_refresh_fcc = 0;
        }
    }

    di->soc_limit_flag = 2;
    di->batt_soc = calculate_state_of_charge(di);
    di->soc_limit_flag = 1;

    DI_UNLOCK();

    if (di->batt_exist){
        schedule_delayed_work(&di->calculate_soc_delayed_work,
                    round_jiffies_relative(msecs_to_jiffies(CALCULATE_SOC_MS/2)));
#if BOOST_ENABLE
        set_boost_enable(1);
        schedule_delayed_work(&di->boost_check_delayed_work,
                    round_jiffies_relative(msecs_to_jiffies(1000)));
#endif

    }

    return 0;
}
#endif

static struct platform_driver hisi_smartstar_coul_driver = {
	.probe		= hisi_smartstar_coul_probe,
	.remove		= __devexit_p(hisi_smartstar_coul_remove),
#ifdef CONFIG_PM
	.suspend	       = hisi_smartstar_coul_suspend,
	.resume		= hisi_smartstar_coul_resume,
#endif
	.driver		= {
	.name		= "hisi_smartstar_coul",
	},
};
#define NV_FACTORY_MODE    (36)
#define NV_POWERUP_MODE    (50364)
/*判断当前软件是否是烧片版本*/
unsigned int smartstar_coul_is_ftm_mode(void)
{
    int rt = 0;
    unsigned int is_ftm_mode = 0;

    rt = bsp_nvm_read(NV_FACTORY_MODE, &is_ftm_mode, sizeof(unsigned int));

    if(0 != rt)
    {
        SMARTSTAR_COUL_ERR("read ftm mode NV failed!!!\r\n ");
        return 0;
    }
    else
    {
        SMARTSTAR_COUL_ERR("read ftm mode NV success,mode=%d\r\n ",is_ftm_mode);
        return (!is_ftm_mode);
    }
}
/*判断是否是不带电池开机*/
unsigned int smartstar_coul_is_no_battery_powerup_enable(void)
{
    int rt = 0;
    POWERUP_MODE_TYPE powerup_mode_value;

    memset( (void *)&powerup_mode_value, 0, sizeof(POWERUP_MODE_TYPE));
    rt = bsp_nvm_read(NV_POWERUP_MODE, &powerup_mode_value, sizeof(POWERUP_MODE_TYPE));
    if(0 != rt)
    {
        SMARTSTAR_COUL_ERR("read powerup NV failed!!!\r\n ");
        return 0;
    }
    else
    {
        SMARTSTAR_COUL_ERR("no_battery_powerup_enable=%d\r\n ",
        powerup_mode_value.no_battery_powerup_enable);
        return powerup_mode_value.no_battery_powerup_enable;
    }    
}
int __init hisi_smartstar_coul_init(void)
{
    if(is_in_update_mode())
    {
        return;
    }
    if(smartstar_coul_is_ftm_mode())
    {
        return;
    }
    if(smartstar_coul_is_no_battery_powerup_enable())
    {
        return;
    }
    return (platform_driver_register(&hisi_smartstar_coul_driver));
}

void __exit hisi_smartstar_coul_exit(void)
{
    platform_driver_unregister(&hisi_smartstar_coul_driver);
}


fs_initcall(hisi_smartstar_coul_init);
module_exit(hisi_smartstar_coul_exit);

MODULE_AUTHOR("HUAWEI");
MODULE_DESCRIPTION("hisi coul module driver");
MODULE_LICENSE("GPL");


