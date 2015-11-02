/*************************************************************************
*   版权所有(C) 2008-2013, 深圳华为技术有限公司.
*
*   文 件 名 :  bsp_reg_def.h
*
*   作    者 :  l00225826
*
*   描    述 :  m3启动相关内容
*
*************************************************************************/
#ifndef __BSP_REG_DEF_H__
#define __BSP_REG_DEF_H__

/*lint -e760 -e547*/
#define READ_REGISTER_ULONG(addr) (*(volatile UINT32 *)(addr))
#define WRITE_REGISTER_ULONG(addr,val) (*(volatile UINT32 *)(addr) = (val))
#define INREG32(x)          READ_REGISTER_ULONG(x)
#define OUTREG32(x, y)      WRITE_REGISTER_ULONG((x), (UINT32)(y))
#define SETREG32(x, y)      OUTREG32((x), INREG32(x)|(y))
#define CLRREG32(x, y)      OUTREG32((x), INREG32(x)&~(y))
#define SETBITVALUE32(addr, mask, value)  OUTREG32((addr), (INREG32(addr)&(~(mask))) | ((value)&(mask)))
/*lint +e760 +e547*/

#endif
