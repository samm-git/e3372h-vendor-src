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
#ifndef _DLOAD_COMM_H_
#define _DLOAD_COMM_H_
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
#define DLOAD_OK          0

#define DLOAD_NAME_MAX_LEN 256

/****************************************************************************
 3 枚举类型，结构体，宏定义
 结构体类型加 :_s
 枚举类型加   :_e
 联合体类型加 :_u
****************************************************************************/
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
    DLOAD_STATE_NORMAL_RUNNING,    /*正在升级中*/
    DLOAD_STATE_AUTOUG_RUNNING,    /*在线升级中*/
    DLOAD_STATE_NORMAL_LOW_POWER,  /*SD卡升级电池低电*/
    DLOAD_STATE_NORMAL_SUCCESS,    /*SD卡升级成功*/
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
    SET_SMEM_UT_DLOAD_MAGIC,                /* 设置多台加载升级标志 */  
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
