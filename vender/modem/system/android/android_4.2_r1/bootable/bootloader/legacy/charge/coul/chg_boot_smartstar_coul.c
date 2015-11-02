/*****************************************************************************************
* Filename:	hisi_smartstar_coul.c
*
* Discription:  smartstar coulometer functions.
* Copyright: 	(C) 2013 huawei.
*
* revision history:
* 03/25/13 yuanqinshun -v1.0
*
******************************************************************************************/
#include "product_config.h"
#include <boot/boot.h>
#include <balongv7r2/types.h>
#include <balongv7r2/time.h>
#include <bsp_hkadc.h>
#include <bsp_coul.h>
#include "bsp_nvim.h"
#include "bsp_sram.h"
#include "bsp_pmu.h"
#include <stdarg.h>
#include "chg_boot_chip_platform.h"
#include "chg_boot_charge_drv.h"

#define __ddrrun_func
#define coul_dbg  cprintf
#define SMARTSTAR_COUL_ERR(fmt,args...)     do { coul_dbg("[smartstar]" fmt, ## args); } while (0)
#define SMARTSTAR_COUL_DBG(fmt,args...)     do { coul_dbg("[smartstar]" fmt, ## args); } while (0)
#define SMARTSTAR_COUL_INF(fmt,args...)     do { coul_dbg("[smartstar]" fmt, ## args); } while (0)
#define PMUSSI_DELAY()                          udelay(500)

#define SMARTSTAR_REG_READ(regAddr)             pmussi_reg_read((int)(regAddr))
#define SMARTSTAR_REGS_READ(regAddr,buf,size)   (void)pmussi_recv_data((int)(regAddr),(char*)(buf),(int)(size))
#define SMARTSTAR_REGS_WRITE(regAddr,buf,size)  (void)pmussi_send_data((int)(regAddr),(char*)(buf),(int)(size))

#define COUL_WORKING     (0x44)

#define SMARTSTAR_COUL_STATE_REG 0x1A6
#define SMARTSTAR_CL_OUT_BASE       0x139
#define SMARTSTAR_CL_IN_BASE        0x135
#define SMARTSTAR_CHG_TIMER_BASE    0x141
#define SMARTSTAR_LOAD_TIMER_BASE   0x13d
#define SMARTSTAR_SHUTDOWN_TIMER_BASE 0x145
#define SMARTSTAR_OFFSET_VOLTAGE    0x151
#define SMARTSTAR_OCV_REG_BASE      0x153
#define SMARTSTAR_VOL_FIFO_BASE     0x155
#define SMARTSTAR_CUR_FIFO_BASE 0x17d
#define SMARTSTAR_BATTERY_MOVE_ADDR 0x1a8
#define SMARTSTAR_SAVE_OCV_ADDR 0x1a9
#define SMARTSTAR_NV_SAVE_SUCCESS 0x1a7   //use bit 1
#define SMARTSTAR_NV_READ_SUCCESS 0x1a7 //use bit 2

#define BATTERY_MOVE_MAGIC_NUM        0xc3

#define R_COUL_MOHM                 10      /* resisitance mohm */
#define ABNORMAL_BATT_TEMPERATURE_LOW   (-40)
#define ABNORMAL_BATT_TEMPERATURE_HIGH  (80)

#define NV_HUAWEI_COUL_INFO_I    (50462)
extern boolean chg_boot_is_batt_over_discharge(void);
#if 1
#define DEFAULT_V_OFF_A 1000000/1000
#define DEFAULT_V_OFF_B 0/1000
#define DEFAULT_C_OFF_A 1000000/1000
#define DEFAULT_C_OFF_B 0/1000
static int v_offset_a = DEFAULT_V_OFF_A;
static int v_offset_b = DEFAULT_V_OFF_B;
static int c_offset_a = DEFAULT_C_OFF_A;
static int c_offset_b = DEFAULT_C_OFF_B;
static int r_pcb = 0;
#endif


static char __ddrrun_func pmussi_reg_read(int reg_addr)
{
    char regval;

    bsp_hi6551_reg_read(reg_addr, &regval);
    PMUSSI_DELAY();

    return regval;
}

static int __ddrrun_func pmussi_recv_data (int reg_addr, char *buffer, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        bsp_hi6551_reg_read(reg_addr + i, (buffer + i));
        PMUSSI_DELAY();
    }

    return 0;
}

static int __ddrrun_func pmussi_send_data (int reg_addr, char *buffer, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        bsp_hi6551_reg_write( reg_addr + i, *(buffer + i));
        PMUSSI_DELAY();
    }

    return 0;
}


/**
 * clear_cc_register
 *
 * clear the cc register
 */
static void __ddrrun_func clear_cc_register(void)
{
	long ccregval = 0;
    SMARTSTAR_REGS_WRITE(SMARTSTAR_CL_IN_BASE,&ccregval,4);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_CL_OUT_BASE,&ccregval,4);
}

/*
 *	from coul get battery cl_in or cl_out.
 */
static void __ddrrun_func clear_coul_time(void)
{
    unsigned long cl_time = 0;

    SMARTSTAR_REGS_WRITE(SMARTSTAR_CHG_TIMER_BASE,&cl_time ,4);
    SMARTSTAR_REGS_WRITE(SMARTSTAR_LOAD_TIMER_BASE,&cl_time ,4);
}

/**
 * convert_regval2mv
 * 1 bit = 225.1196 uv = 4.8/21322 V = 4.8/21322 * 1000 * 1000 uV = 24 * 1000 * 100/ 10661 uV = 24*100/10661 mV
 * convert regval to mv
 */
static int __ddrrun_func convert_regval2mv(short reg_val)
{
    u32 temp;

    if (reg_val & 0x8000) {
        return -1;
    }

    temp = reg_val * 24 * 100;

    temp = temp/10661;

#if 1 /* for debug */
	temp = v_offset_a *temp;
	temp = temp/1000;
	temp += v_offset_b;
#endif

    return (int)temp;
}

/**
 * smartstar_battery_voltage_mv
 *
 * battery voltage in mv
 */
static int __ddrrun_func smartstar_battery_voltage_mv(void)
{
    short regval, volt_mv;

    SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE, &regval, 2);

    volt_mv = convert_regval2mv(regval);
    
    SMARTSTAR_COUL_INF("read voltage, reg=0x%x, vol=%dmv\n", regval, volt_mv);

    return volt_mv;
}

/**
 * convert_regval2ua
 * 10 mohm resistance: 1 bit = 5/10661 A = 5*1000*1000 / 10661 uA = 5*1000*10 / 10661 mA
 * 20 mohm resistance: 1 bit = 10 mohm / 2
 * 30 mohm resistance: 1 bit = 10 mohm / 3
 * ...
 * high bit = 0 is in, 1 is out
 * convert regval to ua
 */
static  int convert_regval2ma(short reg_val)
{
    int ret;
    int temp;

    ret = reg_val;
    temp = ret*1000*10*5;
    temp = temp/10661;

    ret = temp / (R_COUL_MOHM/10);

    ret = -ret;

#if 1 /* for debug */
	temp = c_offset_a*ret;
	ret = temp/1000;
	ret += c_offset_b;
#endif

    return ret;
}

/**
 * smartstar_battery_current_ua - return the current of battery.
 *
 * return the battery current in uA.
 */
int smartstar_battery_current_ma(void)
{
    short regval;

    SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE, &regval, 2);

    return convert_regval2ma(regval);
}

static void __ddrrun_func get_simultaneous_battery_voltage_and_current(int *ibat_ma, int *vbat_mv)
{
    int vol[3], cur[3];
    int i;
    for (i=0; i<3; i++){
        vol[i] = smartstar_battery_voltage_mv();
        cur[i] = smartstar_battery_current_ma();
    }

    if (vol[0]==vol[1] && cur[0]==cur[1]){
        *ibat_ma = cur[0];
        *vbat_mv = vol[0];
    } else if (vol[1]==vol[2] && cur[1]==cur[2]){
        *ibat_ma = cur[1];
        *vbat_mv = vol[1];
    } else {
        *ibat_ma = cur[2];
        *vbat_mv = vol[2];
    }

    *vbat_mv += *ibat_ma/1000*r_pcb/1000;

}


int __ddrrun_func volt_to_temp(int voltage){
#define ARRAYLENGTH 31

int BattArray[][2] =
        {
            {-273, 2600}, {-40, 2380}, {-36, 2331}, {-32, 2275}, {-28, 2210},
            {-24, 2137}, {-20, 2057}, {-16, 1969}, {-12, 1875}, {-8, 1775},
            {-4, 1671}, {0, 1565}, {4, 1457}, {8, 1350}, {12, 1244},
            {16, 1142}, {20, 1044}, {24, 951}, {28, 863}, {32, 781},
            {36, 705}, {40, 636}, {44, 572}, {48, 515}, {52, 462},
            {56, 415}, {60, 372}, {64, 335}, {74, 257}, {84, 199},
            {125, 74}, {0, 0},
        };


	int temperature = 0;
	int index = 0;
	if(voltage >= BattArray[0][1])
		return BattArray[0][0];
	if(voltage <= BattArray[ARRAYLENGTH -1][1])
		return BattArray[ARRAYLENGTH -1][0];
	for(index = 0; index < ARRAYLENGTH; index++){
		if(voltage == BattArray[index][1])
			return BattArray[index][0];
		if(voltage > BattArray[index][1]){
			break;
		}
	}
	temperature = BattArray[index][0] + (int)((float)(voltage - BattArray[index][1])*BattArray[index-1][0]-(voltage - BattArray[index][1])*BattArray[index][0])/(BattArray[index-1][1] - BattArray[index][1] + 0.5);
	return temperature;
}

#define ADC_BATT_TEMP 2
#define INVALID_TEMP -99
int __ddrrun_func get_battery_temperature(void)
{
    int temp;
    temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);
    SMARTSTAR_COUL_INF("batt temp=%d\n", temp);
    return temp;
}

static int __ddrrun_func smartstar_battery_temperature(void)
{
    /* Power on */
    int temp;
    temp = chg_boot_get_temp_value(CHG_PARAMETER__BATT_THERM_DEGC);

    SMARTSTAR_COUL_INF("smartstar_battery_temperature batt temp=%d\n", temp);

    return temp;
}

#define ADC_BATT_ID  13
#define INVALID_BATT_ID_VOL  -999
/*电池ID可以根据产品宏来区分*/
#if defined(BSP_CONFIG_BOARD_E5_DCM)
#define DEFAULT_BATT_ID_VOL  2222//feimaotui,for 1780mAh
#endif
static int __ddrrun_func smartstar_battery_id_voltage(void)
{
    /* Power on */
    unsigned short volt = 0;
    //目前E5不支持电池ID读取，先通过产品宏定义，后续通过hkadc读取
#if defined(BSP_CONFIG_BOARD_E5_DCM)
    volt = DEFAULT_BATT_ID_VOL;
#endif

    SMARTSTAR_COUL_INF("hkadc_bbp_convert read battery id voltage return volt=%d\n", volt);

    return (int)volt;
}


#define SS_COUL_NV_NAME  "SS_COUL"
#define SS_COUL_NV_NUM   NV_HUAWEI_COUL_INFO_I
struct ss_coul_nv_info *coul_nv_info = NULL;

static unsigned int __ddrrun_func get_shutdown_time()
{
    unsigned int shutdown_timer = 0;
    SMARTSTAR_REGS_READ(SMARTSTAR_SHUTDOWN_TIMER_BASE, &shutdown_timer, 4);
    return shutdown_timer;
}

static void __ddrrun_func clear_shutdown_time()
{
    unsigned int shutdown_timer = 0;
    SMARTSTAR_REGS_WRITE(SMARTSTAR_SHUTDOWN_TIMER_BASE, &shutdown_timer, 4);
}

/*
 *	from coul get battery cl_in or cl_out.
 */
static void show_coul_time_reg(void)
{
    unsigned long cl_in_time, cl_out_time;

    SMARTSTAR_REGS_READ(SMARTSTAR_CHG_TIMER_BASE,&cl_in_time,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_LOAD_TIMER_BASE,&cl_out_time,4);

    SMARTSTAR_COUL_INF("cl_in_time=%d, cl_out_time=%d\n", cl_in_time, cl_out_time);
}

/**
 * smartstar_battery_voltage
 *
 * battery voltage in milivolts
 */
static void show_cc_reg(void)
{
    unsigned long cc_in = 0;
    unsigned long cc_out = 0;
    SMARTSTAR_REGS_READ(SMARTSTAR_CL_IN_BASE,&cc_in,4);
    SMARTSTAR_REGS_READ(SMARTSTAR_CL_OUT_BASE,&cc_out,4);

    SMARTSTAR_COUL_INF("cc_in_reg=%d, cc_out_reg=%d\n", cc_in, cc_out);
}
static int __ddrrun_func is_battery_moved()
{
    unsigned char val;
    val = SMARTSTAR_REG_READ(SMARTSTAR_BATTERY_MOVE_ADDR);

    if (val == BATTERY_MOVE_MAGIC_NUM || val == (BATTERY_MOVE_MAGIC_NUM+1)){
        return 0;
    }
    else {
        val = BATTERY_MOVE_MAGIC_NUM+1;
        SMARTSTAR_REGS_WRITE(SMARTSTAR_BATTERY_MOVE_ADDR, &val, 1);
        return 1;
    }
}

#define FLAG_USE_CLAC_OCV 0xABCD
#define FLAG_USE_PMU_OCV 0xCDEF
static int __ddrrun_func is_saved_ocv_valid(int is_calc_ocv_valid)
{
    short ocvreg;
    int ocv;
    char val;

    SMARTSTAR_REGS_READ(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
    
    val = SMARTSTAR_REG_READ(SMARTSTAR_NV_SAVE_SUCCESS);
    SMARTSTAR_COUL_INF("ocv valid info:ocvreg = 0x%4x,nv_save = 0x%x\n", ocvreg,val);
    if((val & 0x02) != 0x02)
    {
        return 0;
    }

    if (ocvreg==0 || ocvreg==FLAG_USE_CLAC_OCV || ocvreg==FLAG_USE_PMU_OCV){
        return 0;
    }

    if (ocvreg&0x8000 && is_calc_ocv_valid){
        ocvreg = ocvreg & ~0x8000;
    }

    ocv = convert_regval2mv(ocvreg);
#if 0
    if (ocv<3000 || ocv>4500){
        return 0;
    }
#endif

    return 1;
}
static void __ddrrun_func show_saved_ocv_time_cc()
{
    unsigned short ocvreg;
    int ocv;

    SMARTSTAR_REGS_READ(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);

    ocv = convert_regval2mv(ocvreg);

    SMARTSTAR_COUL_INF("saved ocv reg=0x%x ocv=%dmv\n", ocvreg, ocv);
    show_coul_time_reg();
    show_cc_reg();
}

static void __ddrrun_func save_flag_for_calc_ocv()
{
    short ocvreg = FLAG_USE_CLAC_OCV;
    SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
}
static void __ddrrun_func save_flag_for_pmu_ocv()
{
    short ocvreg = FLAG_USE_PMU_OCV;
    SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
}

static int nv_read_success = 0;
static int __ddrrun_func save_nv()
{
    if (!nv_read_success){
        return -1;
    }

    memcpy((unsigned int *)SRAM_COUL_ADDR, coul_nv_info, sizeof(*coul_nv_info));
    return 0;
}

#define REG_VAL_5_MV 22 /* 1 bit = 225.1196 uv; 5 mv = 22.210 bit*/
#define REG_VAL_3_MV 13 /* 1 bit = 225.1196 uv; 3 mv = 13.326 bit*/
static int delta_ocv = 0;
static int __ddrrun_func save_pmu_ocv()
{
    short ocvreg,volreg_offset;

    SMARTSTAR_REGS_READ(SMARTSTAR_OCV_REG_BASE,&ocvreg,2);
    SMARTSTAR_REGS_READ(SMARTSTAR_OFFSET_VOLTAGE,&volreg_offset,2);
    ocvreg -= volreg_offset;
    ocvreg += delta_ocv;

    SMARTSTAR_REGS_WRITE(SMARTSTAR_SAVE_OCV_ADDR,&ocvreg,2);
    coul_nv_info->calc_ocv_reg_v = 0;
    coul_nv_info->calc_ocv_reg_c = 0;
    coul_nv_info->start_cc = 0;
    coul_nv_info->hkadc_batt_temp = (short)smartstar_battery_temperature();
    coul_nv_info->hkadc_batt_id_voltage = (short)smartstar_battery_id_voltage();
    coul_nv_info->ocv_temp = smartstar_battery_temperature()*10;
    SMARTSTAR_COUL_INF("save pmu ovc fastboot read batt temp =%d , batt id voltage=%d\n", \
                        coul_nv_info->hkadc_batt_temp, coul_nv_info->hkadc_batt_id_voltage);
    return save_nv(); 
}
static void __ddrrun_func enable_charger(void)
{
    (void)chg_boot_set_charge_enable(1);
}
static void __ddrrun_func disable_charger(void)
{
    (void)chg_boot_set_charge_enable(0);
}

static int __ddrrun_func save_calc_ocv()
{
    short v_reg, c_reg;

    disable_charger();
    sleep(1320); /* 1210ms + 110ms */

    SMARTSTAR_REGS_READ(SMARTSTAR_VOL_FIFO_BASE, &v_reg, 2);
    SMARTSTAR_REGS_READ(SMARTSTAR_CUR_FIFO_BASE, &c_reg, 2);
    
    enable_charger();

    coul_nv_info->calc_ocv_reg_v = v_reg;
    coul_nv_info->calc_ocv_reg_c = c_reg;
    coul_nv_info->start_cc = 0;
    coul_nv_info->hkadc_batt_temp = (short)smartstar_battery_temperature();
    coul_nv_info->hkadc_batt_id_voltage = (short)smartstar_battery_id_voltage();
    coul_nv_info->ocv_temp = smartstar_battery_temperature()*10;

    SMARTSTAR_COUL_INF("save calc ocv fastboot read batt temp =%d , batt id voltage=%d\n",\
                        coul_nv_info->hkadc_batt_temp, coul_nv_info->hkadc_batt_id_voltage);
    SMARTSTAR_COUL_INF("v_reg=%d, c_reg=%d\n", v_reg, c_reg);

    return save_nv();
        
}
static int __ddrrun_func use_pmu_ocv()
{
    /* pmu ocv is valid, save it and clear cc and time */
    clear_cc_register();
    clear_coul_time();
    save_flag_for_pmu_ocv();
    return save_pmu_ocv();
}
static int pmu_ocv_valid = 1;
static int __ddrrun_func use_calc_ocv()
{
    clear_cc_register();
    clear_coul_time();
    save_flag_for_calc_ocv();
    if (save_calc_ocv()){
        SMARTSTAR_COUL_ERR("ignore smartstar coul nv error!\n");
    }
    return 0;
}
static int __ddrrun_func use_saved_ocv(int is_calc_ocv_valid)
{
    if(is_saved_ocv_valid(is_calc_ocv_valid)){
    SMARTSTAR_COUL_INF("use saved ocv\n");

        coul_nv_info->hkadc_batt_temp = smartstar_battery_temperature();
        coul_nv_info->hkadc_batt_id_voltage = smartstar_battery_id_voltage();
        /* don't clean cc and time, kernel will use them and saved ocv to calc soc */
        return save_nv();
    }
    else{
        return use_calc_ocv();
    }
}
int __ddrrun_func get_ss_use_pmu_ocv(void)
{
    /*上电后，默认不是能充电，PMU OCV有效，手机需要硬件改版后生效*/
    /*判断电池是否过放，如果电池过放，则PMU OCV无效*/
    if(chg_boot_is_batt_over_discharge())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int __ddrrun_func get_charger_state(void)
{
    //目前不支持fastboot阶段写NV，因此不区分场景，统一使用PMU读取的OCV
    return 1;
}

#define SAVED_OCV_VALID_TIME    (60*60*24)
#define OCV_VALID_TIME    (60*60*5/2)
static void __ddrrun_func get_initial_ocv(void)
{
    int error = 0;
    int charger_state, new_battery;
    unsigned int shutdown_time;
    
    pmu_ocv_valid = get_ss_use_pmu_ocv();

    shutdown_time = get_shutdown_time();
    clear_shutdown_time();

    new_battery = is_battery_moved();
    charger_state = get_charger_state();

    show_cc_reg();
    show_coul_time_reg();

    SMARTSTAR_COUL_INF("new_battery=%d, charger_state=%d, shutdown_time=%d\n", new_battery, charger_state, shutdown_time);

    if (!charger_state){
        /* power up by press powerkey, ocv be pulled down */
        delta_ocv = REG_VAL_5_MV;
    }
    else{
        /* power up by usb, ocv be pulled up, value tbd */
        delta_ocv = 0; //REG_VAL_3_MV;
    }

    if (new_battery){
        if (charger_state && !pmu_ocv_valid){
            error = use_calc_ocv();
        }
        else{
            error = use_pmu_ocv();
        }
    }
    else{
        if (shutdown_time > SAVED_OCV_VALID_TIME)
        {
            if (charger_state && !pmu_ocv_valid){
                error = use_calc_ocv();
            }
            else{
                error = use_pmu_ocv();
            }
        }
        else if (shutdown_time > OCV_VALID_TIME)
        {
            if (charger_state && !pmu_ocv_valid){
                error = use_saved_ocv(0);
            }
            else{
                error = use_pmu_ocv();
            }
        }
        else{
            error = use_saved_ocv(1);
        }
    }

    if (error){
        #if 0
        /* save a flag to notify kernel just use pmu ocv */
        save_flag_for_nv_error();
        clear_cc_register();
        clear_coul_time();
        #else
        use_calc_ocv();
        #endif
    }

    show_saved_ocv_time_cc();
    return;
}

/* Function: 	wait_ss_coul_device_work(void)
*  Discription:	wait the smart star coul device work normal
*  Parameters:	NULL
*  return value:NULL
*/
int __ddrrun_func wait_ss_coul_device_work(void)
{

    int tryloop = 0,ret;
    char regval;


    coul_nv_info = alloc(sizeof(*coul_nv_info));
    memset(coul_nv_info, 0, sizeof(*coul_nv_info));

    ret = bsp_nvm_read(NV_HUAWEI_COUL_INFO_I, (u8*)coul_nv_info, sizeof(*coul_nv_info));
    if(ret)
    {
        SMARTSTAR_COUL_ERR("read nv %s(%d) failed !\n", SS_COUL_NV_NAME, SS_COUL_NV_NUM);
        regval = SMARTSTAR_REG_READ(SMARTSTAR_NV_READ_SUCCESS);
        regval &= 0xFA;
        SMARTSTAR_REGS_WRITE(SMARTSTAR_NV_READ_SUCCESS, &regval, 1);
        nv_read_success = 0;    
    }
    else
    {
        /*需要使用校准NV来得出v_offset_a和v_offset_b*/
        v_offset_a = coul_nv_info->v_offset_a == 0 ? DEFAULT_V_OFF_A : coul_nv_info->v_offset_a / 1000;
        v_offset_b = coul_nv_info->v_offset_b == 0 ? DEFAULT_V_OFF_B : coul_nv_info->v_offset_b / 1000;
        c_offset_a = coul_nv_info->c_offset_a == 0 ? DEFAULT_C_OFF_A : coul_nv_info->c_offset_a / 1000;
        c_offset_b = coul_nv_info->c_offset_b == 0 ? DEFAULT_C_OFF_B : coul_nv_info->c_offset_b / 1000;
        r_pcb = coul_nv_info->r_pcb;
        regval = SMARTSTAR_REG_READ(SMARTSTAR_NV_READ_SUCCESS);
        regval |= 0x4;
        SMARTSTAR_REGS_WRITE(SMARTSTAR_NV_READ_SUCCESS, &regval, 1);
        nv_read_success = 1;
        SMARTSTAR_COUL_INF("read NV_HUAWEI_COUL_INFO_I success\n");
    }

    do {
        regval = SMARTSTAR_REG_READ(0x0);
        SMARTSTAR_COUL_INF("do a dummy read, smartstar version is 0x%x\n",
                            regval);

        if ((tryloop++) > 5)
            return -1;

    } while(regval != 0x20);

    tryloop=0;
    do {
        regval = SMARTSTAR_REG_READ(SMARTSTAR_COUL_STATE_REG);

        if ((tryloop++) > 15){
            SMARTSTAR_COUL_ERR("wait coul work timeout, reg = 0x%x\n",
                                regval);
            return -1;
        }
        sleep(110); /* sleep 110 ms more to get a valid voltage/current value */
    } while(regval != COUL_WORKING);

    get_initial_ocv();

    return 0;
}


/* Function: 	get_ss_bat_voltage(void)
*  Discription:	get the battery voltage
*  Parameters:	NULL
*  return value:the data of the battery
*/
int __ddrrun_func get_ss_bat_voltage(void)
{
    int ibat_ma, vbat_mv;
    get_simultaneous_battery_voltage_and_current(&ibat_ma, &vbat_mv);
    return vbat_mv;
}


/* Function: 	 get_ss_bat_temperature(void)
*  Discription:	 get the battery temperature
*  Parameters:	 NULL
*  return value: the temperature of the battery
*/
int __ddrrun_func get_ss_bat_temperature(void)
{
    return smartstar_battery_temperature();
}

/**
 * is_smartstar_battery_exist -
 *
 * 0:battery isn't exist, 1: exist
 */
int __ddrrun_func is_smartstar_battery_exist(void)
{
    int temp;

    temp = smartstar_battery_temperature();
    if ((temp >= ABNORMAL_BATT_TEMPERATURE_LOW)
        && (temp <= ABNORMAL_BATT_TEMPERATURE_HIGH)) {
        return 1;
    } else {
        return 0;
    }
}


/* Function: 	 check_ss_bat_present(void)
*  Discription:
*  Parameters:	 NULL
*  return value: battery present or not
*/
int __ddrrun_func check_ss_bat_present(void)
{
    return is_smartstar_battery_exist();
}

