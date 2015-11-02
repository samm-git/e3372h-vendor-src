/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  DrvInterface.h
*
*   ��    �� :  yangzhi
*
*   ��    �� :  ���ļ�����Ϊ"DrvInterface.h", ����V7R1������Э��ջ֮���API�ӿ�ͳ��
*
*   �޸ļ�¼ :  2011��1��18��  v1.00  yangzhi����
*************************************************************************/

#ifndef __DRV_V9R1_BBIT_H__
#define __DRV_V9R1_BBIT_H__

#include "drv_comm.h"


/************************V9R1BBIT START***********************/
typedef enum
{
    DBLED_DRV1 = 0,        /* DRV���Ե�1 */
    DBLED_DRV2,            /* DRV���Ե�2 */
    DBLED_DSP1 = 2,        /* DSP���Ե�1 */
    DBLED_DSP2,            /* DSP���Ե�2 */
    DBLED_RESERVED1 = 4,   /* �������Ե�1 */
    DBLED_RESERVED2,       /* �������Ե�2 */
    DBLED_MAX
}DBLED_ID_E;

/*****************************************************************************
 �� �� ��  : DRV_DBLED_ON
 ��������  : ���ԵƵƵ�����������
 �������  : ledId:���ID,ȡֵ0~5
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   : ��ҫ�� 00179452
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int DRV_DBLED_ON(unsigned int ledId);

/*****************************************************************************
 �� �� ��  : DRV_DBLED_OFF
 ��������  : ���Ե�Ϩ��������
 �������  : ledId:���ID,ȡֵ0~5
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��25��
    ��    ��   : ��ҫ�� 00179452
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int DRV_DBLED_OFF(unsigned int ledId);

/*V9R1SFT SC����ַ��BBITƽ̨��SC*/
#define SC_BASE    SOC_SCTRL_BASE_ADDR

/*****************************************************************************
 �� �� ��  : DRV_COPY_NVUSE_TO_NVBACKUP
 ��������  : ��NV��ʹ����������������
 �������  : ��
 �������  : ��
 �� �� ֵ  : int��0�ɹ�������ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��21��
    ��    ��   : ��� y00186965
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int DRV_COPY_NVUSE_TO_NVBACKUP ( void );


/*****************************************************************************
 �� �� ��  : DRV_COPY_NVUPGRADE_TO_NVUSE
 ��������  : ��NV��������������ʹ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : int��0�ɹ�������ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��21��
    ��    ��   : ��� y00186965
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int DRV_COPY_NVUPGRADE_TO_NVUSE( void );
/*************************V9R1BBIT END************************/

#endif
