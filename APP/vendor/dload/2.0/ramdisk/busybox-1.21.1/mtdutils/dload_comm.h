/******************************************************************************

                  版权所有 (C), 2001-2014, 华为技术有限公司

******************************************************************************
  文 件 名   : dload_comm.h
  版 本 号   : 初稿
  作    者   : w00183456
  生成日期   : 2014年2月28日
  最近修改   :
  功能描述   : 申明定义共用变量/宏/结构体
  函数列表   :
  修改历史   :
*********************************问题单修改记录******************************
  问题单号               修改人         修改时间           修改说明                             
  DTS2014090106291      c00227249      2014-09-02         同步升级归一化修改
                   
******************************************************************************/
#ifndef _DLOAD_COMM_H_
#define _DLOAD_COMM_H_
/******************************************************************************
  1 其他头文件包含
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "dload_debug.h"

#ifdef  __cplusplus
    extern "C"{
#endif

/****************************************************************************
  2 变量类型重定义
*****************************************************************************/
typedef int                int32;
typedef short              int16;
typedef char               int8;
typedef unsigned long long uint64;
typedef long long          int64;
typedef unsigned long      uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef unsigned char      boolean;

/****************************************************************************
 3 枚举类型，结构体，宏定义
 结构体类型加 :_s
 枚举类型加   :_e
 联合体类型加 :_u
****************************************************************************/
#define EXTERN   extern
#define STATIC   static
#define CONST    const

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define DLOAD_ERR        -1
#define DLOAD_OK         0

#define DLOAD_NAME_MAX_LEN    (256)

/* 子模块初始化函数指针类型 */
typedef int32 (*module_interface)(void);

/****************************************************************************
 3 枚举类型，结构体，宏定义
 结构体类型加 :_s
 枚举类型加   :_e
 联合体类型加 :_u
****************************************************************************/
typedef enum
{
    FILE_NONE           =        0x00,  /* No Programming file yet */
    FILE_PARTI          =        0x01,  /* Partition table */
    /* 高通平台分区，归一化编译考虑用宏控制 */
    FILE_SBL1          =        0x02,  /*SBL1*/
    FILE_WDT           =        0x03 , /*WDT*/
    FILE_TZ            =        0x04,  /*TZ*/
    FILE_RPM           =        0x05 , /*RPM*/
    FILE_EFS2          =        0x06,  /*EFS2 for arm*/
    FILE_EFS2_4K       =        0x1C,  /*EFS2 for arm,4K FLASH PAGE适配*/
    FILE_EFS2_2K       =        0x1D,  /*EFS2 for arm,2K FLASH PAGE适配*/
    FILE_MBA            =        0x07,  /*MBA*/
    FILE_QDSP           =        0x08,  /*QDSP*/
    FILE_BOOT           =        0x09,  /*APPBOOT include linux kernel*/
    FILE_ADSP           =        0X0F,  /* */
    FILE_MODEM_CPIO  =          0x1E,  /*modem.cpio*/
    /* Balong 平台分区 */
    FILE_M3BOOT        =        0x20,  /*m3boot*/
    
    FILE_VXWORK        =        0x22,  /*vxwork*/
    FILE_M3IMAGE       =        0x23,  /*m3_firmware*/
    FILE_DSP           =         0x24,  /*DSP*/
    FILE_NVDLOAD       =        0x25,  /*upgrade NV+XML*/
    FILE_NVIMG         =        0x26,  /*NV Work*/


    /* ISO 分区，文件类型值不能修改，是工具打包配套的 */
    FILE_ISO_HD        =        0x0A,  /*ISO header file type*/
    FILE_ISO           =        0x0B,  /*ISO file type :save to MMC Partiton*/

    /* 从0X10 开始预留4个ID ，为不同平增加分区用 */
    FILE_SYSTEM         =         0x10,  /* */
    FILE_FASTBOOT       =        0x11,  /*FASTBOOT: QC  APPSBL include fastboot*/
    FILE_RECOVERY       =        0x12,  /* */
    FILE_LOGO           =         0x13,  /* */
    FILE_RESERVE1       =        0x14,  /*预留分区 1*/
    FILE_RESERVE2       =        0x15,  /*预留分区 2*/
    FILE_RESERVE3       =        0x16,  /*预留分区 3*/
    FILE_WIMAXCFG       =        0x17,  /* */
    FILE_WIMAXCRF       =        0x18,  /* */
    FILE_USERDATA       =        0x19,  /* */
    FILE_OEMINFO        =        0x1A,  /* */
    FILE_BBOU           =        0x1B,  /* */

    /* 下面文件类型是固定不能修改的 */
    FILE_NV_MBN           =      0x50,    /* */
    FILE_XML_MBN          =      0x51,    /* */
    FILE_CUST_MBN         =      0x52,    /* */
    FILE_PRL_NAM0         =      0x53,    /* */
    FILE_PRL_NAM1         =      0x54,    /* */
    FILE_WEBUI_HD         =      0X55,    /*WEBUI header file type*/
    FILE_WEBUI            =      0x56,    /* WEBUI configature file type*/
    FILE_WEBSDK           =      0x57,    /*WEBSDK code file type*/ 

    FILE_WIMAXCFG_CPIO   =      0x58,    /* WIMAXCFG CPIO, balong not used*/
    FILE_SYSTEM_CPIO     =      0x59,    /* SYSTEM CPIO */
    FILE_WEBSDK_CPIO     =      0x5A,    /* WEBSDK CPIO */
    FILE_WEBUI_CPIO      =      0x5B,    /* WEBUI CPIO */
    FILE_WIMAXCRF_CPIO   =      0x5C,    /* WIMAXCRF CPIO ,balong not used*/

    FILE_HOTA             =      0x5D,    /*HOTA update.zip packet file id*/  
    FILE_FIRMWARE1       =      0x5E,    /*ONLINE partition file id*/
    FILE_FOTA_FS         =      0x5F,    /*file system delta file id*/
    FILE_FOTA_FW         =      0x60,    /*firmware delta file id*/
    FILE_DISCARD         =      0xc0,    /*discard all files which file type >= FILE_DISCARD*/
    FILE_MAX
} file_type_e;

/*USB交互命令,PC->UE接受的命令*/
typedef enum
{
    DLOAD_CMD_ACK           = 0x02,    /*正常响应, UE->PC*/
    DLOAD_CMD_RESET         = 0x0A,    /*复位命令, PC->UE*/
    DLOAD_CMD_VERREQ        = 0x0C,    /*软件信息请求命令, PC->UE*/
    DLOAD_CMD_VERRSP        = 0x0D,    /*软件信息返回响应, UE->PC*/
    DLOAD_CMD_SWITCH        = 0x3A,    /*下载模式切换虚拟命令, UE<->PC*/
    DLOAD_CMD_INIT          = 0x41,    /*一次下载初始化命令, PC->UE*/
    DLOAD_CMD_DATA          = 0x42,    /*下载数据包传输命令, PC->UE*/
    DLOAD_CMD_END           = 0x43,    /*结束一次下载命令, PC->UE*/
    DLOAD_CMD_VERIFY        = 0x45,    /*产品ID查询与返回命令, UE<->PC*/
    DLOAD_CMD_TCPU_VER      = 0x46,    /*单板软件版本查询与返回命令,仅存在于强制加载模式,UE<->PC*/  
    DLOAD_CMD_BCPU_VER      = 0x4B,    /*单板软件版本查询与返回命令,仅存在于bootrom重启模式,UE<->PC*/  
    DLOAD_CMD_CDROM_STATUS  = 0xFB,    /*CDROM状态查询, 仅存在于强制加载模式,UE<->PC*/
    DLOAD_CMD_BUF
}dload_cmd_e;

/*USB交互命令,UE->PC发送的命令*/
typedef enum
{
    DLOAD_RSP_ACK,                   /*正常响应*/
    /*非法响应, UE->PC*/
    DLOAD_RSP_NAK_INVALID_FCS,       /*CRC校验错误*/  
    DLOAD_RSP_NAK_INVALID_DEST,      /*目标内存空间超出范围*/ 
    DLOAD_RSP_NAK_INVALID_LEN,       /*接收到的与预置的长度不一致*/ 
    DLOAD_RSP_NAK_EARLY_END,         /*数据长度过短*/ 
    DLOAD_RSP_NAK_TOO_LARGE,         /*数据长度过长*/ 
    DLOAD_RSP_NAK_INVALID_CMD,       /*无效的命令*/ 
    DLOAD_RSP_NAK_FAILED,            /*操作无法完成*/ 
    DLOAD_RSP_NAK_WRONG_IID,         /*未使用*/ 
    DLOAD_RSP_NAK_BAD_VPP,           /*未使用*/ 
    DLOAD_RSP_NAK_OP_NOT_PERMITTED,  /*现特性不允许该操作*/ 
    DLOAD_RSP_NAK_INVALID_ADDR,      /*此地址的内存无法访问*/ 
    DLOAD_RSP_NAK_VERIFY_FAILED,     /*鉴权失败,没有权限操作*/ 
    DLOAD_RSP_NAK_NO_SEC_CODE,       /*没有安全码*/ 
    DLOAD_RSP_NAK_BAD_SEC_CODE,      /*安全码错误无效 */
    DLOAD_RSP_NAK_FILE_TOO_BIG,      /* Failure: the file to be down is larger than the section in flash */
    DLOAD_RSP_NAK_FLASH_OPER_FAIL    /* Failure: the flash operation required failed*/
}dload_rsp_type_e;

/*升级类型枚举值*/
typedef  enum
{
    DLOAD_MMC_UPDATE = 0,           /*  SD卡升级 */
    DLOAD_ONLINE_UPDATE,           /*  在线升级 */
    DLOAD_USB_UPDATE,               /*  一键升级/多台加载 */
    DLOAD_UPDATE_MAX 
} dload_type_e;

/*状态枚举值*/
typedef enum
{
    IDLEUP                       = 10,
    QUERYING                     = 11,
    NEWVERSIONFOUND              = 12,
    DOWNLOAD_FAILED              = 20,
    DOWNLOAD_PROGRESSING         = 30,
    DOWNLOADPENDING              = 31,
    DOWNLOAD_COMPLETE            = 40,
    READY_TO_UPDATE              = 50,
    UPDATE_PROGRESSING           = 60,
    UPDATE_FAILED_HAVE_DATA      = 70,
    UPDATE_FAILED_NO_DATA        = 80,
    UPDATE_SUCCESSFUL_HAVE_DATA  = 90,
    UPDATE_SUCCESSFUL_NO_DATA    = 100,
    UPDATE_SIGN_VERIFY_FAIL      = 110,      /*在线升级签名验证失败 */
    UPDATE_MESSAGE_TYPE_COUNT
} update_mesg_type_e, update_status_e;

typedef enum
{
    NVNOTRESTORE = 0,            /*正常端口形态*/
    NVRESTORE    = 0X444E5350,   /*NV恢复端口形态魔术字*/
} dload_port_state_e;

typedef enum
{
    DLOAD_BALONG,  /* 巴龙平台 */
    DLOAD_QCOMM,   /* 高通平台 */
}platform_type_t;

typedef enum
{
    DLOAD_STICK,    /* 数据卡形态 */
    DLOAD_HILINK,   /* 闪电卡形态 */
    DLOAD_WINGLE,   /* wingle形态 */
    DLOAD_E5,       /* 通用E5形态 */
    DLOAD_CPE,      /* CPE形态 */
    DLOAD_MOD,      /* 模块形态 */
    DLOAD_UDP,      /* UDP形态 */
}product_type_t;

/* 升级状态类型 */
typedef enum
{
    DLOAD_STATE_NORMAL_IDLE,       /*空闲状态*/
    DLOAD_STATE_NORMAL_RUNNING,   /*正在升级中*/
    DLOAD_STATE_AUTOUG_RUNNING,   /*在线升级中*/
    DLOAD_STATE_NORMAL_LOW_POWER, /*SD卡升级电池低电*/
    DLOAD_STATE_NORMAL_SUCCESS,   /*SD卡升级成功*/
    DLOAD_STATE_NORMAL_FAIL,       /*SD卡升级，在先升级失败时，LCD提示*/
    DLOAD_STATE_FAIL,              /*升级尝试最大次数后显示失败 或 usb升级解析命令或数据失败，E5 在此两种状态下lcd 显示idle*/
    DLOAD_STATE_INVALID,
    /* 最多支持10个状态 */
}dload_state_t;

typedef struct _PRODUCT_INFO
{
    platform_type_t  platfm_id_e; /* 平台标识 */
    product_type_t   prodct_id_e; /* 产品形态 */
    uint32           hw_major_id; /* 产品硬件主ID */
    uint32           hw_minor_id; /* 产品硬件次ID */
    uint8            platfm_name[DLOAD_NAME_MAX_LEN]; /* 平台名称 */
    uint8            prodct_name[DLOAD_NAME_MAX_LEN]; /* 对外产品名称 */
    uint8            upgrade_id[DLOAD_NAME_MAX_LEN];  /* 产品升级ID */
}product_info_t;

/* 平台信息结构体 */
typedef struct _PLATFORM_INFO
{
    product_info_t  product_info_st; /* 平台产品信息 */
    struct _PLATFORM_FEATURE
    {
        boolean  led_feature;           /* LED特性 TRUE:支持，FALSE: 不支持 */
        boolean  lcd_feature;           /* LCD特性 TRUE:支持，FALSE: 不支持 */
        boolean  reserved4;             /* 预留 */
        boolean  reserved3;             /* 预留 */
        boolean  reserved2;             /* 预留 */
        boolean  reserved1;             /* 预留 */
    }platform_feature;
}platform_info_t;

/* 升级特性配置结构体 */
#define  FEATURE_RESERV_NUM   50        /* 升级预留特性个数 */
typedef struct _DLOAD_FEATURE
{
    uint32           hw_major_id;        /* 产品硬件主ID */
    uint32           hw_minor_id;        /* 产品硬件次ID */
    boolean          usb_dload;          /* 升级: USB升级 */
    boolean          scard_dload;        /* 升级: SD升级 */
    boolean          online_dload;       /* 升级: 在线升级 */
    boolean          safety_dload;       /* 升级: 安全升级 */
    boolean          hota_dload;         /* 升级: HOTA升级 */
    boolean          fota_dload;         /* 升级: FOTA升级 */
    boolean          reserved[FEATURE_RESERV_NUM]; /* 预留 */
} dload_feature_t;

/* 共享内存访问枚举定义 */
typedef enum
{
    SET_SMEM_DLOAD_MODE_MAGIC,             /* 设置进入下载模式标志 */
    SET_SMEM_DLOAD_MODE_ZERO,              /* 设置进入下载模式标志清零 */
    GET_SMEM_IS_DLOAD_MODE,                /* 获取是否进入下载模式 */
    SET_SMEM_SCARD_DLOAD_MAGIC,            /* 设置按键进入SD升级标志 */
    SET_SMEM_SCARD_DLOAD_ZERO,             /* 设置按键进入SD升级标志清零 */  
    GET_SMEM_IS_SCARD_DLOAD,               /* 获取是否需要SD升级 */  
    SET_SMEM_SWITCH_PORT_MAGIC,            /* 设置NV自动恢复阶段USB端口形态标志 */
    SET_SMEM_SWITCH_PORT_ZERO,             /* 设置NV自动恢复阶段USB端口形态标志清零 */ 
    GET_SMEM_IS_SWITCH_PORT,               /* 获取是否NV自动恢复阶段USB端口形态已切换 */  
    SET_SMEM_ONL_RESTORE_REBOOT_MAGIC,    /* 设置在线升级NV自动恢复完成后单板需要重启 */
    SET_SMEM_ONL_RESTORE_REBOOT_ZERO,     /* 设置在线升级NV自动恢复完成后单板需要重启清零 */
    GET_SMEM_ONL_IS_RESTORE_REBOOT,       /* 获取在线升级NV自动恢复完成后单板是否需要重启 */
    SET_SMEM_SD_RESTORE_REBOOT_MAGIC,     /* 设置SD升级NV自动恢复完成后单板重启标志 */  
    SET_SMEM_SD_RESTORE_REBOOT_ZERO,      /* 设置SD升级NV自动恢复完成后单板重启标志清零 */  
    GET_SMEM_SD_IS_RESTORE_REBOOT,        /* 获取SD升级NV自动恢复完成后单板是否需要重启 */  
    SET_SMEM_FORCE_MODE_MAGIC,             /* 设置进入逃生模式标志 */
    SET_SMEM_FORCE_MODE_ZERO,              /* 设置进入逃生模式标志清零 */  
    SET_SMEM_FORCE_SUCCESS_MAGIC,          /* 设置逃生模式升级成功标志 */  
    GET_SMEM_IS_FORCE_MODE,                 /* 获取是否进入逃生模式 */  
    SET_SMEM_EXCEP_REBOOT_INTO_ONL_MAGIC, /* 设置异常重启多次进入在线升级标志 */
    SET_SMEM_EXCEP_REBOOT_INTO_ONL_ZERO,  /* 设置异常重启多次进入在线升级标志清零 */  
    GET_SMEM_IS_EXCEP_REBOOT_INTO_ONL,    /* 获取异常重启多次是否进入在线升级 */  
    SET_SMEM_UT_DLOAD_MAGIC,               /* 设置多台加载升级标志 */  
    SET_SMEM_UT_DLOAD_ZERO,                 /* 设置多台加载升级标志清零 */  
    GET_SMEM_IS_UT_DLOAD,                   /* 获取是否是多台加载升级 */ 
    SET_SMEM_MULTIUPG_DLOAD_MAGIC,         /* 设置组播升级标志 */
}smem_visit_t;

/*************************************************显示模块公共信息************************************************/
#define STR_MAX_LEN            (64)
#define PATH_MAX_LEN           (256)

/* 状态动作指示 */
typedef enum
{
    LED_READ,
    LED_WRITE,
    LCD_READ,
    LCD_WRITE,
}oper_mode_t;

/* 路径标识码映射结构 */
typedef struct
{
    uint32  path_index;
    char    path_content[PATH_MAX_LEN];
}path_info_t;

/* 升级 lcd/led 运行状态数据结构 */
typedef struct
{
    uint32        up_state;
    oper_mode_t   oper_mode;
    union
    {
        uint32    path_index;
        int32     num_value;
        char      str_value[STR_MAX_LEN];
    };
    uint32        sleep_len;
}upstate_info_t;

#ifdef  __cplusplus
    }
#endif
#endif /* _DLOAD_COMM_H_ */
