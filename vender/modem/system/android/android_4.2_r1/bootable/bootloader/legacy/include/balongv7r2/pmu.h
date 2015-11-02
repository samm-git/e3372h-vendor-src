/*************************************************************************
*   版权所有(C) 1987-2020, 深圳华为技术有限公司.
*
*   文 件 名 :  pmu.h
*
*   作    者 :  xuwenfang
*
*   描    述 :  pmu.c 的头文件
*
*
*************************************************************************/

#ifndef __PMU_H__
#define __PMU_H__

#include <balongv7r2/types.h>
#include <boot/boot.h>
#include <bsp_pmu.h>

#define  pmic_print_error(fmt, ...)    (cprintf("[pmu]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))
#define  pmic_print_info(fmt, ...) \
do {                               \
       /* pmic_print_error(fmt, ##__VA_ARGS__);*/\
} while (0)

/*函数声明, 放在bsp_pmu.h中*/
#endif