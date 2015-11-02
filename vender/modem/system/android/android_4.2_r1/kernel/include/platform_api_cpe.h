/* =====================================================================================
  Copyright 漏,    1988-2009, Huawei Tech. Co., Ltd.
 
  File name:      platform_api.h
 
  Author:         w00163299

  Version:        1.0

  Date:           2012/1/1 
  
  Description:   Public platform api for applications 
*    日期        问题单号            修改人       问题描述                      
 ===================================================================================== */

#ifndef PLATFORM_API_CPE_H
#define PLATFORM_API_CPE_H

#ifndef O_RDWR
#define O_RDWR		02
#endif

//#include <stdint.h>
//#include <stdbool.h>

/***************在线升级reboot接口头文件内容************/

#ifdef MBB_FEATRUE_BCMNETLINK

#define NETLINK_SYSWATCH (31)

#else

#define NETLINK_SYSWATCH (20)/*netlink ID*/
#endif





/******************RNDIS端口切换接口头文件内容*****************/
enum ioctl_type
{
	ENUM_FORCE_SWITCH_PROJECT_MODE = 0,
	ENUM_FORCE_SWITCH_DEBUG_MODE    = 1,
	ENUM_USB_RNDIS_SET_RATE         = 3
};

/******************breathled呼吸灯头文件内容*****************/
typedef enum
{
    LED_STATE_LIGHT = 0, //呼吸灯常量
    LED_STATE_SLEEP = 1, //呼吸灯灭
    LED_STATE_SPARK = 2,  //呼吸灯闪烁
    BREATH_LED_STATE_ENABLE  = 3, //呼吸灯使能
    BREATH_LED_STATE_DISABLE = 4, //呼吸灯不使能
    BREATH_LED_STATE_MAX
}LED_STATE;
/********************DRVSTARTMODE接口头文件内容***************/

typedef enum DRV_START_MODE_tag
{
    DRV_START_MODE_EXCEPTION,  /* 异常开机模式，此时大部分业务都不启动, 仅提示信息 */
    DRV_START_MODE_NORMAL,   /*正常开机*/
    DRV_START_MODE_CHARGING,   /* 充电开机模式, ps 等业务不启动  */  
    DRV_START_MODE_NOAPP,      /*SD卡升级第二次重启后不启动APP*/
    DRV_START_MODE_BUTT
}DRV_START_MODE_ENUM;
 


/********查询电池电量和充电状态接口头文件内容********/

/*电池充电状态--查询到状态以底软为准*/
typedef enum CHARGING_STATE_ENUM_tag
{
    CHARGING_INIT =  -1,
    NO_CHARGING_UP = 0,         /* 开机未充电*/
    CHARGING_UP,               /*开机正在充电 */
    NO_CHARGING_DOWN,          /* 关机未充电 */
    CHARGING_DOWN,             /* 关机充电  */
    NO_BATTERY                 /* 电池不存在 */
}CHARGING_STATE_ENUM;


/* 电池电量*/
typedef enum BATT_LEVEL_ENUM_tag
{   
    BATT_INIT =  -2,
    BATT_LOW_POWER = -1 ,  /*电池低电，*/
    BATT_LEVEL_0,                 /*0格电池电量**/
    BATT_LEVEL_1 ,                /*1格电池电量*/
    BATT_LEVEL_2 ,                 /*2格电池电量*/
    BATT_LEVEL_3 ,               /*3格电池电量*/
    BATT_LEVEL_4  ,              /*4格电池电量*/
    BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;
/*电池高温上报事件*/
typedef enum BATT_TEMP_EVENT_tag
{
   BATT_TEMP_LOW,   /* 电池低温事件 */      
   BATT_TEMP_HIGH,   /* 电池高温事件 */ 
   BATT_TEMP_NORMAL, /* 电池高温解除事件 */
   BATT_TEMP_MAX     /* 如果事件值不小于此值，则为非法事件*/
}BATT_TEMP_EVENT;

/*电池充电阶段*/
typedef enum _CHARGE_TYPE
{
    E_CHARGE_NULL = 0,
    E_CHARGE_TRICKLE = 1, /* 电池处于涓充状态 */      
    E_CHARGE_FAST = 2,   /* 电池处于快速充电状态 */ 
    E_CHARGE_SUPP = 3, /* 电池处于补充电状态 */
    E_CHARGE_MAINT = 4, /* 电池处于满电状态 */
    GPIO_CHARGE_TYPE_MAX  = 0xFF   /* 如果事件值不小于此值，则为非法事件*/
} CHARGE_TYPE;

/*电池充电状态查询结构*/
typedef struct BATT_STATE_tag        
{   
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM      battery_level;
    BATT_TEMP_EVENT      batt_temp_state;
    /*电池电量百分比*/
    signed int              battery_percent;  
    /*电池充电类型*/
    CHARGE_TYPE             charge_type;
    /*预计充满时间*/
    signed int              charge_remain_time;/*  -2 : 表示正在计算  -1： 表示不支持剩余时间，大于等于0 表示充电剩余时间，单位为分钟*/
    /*预留扩展字段*/
    signed int              rever;
}BATT_STATE_T;
/*AF10在位状态枚举结构*/
typedef enum
{
    AF10_STATE_INIT = -1,
	AF10_STATE_IN = 0,
    AF10_STATE_OUT,
}AF10_STATE_ENUM_LIST;
/*对外充电状态枚举*/
typedef enum
{
    CHG_EXTCHAGRE_STATE_INIT = -1,
    CHG_EXTCHAGRE_STATE_STOP = 0,
    CHG_EXTCHAGRE_STATE_START,
    CHG_EXTCHAGRE_STATE_TEMP, 
    CHG_EXTCHAGRE_STATE_MAX
}EXTCHAGRE_STATE_ENUM_LIST;

/*电池对外充电状态查询结构*/
typedef struct BATT_EXTSTATE_tag        
{   
    AF10_STATE_ENUM_LIST  extAF10_state;
    EXTCHAGRE_STATE_ENUM_LIST   extcharging_state;    
}EXTCHAGRE_STATE_T;

typedef enum
{
    PRODUCT_TYPE_E5220 = 0,
    PRODUCT_TYPE_E5221,
    PRODUCT_TYPE_E5151,
    PRODUCT_TYPE_E5756,
    PRODUCT_TYPE_E5251,
    PRODUCT_TYPE_MAX,
}PRODUCT_TYPE_ENUM_LIST;

typedef enum CHG_ON_OFF_LINE_STATUS_tag
{
    STATUS_OFFLINE = 0,//充电器/usb在位
    STATUS_ONLINE = 1, //充电器/usb不在位
}CHG_ON_OFF_LINE_STATUS;

typedef enum CHARGE_TYPE_ENUM_tag
{
    CHARGE_TYPE_NON_CHECK = -1,          /* 还未判断出来连接线类型 */
    CHARGE_TYPE_HW = 0,                      /* 华为充电器             */
    CHARGE_TYPE_USB = 1,                  /* USB或第三方充电器           */
    
}CHARGE_TYPE_ENUM;

typedef struct CHG_STATUS_STRU
{
    CHARGE_TYPE_ENUM chg_type;//充电器类型
    CHG_ON_OFF_LINE_STATUS chg_on_off_line_flag;//是否在位
}CHG_STATUS;
typedef enum CHARGE_STATUS_tag
{
    CHARGE_STATUS_READ_FAIL = -1,  /*上层与底层通信失败*/
    CHARGE_STATUS_OFFLINE = 0,          /* 不在位*/
    CHARGE_STATUS_USB_TYPE = 1,                      /* USB           */
    CHARGE_STATUS_WALL_CHG_TYPE = 2,                  /*  华为充电器          */
}CHARGE_STATUS;

#ifdef FEATURE_HUAWEI_MBB_V7R1
typedef enum        
{   
	BATT_LEVEL_GET       = 0, /*电池充电格数查询*/
	/*1、2两个数字可能是系统保留cmd，故跳过*/
	USB_CHARGE_STATE_GET = 3, /*USB或者充电器在位与否查询*/
	CHG_STATE_MAX
}CHG_IOCTL_CMD;

typedef enum        
{   
	CHG_USB_NONE   = 0, /*充电器不在位*/
	CHG_USB_NON_HW = 1, /*非标准充电器在位*/
	CHG_USB_HW     = 2, /*华为标准充电器在位*/
	CHG_USB_MAX
}CHG_USB_STATE;


/*快速开关机用结构*/
typedef enum        
{   
	PWRCTL_WAKE_LOCK      = 3, /*单板不休眠*/
	PWRCTL_WAKE_UNLOCK    = 4, /*单板可休眠*/
	PWRCTL_SHORT_OFF_MODE = 5, /*进入假关机*/
	PWRCTL_SHORT_ON_MODE  = 6, /*退出假关机或者未进入假关机*/
	PWRCTL_MODE_MAX
}PWRCTL_IOCTL_CMD;
#endif
/******************oled显示相关接口头文件内容************/
enum 
{
    OLED_LIGHT = 3,
    OLED_SLEEP,
    OLED_DIM,
    OLED_RESET,
    OLED_UPDATE,
    OLED_CONTRAST,
    OLED_BRIGHTNESS,
    OLED_POWER_ON,
    OLED_POWER_OFF,
    OLED_DEL_TIMER,
    OLED_SYS_POWER_DOWN,
    OLED_INIT_IMAGE,
    OLED_POWER_OFF_CHARGING_START,/*restart power off charging animation*/
    OLED_POWER_OFF_CHARGING_STOP,/*stop power off charging animation*/
    OLED_GET_FRAME_BUF,
    OLED_WRITE_IMAGE,
    OLED_ALL_WHITE,
    OLED_ALL_BLACK,
    OLED_CMD_MAX
}OLED_IOCTL_CMD;


typedef struct tag_oled_write_data {
	unsigned int x_start;
	unsigned int y_start;
	unsigned int x_offset;
	unsigned int y_offset;
	unsigned char *image;
	unsigned int size;
}oled_write_data;
/*底软与应用的接口，定义ioctl的操作命令码*/
typedef enum _LED_IOCTL_CMD
{
    LED_IOCTL_SET = 0,  /*设置LED灯的状态*/
    LED_IOCTL_GET,      /*获取LED灯的状态*/
    LED_IOCTL_CMD_MAX   /*目前支持的命令个数*/
}LED_IOCTL_CMD;
typedef enum _LED_STATE
{
    LED_STATE_ON = 0,
    LED_STATE_OFF,
    LED_STATE_BLINK,
    LED_STATE_COLOR_BLINK,
    LED_STATE_ALL_BLINK,
    LED_STATE_MAX,
}LED_DEV_STATE;
typedef enum _LED_ID
{
    LED_ID_FIRST = 0,
    LED_ID_WIFI = LED_ID_FIRST,  /* WIFI灯 */
    LED_ID_BAT,                  /* 电池灯 */
    LED_ID_SMS,                  /* 短信灯 */
    LED_ID_CIRCLE,               /* 装饰灯 */
    LED_ID_SIGNAL1,              /* 信号灯1 */
    LED_ID_SIGNAL2,              /* 信号灯2 */
    LED_ID_SIGNAL3,              /* 信号灯3 */
    LED_ID_SLIC,                 /* 语音指示灯 */
    LED_ID_MODE,                 /* 网络制式指示灯 */
    LED_ID_MAX
} LED_ID;

typedef enum _LED_COLOR
{
    LED_COLOR_RED = 0,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_BLUE,
    LED_COLOR_WHITE,
    LED_COLOR_CYAN,
    LED_COLOR_PURPLE,
    LED_COLOR_MAX
}LED_COLOR;
typedef enum _LED_CURRENT
{
    LED_CURRENT_LEVEL0 = 0,
    LED_CURRENT_LEVEL1,
    LED_CURRENT_LEVEL2,
    LED_CURRENT_LEVEL3,
    LED_CURRENT_LEVEL4,
    LED_CURRENT_LEVELMAX
}LED_CURRENT;

/*LED的模式 */
typedef enum _LED_MODE
{
    LED_MODE_NORMAL = 0,
    LED_MODE_SLEEP,
    LED_MODE_MAX
}LED_MODE;

/*底软与应用的接口，定义ioctl的设置和获取操作，所使用的参数类型*/
typedef struct _LED_IOCTL_ARG
{
    LED_DEV_STATE led_dev_state;
    LED_ID led_id;
    LED_COLOR led_color;
    LED_COLOR second_led_color;
    unsigned int duration;
    unsigned int second_duration;
    LED_CURRENT led_current;
    LED_MODE sleep_mode;
}LED_IOCTL_ARG;
typedef enum _NET_CRADLE_STATUS
{
    NET_CRADLE_STATUS_ERR = -1,  /*获取网线在位错误*/
    NET_CRADLE_STATUS_OFF = 0,    /*网线不在位*/
    NET_CRADLE_STATUS_ON = 1    /*网线在位*/
}NET_CRADLE_STATUS;
#ifndef FEATURE_HUAWEI_MBB_V7R1
typedef enum
{
    PWRCTL_WAKE_LOCK = 0,         /*不进入睡眠*/
    PWRCTL_WAKE_UNLOCK = 1,    /*可进入睡眠*/
    PWRCTL_SHORT_OFF_MODE = 5, /*进入假关机*/
    PWRCTL_SHORT_ON_MODE  = 6, /*退出假关机或者未进入假关机*/
    PWRCTL_MODE_MAX,                 /*非法值*/
}PWRCTL_IOCTL_CMD;
#endif

#define FUNC_RETURN_OK   0
#define FUNC_RETURN_ERR  -1
#define HW_NDIS_PORT     0x16
#define HW_DEV_NONE      0xFF
#define HW_MAX_PORT      17
#define en_NV_Item_Huawei_Dynamic_PID_Type 50091

typedef enum
{
    /*当前为NDIS UP的状态*/       
    NDIS_DOWN = 0,
    /*当前为NDIS DOWN的状态*/ 
    NDIS_UP,
    /*其它状态*/
    NDIS_OTHER
}NDIS_STATE_ENUM;

typedef struct
{
    unsigned long   ulNvStatus;
    unsigned char   aucFirstPortStyle[17];   /* 设备切换前端口形态 */
    unsigned char   aucRewindPortStyle[17];  /* 设备切换后端口形态 */
    unsigned char   aucReserved[22];         /* 保留 */
}AT_DYNAMIC_PID_TYPE_STRU;

typedef enum 
{
    USB_CHARGE_INIT = -1, /*初始值*/
    USB_CHARGE_TETHERING = 0, /*充电+通信，默认方式*/
    USB_CHARGE_ONLY = 1, /*插入USB后处于仅充电状态*/   
}USB_CHARGE_POLICY;

/*以下结构体定义为 Samba 打印机名称以及打印机状态*/
#ifndef PRINTER_NAME_MAX
#define PRINTER_NAME_MAX (64)
#endif
#ifndef PRINTER_NUM_MAX
#define PRINTER_NUM_MAX (8)
#endif
#ifndef PRINTER_CMD_LEN
#define PRINTER_CMD_LEN (256)
#endif
typedef struct tagPlatformSambaPrinterInfo
{
	char  printer_name[PRINTER_NAME_MAX];  /*打印机名称*/
	int   printer_id;        /*打印机标识*/
}PRINTER_INFO_ST;

/*以下定义Smaba 打印机当前个数以及每台打印机信息，最大支持8 台打印机*/
typedef struct _PRINTER_INFO
{
	int                    printernum;   // 打印机个数
        PRINTER_INFO_ST  sPrinter[PRINTER_NUM_MAX];   //  每台打印机信息
}PRINTER_INFO;
#ifdef MBB_PLATFORM_BCM4708
int reboot_router(void);
int reboot_modem(void);
#else
/*V3平台和V7平台同时都支持的接口定义*/
int reboot(void);
#endif
int update_modem_state(void);
DRV_START_MODE_ENUM drvStartModeGet( void );
int app_get_battery_state(BATT_STATE_T *battery_state);

int oledIoctrl(unsigned int cmd,  unsigned long arg);
void oledRefresh(unsigned int ulXStart, unsigned int ulYStart, unsigned int ulXOffset, unsigned int ulYOffset, unsigned char *pucBuffer);

void uwd_force_switch_project_mode(void);
void uwd_force_switch_debug_mode(void);
int usb_rndis_set_rate(uint32_t ulRate);

int bsp_led_ctrl(LED_STATE state);
int led_dev_ctrl(LED_IOCTL_CMD cmd_id, LED_IOCTL_ARG *led_arg);
int set_special_timer(unsigned char bStart, unsigned int uTime );


int set_wake_lock(bool bLock);
int set_deep_sleep_mode(bool bSleepMode);

int app_get_product_type_state(PRODUCT_TYPE_ENUM_LIST *product_type);

void lan_pm_power_on(unsigned char power_on);
/*NET_CRADLE_STATUS get_net_line_state(void);*/

#ifndef FEATURE_HUAWEI_MBB_V7R1
/******************** 仅在V3平台支持的接口定义*************************/
CHARGE_STATUS get_usb_charge_status(void);
int set_wake_lock_wps(bool bLock);
#endif

#ifdef FEATURE_HUAWEI_MBB_V7R1
/******************** 仅在V7平台支持的接口定义*************************/
int get_usb_charge_status(void);

int set_powersave_mode( int powersavemode );

int get_powersave_mode( void );

/*int app_get_extchg_AF10_state(EXTCHAGRE_STATE_T *extcharging_state);*/

/**********************************************************************
函 数 名      : sync_before_poweroff
功能描述  : 提供给应用的接口，用于关机前进行文件系统的同步
输入参数  :void
输出参数  : 无。
返 回 值      : int类型:0执行成功；-1执行失败。
注意事项  : 注意该接口调用，需尽可能保证关机流程调用之后，不再进行文件系统写操作。
***********************************************************************/
int sync_before_poweroff(void);
#endif

/*int get_usb_charge_policy(USB_CHARGE_POLICY* chg_policy);
int set_usb_charge_policy(const USB_CHARGE_POLICY chg_policy);*/

int app_samba_get_printerinfo(PRINTER_INFO  *pSamPrinterinfo);
int app_samba_inform_status(int status);
#endif /* PLATFORM_API_H */

