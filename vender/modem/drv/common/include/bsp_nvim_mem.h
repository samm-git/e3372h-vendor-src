/******************************************************************************

    Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       :   bsp_nvim_mem.h
  Description     :   nv base definition & type define
  History         :
     1.y00228784       2013-3-26     Draft Enact
     2.
******************************************************************************/


#ifndef __BSP_NVIM_MEM_H__
#define __BSP_NVIM_MEM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*global ddr distribution*/
/*
   ------------------------512k----------------------------
   --------------------------------------------------------
   | glb  | ctrl |file |ref  |file 1|file 2|..... |file n|
   | info | info |list |list |data  |data  |..... |data  |
   | 1k   |      |     |     |      |      |      |      |
   --------------------------------------------------------
*/



#define NV_GLOBAL_START_ADDR         SHM_MEM_NV_ADDR
#define NV_GLOBAL_END_ADDR           SHM_MEM_NV_ADDR + SHM_MEM_NV_SIZE

/*reg global info*/
#define NV_GLOBAL_INFO_ADDR         NV_GLOBAL_START_ADDR
#define NV_GLOBAL_INFO_SIZE         0x400               /*1 K*/

#define NV_GLOBAL_CTRL_INFO_ADDR    (NV_GLOBAL_INFO_ADDR+NV_GLOBAL_INFO_SIZE)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /*__BSP_NVIM_MEM_H__*/



