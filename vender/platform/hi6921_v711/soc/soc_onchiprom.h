/*************************************************************************
*   版权所有(C) 1987-2004, 深圳华为技术有限公司.
*
*   文 件 名 :  soc_onchip.h
*
*
*   描    述 :  OnChipRom相关数据区
*
*
*************************************************************************/

#ifndef __SOC_ONCHIP_H__
#define __SOC_ONCHIP_H__

#include "soc_memmap.h"

#define FEATURE_RSA_LOCAL   // 表示使用本地的RSA算法。代替原来的宏SOC_ONCHIPROM_V7R2
/*--------------------------------------------------------------*
 * 宏定义                                                    *
 *--------------------------------------------------------------*/

#define M3_TCM_ADDR                    (0x10000000)	 /* TCM起始地址 */
#define M3_TCM_SIZE                    (64*1024)     /* 64K */

#define SRAM_ADDR                      (0x4FE00000)     /* AXI Memory起始地址 */
#define SRAM_SIZE                      (128*1024)       /* AXI Memory大小 */

/* 共享数据区放在TCM顶端,BootLoader启动后需要拷贝走 */
#define M3_TCM_SHARE_DATA_SIZE sizeof(tOcrShareData)
#define M3_TCM_SHARE_DATA_ADDR (SRAM_ADDR + SRAM_SIZE - M3_TCM_SHARE_DATA_SIZE)

/* OnChipRom运行过标志和自举标志都在tOcrShareData结构体内 */
#define OCR_INITED_FLAG_ADDR    (SRAM_ADDR + SRAM_SIZE - 4)
#define OCR_AUTO_ENUM_FLAG_ADDR (SRAM_ADDR + SRAM_SIZE - 8)
#define OCR_INITED_FLAG_VALUE   (0x23456789)
#define OCR_UNINITED_FLAG_VALUE (0xA55A6789)


#define AUTO_ENUM_FLAG_VALUE    0x82822828

/* 只有OEM CA中才有OEM ID和HWID,根CA中无 */
#define KEY_OEMID_INDEX        sizeof(KEY_STRUCT)
#define KEY_HWID_INDEX         (KEY_OEMID_INDEX+0x04)

#define BL_LEN_INDEX            (144 * 4)    /* bootload.bin文件长度(Byte)保存在bootload.bin文件中的偏移字节数 */
#define ROOT_CA_INDEX           (BL_LEN_INDEX + 4)   /* 根CA在Image中位置 */

#define ROOT_CA_LEN            260      /*CA证书的长度 260 Byte*/
#define OEM_CA_LEN             268      /*CA证书的长度 268 Byte*/
#define IDIO_LEN               128      /*签名的长度 128 Byte*/

#define MAX_N_LENGTH    32

#define SHA256_HASH_SIZE  8       /*HASH签名占的word数*/

#define MD5_HASH_SIZE   4       /* MD5摘要所占的word数 */

#define NAND_ARGS_FULL_AVAIL	0x3a
#define NAND_ARGS_PART_AVAIL	0x45
#define NANDC_USE_ONCHIPROM_PARAM
#define NANDC_OOBLENGTH_8BITECCTYPE
#ifndef __ASSEMBLY__
/*--------------------------------------------------------------*
 * 数据结构                                                     *
 *--------------------------------------------------------------*/

/* 公钥格式 */
typedef struct
{
    unsigned short eLen;    /* E长度以64bits为单位，取值范围为0~15，0表示16 */
    unsigned short nLen;    /* N长度以32bits为单位，取值范围为0~31，0表示32 */
    unsigned int   e[MAX_N_LENGTH];       /* 幂e(公钥) */
    unsigned int   n[MAX_N_LENGTH];       /* 模数 */
}KEY_STRUCT;

/* nand spec info，每个参数的取值都是真实的值，如page_size可取2048、4096等，而非b01\b11等 */
struct nand_spec_real
{
	unsigned long page_size;
	unsigned long page_num_per_block;
	unsigned long addr_cycle;
	unsigned long ecc_type;
	unsigned long oob_size;
	unsigned long chip_size;	/* 以MB为单位 */
};

/* onchiprom里把nand spec info保存到SRAM的一个32bit的buffer中，下面是每个字段的含义 */
struct nand_spec_sram
{
	/* pagesize/ecctype/addrnum/pagenumperblock 这几个参数与寄存器中配置值一致 */
    unsigned int    page_size          : 4;  /* [0~3]   */
    unsigned int    ecc_type           : 4;  /* [4~7]   */
    unsigned int    addr_num           : 4;  /* [8~11]  */
    unsigned int    page_num_per_block : 4;  /* [12~15] */

	/* sparesize/chipsize这两个参数可能获取不到 */
	unsigned int    spare_size		   : 5;  /* [16~20] */
	unsigned int    chip_size		   : 4;	 /* [21~24] */	

	/* flag */
    unsigned int    flag           	   : 7;  /* [25~31] NAND_ARGS_FULL_AVAIL - 传全部参数，NAND_ARGS_FULL_AVAIL - 传4个参数，else - buffer值无效 */
};

/* 签名校验函数指针 */
typedef int (*idioIdentifyPtr)(unsigned int dataAddr, unsigned int dataLen, KEY_STRUCT *pKey, unsigned int* pIdio);

/* 计算Hash值函数指针 */
typedef int (*SHA256HashPtr)(unsigned int dataAddr, unsigned int dataLen, unsigned int* hash);

/* 计算RSA函数指针 */
typedef int (*RSAPtr)(KEY_STRUCT *pKey, unsigned int* pIndata, unsigned int* pOutdata);

/* 获取nand flash规格的函数指针，给先贴后烧用 */
typedef int (*bsp_nand_get_spec_and_save_ptr)(volatile struct nand_spec_sram *spec_sram);

/* 共享数据结构体,放在AXI顶部,可以往下加元素(即添加在前面) */
typedef struct tagOcrShareData
{
	struct nand_spec_sram nandspec;
	bsp_nand_get_spec_and_save_ptr bsp_nand_get_spec_and_save;	/* 共享函数指针 */

    int errNo;

    /* 共享函数指针 */
    SHA256HashPtr   SHA256Hash;
    RSAPtr          RSA;
    idioIdentifyPtr idioIdentify;

    /* 共享数据段 */
    int    bSecEn;
    int    bRootCaInited;
    KEY_STRUCT *pRootKey;
    unsigned int  ulEnumFlag;
    unsigned int  ulOcrInitedFlag;
}tOcrShareData;

/* 共享数据区放在SRAM顶端,BootLoader启动后需要拷贝走 */
#define M3_SRAM_SHARE_DATA_SIZE sizeof(tOcrShareData)
#define M3_SRAM_SHARE_DATA_ADDR (SRAM_ADDR + SRAM_SIZE - M3_SRAM_SHARE_DATA_SIZE)

#endif /* __ASSEMBLY__ */

#endif

