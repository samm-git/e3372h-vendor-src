/*
This file contains I/O address and related constants for the balongv7r2 interrupts level , modem only.
*/

#ifndef __SOC_INTERRUPTS_MDM_H
#define __SOC_INTERRUPTS_MDM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "soc_interrupts_comm.h"

/**************************************************************************
                  * interrupts level modem only *
***************************************************************************/

#define INT_LVL_IPCM2MDM0                     32
#define INT_LVL_IPCM2MDM1                     33
#define INT_LVL_MDM_COMMRX                    34
#define INT_LVL_MDM_COMMTX                    35

#define INT_LVL_MDM_GPIO0                     112
#define INT_LVL_MDM_GPIO1                     113
#define INT_LVL_MDM_GPIO2                     114
#define INT_LVL_MDM_GPIO3                     115

#define INT_LVL_HIFI_GPIO1                    125
#define INT_LVL_BBE16_GPIO1                   126

#define INT_LVL_SYSCONCTRL2                   132
#define INT_LVL_SYSCONCTRL3                   133

#define INT_LVL_APP_EVENT0                    142


#ifdef __cplusplus
}
#endif

#endif    /* __SOC_INTERRUPTS_MDM_H */

