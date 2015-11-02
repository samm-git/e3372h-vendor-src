/*
 * Driver for AR8035 PHY
 *
 * Author: jiangdihui <jiangdihui@huawei.com>
 *
 */

#include <linux/phy.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include "mbb_net.h"

#define AR8035_INER_LINK_OK             (1 << 10)
#define AR8035_SPEED1000                (1 << 15)
#define AR8035_SPEED100                 (1 << 14)
#define AR8035_FULLDPLX                 (1 << 13)
#define AR8035_LINKSTAT                 (1 << 10)
#define AR8035_HIB_CTRL                 (1 << 15)

#define AR8035_PHYSR                    0x11
#define AR8035_INER                     0x12
#define AR8035_INSR                     0x13
#define AR8035_DEBUG_ADDR               0x1D
#define AR8035_DEBUG_DATA               0x1E
#define AR8035_DEBUG_HIB                0x0B

#define AR8035_PHY_ID                   0x004dd072   
#define AR8035_PHYID_MASK               0x00ffffff

#define GPIO_PHY_RESET  (GPIO_1_8) 
#define PHY_RESET_MTIME  100
#define PHY_RESET_DELAY  10

MODULE_DESCRIPTION("Driver for AR8035 PHY");
MODULE_AUTHOR("jiangdihui <jiangdihui@huawei.com>");
MODULE_LICENSE("GPL");

static struct phy_device *s_phydev = NULL;


/*****************************************************************************
函数名：   mbb_get_phy_device
功能描述:  提供给其它模块获取phy设备
输入参数： 无
返回值：   phy设备
*****************************************************************************/
struct phy_device *mbb_get_phy_device(void)
{
    return s_phydev;
}

/*****************************************************************************
函数名：   ar8035_phy_hwreset
功能描述:  phy设备reset引脚复位
输入参数： 无
返回值：   无
*****************************************************************************/
int ar8035_phy_hwreset(void* priv)
{
    gpio_request(GPIO_PHY_RESET, "ar8035");
    gpio_direction_output(GPIO_PHY_RESET, 1);
    mdelay(PHY_RESET_MTIME); 
    gpio_direction_output(GPIO_PHY_RESET, 0);   
    mdelay(PHY_RESET_DELAY); 
   
    LAN_DEBUG("AR8035 start phy reset PIN:%d\r\n", GPIO_PHY_RESET);

    return 0;
}
  
/*****************************************************************************
函数名：   ar8035_config_init
功能描述:  phy设备初始化
输入参数： phydev
返回值：   NET_RET_OK(0)为ok
*****************************************************************************/
static int ar8035_config_init(struct phy_device *phydev)
{
    int reg, err;

    if (NULL == phydev)
    {
        return NET_RET_FAIL;
    }

    LAN_DEBUG("AR8035 phy driver configinit\r\n");

    /*关闭hibernate， 否则GMAC dma初始化会失败*/
    err = phy_write(phydev, AR8035_DEBUG_ADDR, AR8035_DEBUG_HIB);
    if (err < 0)
    {
        return err;
    }

    reg = phy_read(phydev, AR8035_DEBUG_DATA);
    reg &= (~AR8035_HIB_CTRL);
    err = phy_write(phydev, AR8035_DEBUG_DATA, reg);
    if (err < 0)
    {
        return err;
    }
 
#if 0
    /*配置phy自动协商*/
    reg = phy_read(phydev, MII_BMCR);
    reg |= (BMCR_ANENABLE | BMCR_ANRESTART);
    err = phy_write(phydev, MII_BMCR, reg);
    if (err < 0)
    {
        return err;
    }
    
    reg = phy_read(phydev, AR8035_INER);
    reg = reg | AR8035_INER_LINK_OK;
    err = phy_write(phydev, AR8035_INER, reg);

    if (err < 0)
    {
        return err;
    }    
#endif

    phydev->autoneg = AUTONEG_ENABLE;

    s_phydev = phydev;

    /*网口模块sysfs节点初始化*/
    hw_net_sysfs_init();

    return NET_RET_OK;
}

/*****************************************************************************
函数名：   ar8035_read_status
功能描述:  phy设备获取状态
输入参数： phydev
返回值：   NET_RET_OK(0)为获取状态OK
*****************************************************************************/
int ar8035_read_status(struct phy_device *phydev)
{
    int reg;

    if (NULL == phydev)
    {
        return NET_RET_FAIL;
    }

    reg = phy_read(phydev, AR8035_PHYSR);    
    if (reg < 0)
    {
        return reg;
    }
    
    /*phy是否link*/
    if (reg & AR8035_LINKSTAT)
    {
        phydev->link = 1;
    }    
    else
    {
        phydev->link = 0;
    }    

    /*phy速率配置*/
    if ((reg & AR8035_SPEED100) && !(reg & AR8035_SPEED1000))
    {
        phydev->speed = SPEED_100;
    }
    else if (!(reg & AR8035_SPEED100) && (reg & AR8035_SPEED1000))
    {
        phydev->speed = SPEED_1000;
    }
    else
    {
        phydev->speed = SPEED_10;
    }
    if (reg & AR8035_FULLDPLX)
    {
        phydev->duplex = DUPLEX_FULL;
    }
    else
    {
        phydev->duplex = DUPLEX_HALF;
    }
    
    phydev->pause = 0;
    phydev->asym_pause = 0;

    //LAN_DEBUG("AR8035 phy read status link:%d, speed:%d, duplex:%d\r\n", phydev->link, phydev->speed, phydev->duplex);

    return NET_RET_OK;
}

#define REG_NUM_MAX   32
void SetPhyReg(unsigned int  regnum, unsigned short val)
{
    if (NULL == s_phydev)
    {
        return;
    }

    if (regnum >= REG_NUM_MAX || val > 0xFFFF)
    {       
        return;
    }

    LAN_DEBUG("set phy register %d value: 0x%x\r\n", regnum, val);

    phy_write(s_phydev, regnum, val);
}

int GetPhyReg(unsigned int regnum)
{
    int value; 
    if (NULL == s_phydev)
    {
        return -1;
    }

    if (regnum >= REG_NUM_MAX)
    {       
        return -1;
    }
    
    value = phy_read(s_phydev, regnum);
   
    LAN_DEBUG("get phy register %d value: 0x%x\r\n", regnum, value);

    return value;
}

EXPORT_SYMBOL(SetPhyReg);
EXPORT_SYMBOL(GetPhyReg);

static struct phy_driver ar8035_driver = {
    .phy_id         = AR8035_PHY_ID,
    .phy_id_mask    = AR8035_PHYID_MASK,      
    .name           = "AR8035 Gigabit Phy",
    .features       = PHY_GBIT_FEATURES,
    .flags          = PHY_HAS_INTERRUPT,
    .config_aneg    = genphy_config_aneg,
    .read_status    = ar8035_read_status,
    .config_init    = ar8035_config_init,
    .driver         = { .owner = THIS_MODULE,},
};

/*****************************************************************************
函数名：   ar8035_init
功能描述: ar8035 phy设备注册
输入参数： 无
返回值：   
*****************************************************************************/
static int __init ar8035_init(void)
{
    int ret;

    LAN_DEBUG("AR8035 phy driver register\r\n");

    ret = phy_driver_register(&ar8035_driver);

    return ret;
}

/*****************************************************************************
函数名：   ar8035_exit
功能描述: ar8035 phy设备退出
输入参数： 无
返回值：   
*****************************************************************************/
static void __exit ar8035_exit(void)
{
    phy_driver_unregister(&ar8035_driver);

    hw_net_sysfs_uninit();
}

module_init(ar8035_init);
module_exit(ar8035_exit);

static struct mdio_device_id __maybe_unused ar8035_tbl[] = {
    { AR8035_PHY_ID, AR8035_PHYID_MASK },
    { }
};

MODULE_DEVICE_TABLE(mdio, ar8035_tbl);

