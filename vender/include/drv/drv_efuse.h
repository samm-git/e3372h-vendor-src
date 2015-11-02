/*************************************************************************
*   版权所有(C) 1987-2011, 深圳华为技术有限公司.
*
*   文 件 名 :  DrvInterface.h
*
*   作    者 :  yangzhi
*
*   描    述 :  本文件命名为"DrvInterface.h", 给出V7R1底软和协议栈之间的API接口统计
*
*   修改记录 :  2011年1月18日  v1.00  yangzhi创建
*************************************************************************/

#ifndef __DRV_EFUSE_H__
#define __DRV_EFUSE_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "drv_comm.h"


#define     BUF_ERROR          (-55)
#define     LEN_ERROR          (-56)
#define     READ_EFUSE_ERROR   (-57)



/*************************************************
 函 数 名   : efuseWriteHUK
 功能描述   : HUK写efuse接口
 输入参数   : pBuf:烧写内容;len:烧写长度(字节)
 输出参数   :
 返 回 值      : OK/ERROR
 调用函数   :
 被调函数   :

 修改历史   :
    日    期       : 2012年3月27日
   修改内容 : 新生成函数

*************************************************/
extern int efuseWriteHUK(char *pBuf,unsigned int len);

#define EFUSE_WRITE_HUK(pBuf,len) efuseWriteHUK(pBuf,len)

/*************************************************
 函 数 名   : CheckHukIsValid
 功能描述   : 判断HUK是否有效，若为全0则无效，非全0则有效
 输入参数   : 无
 输出参数   :
 返 回 值   : BSP_FALSE:HUK无效；BSP_TRUE:HUK有效
 调用函数   :
 被调函数   :

 修改历史   :
   日    期 : 2012年3月27日
   修改内容 : 新生成函数

*************************************************/

extern int CheckHukIsValid(void);
#define DRV_CHECK_HUK_IS_VALID() CheckHukIsValid()

/*************************************************
 函 数 名   : DRV_GET_DIEID
 功能描述   : DRV_GET_DIEID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)
 调用函数   :
 被调函数   :

 修改历史   :
   日    期 : 2013年8月12日
   修改内容 : 新生成函数

*************************************************/
extern int DRV_GET_DIEID(unsigned char* buf,int length);

/*************************************************
 函 数 名   : DRV_GET_CHIPID
 功能描述   : DRV_GET_CHIPID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)
 调用函数   :
 被调函数   :

 修改历史   :
   日    期 : 2013年8月12日
   修改内容 : 新生成函数

*************************************************/
extern int DRV_GET_CHIPID(unsigned char* buf,int length);

#ifdef __cplusplus
}
#endif

#endif

