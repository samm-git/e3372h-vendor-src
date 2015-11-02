/*******************************************************************************
  This contains the functions to handle the platform driver.

  Copyright (C) 2007-2011  STMicroelectronics Ltd

  Copyright (C) Huawei Technologies Co.,Ltd.
  2014-09-23 - adapted for Hisi_Balong in stmmac_pltfr_probe and add
  stmmac_power_dump stmmac_power_get stmmac_power_put stmmac_power_on
  stmmac_power_off stmmac_clk_enable stmmac_pltfr_init stmmac_pltfr_exit
  Fangyuan Liu <foss@huawei.com>.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/
/*lint -save -e305 -e537 -e730 -e732 -e737 -e713 -e718 -e746 -e728 -e830*/

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_net.h>
#include "stmmac.h"
#include "stmmac_debug.h"
#include "soc_interrupts_comm.h"
#include "soc_memmap_comm.h"
#include "hi_syscrg_interface.h"
#include <bsp_clk.h>
#include "bsp_regulator.h"

#ifdef CONFIG_GMAC_TEST
/*gmac dev, add by wangweichao*/
struct stmmac_priv *g_priv_dev;
#endif
u64 stmmac_dma_mask = 0xffffffffUL;


enum gmac_phy_addr
{
	PHY0  = 0,			//The device can support 32 phys, but we use first phy only
	PHY1,
	PHY2,
	PHY3,
	PHY4,
	PHY5,
	PHY6,
	PHY7,
	PHY17 = 0x11,       //phy address depends on hardware
	PHY31 = 31
};

#ifdef CONFIG_OF
static int __devinit stmmac_probe_config_dt(struct platform_device *pdev,
					    struct plat_stmmacenet_data *plat,
					    const char **mac)
{
	struct device_node *np = pdev->dev.of_node;

	if (!np)
		return -ENODEV;

	*mac = of_get_mac_address(np);
	plat->interface = of_get_phy_mode(np);
	plat->mdio_bus_data = devm_kzalloc(&pdev->dev,
					   sizeof(struct stmmac_mdio_bus_data),
					   GFP_KERNEL);

	/*
	 * Currently only the properties needed on SPEAr600
	 * are provided. All other properties should be added
	 * once needed on other platforms.
	 */
	if (of_device_is_compatible(np, "st,spear600-gmac")) {
		plat->pbl = 8;
		plat->has_gmac = 1;
		plat->pmt = 1;
	}

	return 0;
}
#else
static int __devinit stmmac_probe_config_dt(struct platform_device *pdev,
					    struct plat_stmmacenet_data *plat,
					    const char **mac)
{
	return -ENOSYS;
}
#endif /* CONFIG_OF */

#define STMMAC_PWR_NAME     "gmac-vcc"
struct stmmac_power_stat{
    unsigned int get_all;
    unsigned int get_fail;
    unsigned int put_all;
    unsigned int put_ok;
    unsigned int on_all;
    unsigned int on_null;
    unsigned int on_fail;
    unsigned int off_all;
    unsigned int off_null;
};

static struct regulator *stmmac_vcc = NULL;
static struct stmmac_power_stat g_stmmac_power_stat = {0};

void stmmac_power_dump(void)
{
    printk("stmmac power dump   :\n");
    printk("get_all             :%d\n",g_stmmac_power_stat.get_all);
    printk("get_fail            :%d\n",g_stmmac_power_stat.get_fail);
    printk("put_all             :%d\n",g_stmmac_power_stat.put_all);
    printk("put_ok              :%d\n",g_stmmac_power_stat.put_ok);
    printk("on_all              :%d\n",g_stmmac_power_stat.on_all);
    printk("on_null             :%d\n",g_stmmac_power_stat.on_null);   
    printk("on_fail             :%d\n",g_stmmac_power_stat.on_fail);
    printk("off_all             :%d\n",g_stmmac_power_stat.off_all);
    printk("off_null            :%d\n",g_stmmac_power_stat.off_null);   
}
EXPORT_SYMBOL(stmmac_power_dump);

static void stmmac_power_get(void)
{
    g_stmmac_power_stat.get_all++;
    
    /* get the usb supply to prepare to power on */
    stmmac_vcc = regulator_get(NULL, STMMAC_PWR_NAME);
    if (IS_ERR(stmmac_vcc)) {
        g_stmmac_power_stat.get_fail++;
        printk("regulator_get %s error:%d!/n",
            STMMAC_PWR_NAME,(int)PTR_ERR(stmmac_vcc));
    }
}

static void stmmac_power_put(void)
{
    g_stmmac_power_stat.put_all++;

    if(stmmac_vcc){
        g_stmmac_power_stat.put_ok++;
        regulator_put(stmmac_vcc); 
        stmmac_vcc = NULL;
    }
}

static void stmmac_power_on(void)
{
    int ret;

    g_stmmac_power_stat.on_all++;

    if (!stmmac_vcc) {
        g_stmmac_power_stat.on_null++;
        printk("stmmac:null stmmac_vcc!\n");
        return;
    }

    ret = regulator_enable(stmmac_vcc);
    if(ret) {
        g_stmmac_power_stat.on_fail++;
        printk("stmmac:failed to regulator_enable %s(err code:%d)\n",
            STMMAC_PWR_NAME, ret);
    }

    return;
}

static void stmmac_power_off(void)
{
    g_stmmac_power_stat.off_all++;

    if (!stmmac_vcc) {
        g_stmmac_power_stat.off_null++;
        printk("stmmac:null stmmac_vcc!\n");
        return;
    }

    regulator_disable(stmmac_vcc);
}

static int stmmac_clk_enable(void)
{
    struct clk *aclk = NULL;
    struct clk *swclk = NULL;
    struct clk *gtx = NULL;
    struct clk *tx = NULL;
    struct clk *rx = NULL;
    int ret = 0;

    aclk =clk_get(NULL,"gmac_aclk");    
    if(IS_ERR(aclk)){
            printk("clk:gmac_aclk is NULL, can't find it,please check!!\n");
            return 1;
    }
    ret = clk_enable(aclk);
    if(!ret){
            printk("clk:gmac_aclk enable OK!!\n");
     } 
    
    swclk = clk_get(NULL,"gmac_switch_clk");
    if(IS_ERR(swclk)){
            printk("clk:gmac_swclk is NULL, can't find it,please check!!\n");
            return 1;
    }
    ret = clk_enable(swclk);
    if(!ret){
            printk("clk:gmac_swclk enable OK!!\n");
     } 
    
    gtx = clk_get(NULL,"gmac_gtx_clk");
    if(IS_ERR(gtx)){
            printk("clk:gmac_gtx is NULL, can't find it,please check!!\n");
            return 1;
    }
    ret = clk_enable(gtx);
    if(!ret){
            printk("clk:gmac_gtx enable OK!!\n");
     } 

    
    tx = clk_get(NULL,"gmac_tx_clk");
    if(IS_ERR(tx)){
            printk("clk:gmac_tx is NULL, can't find it,please check!!\n");
            return 1;
    }
    ret = clk_enable(tx);
    if(!ret){
            printk("clk:gmac_tx enable OK!!\n");
     } 
    
    rx = clk_get(NULL,"gmac_rx_clk");
    if(IS_ERR(rx)){
            printk("clk:gmac_rx is NULL, can't find it,please check!!\n");
            return 1;
    }
    ret = clk_enable(rx);
    if(!ret){
            printk("clk:gmac_rx enable OK!!\n");
    } 

    return ret;
}

/**
 * stmmac_pltfr_probe
 * @pdev: platform device pointer
 * Description: platform_device probe function. It allocates
 * the necessary resources and invokes the main to init
 * the net device, register the mdio bus etc.
 */
static int stmmac_pltfr_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct resource *res;
	void __iomem *addr = NULL;
	struct stmmac_priv *priv = NULL;
	struct plat_stmmacenet_data *plat_dat = NULL;
	const char *mac = NULL;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	if (!request_mem_region(res->start, resource_size(res), pdev->name)) {
        GMAC_ERR(("%s: ERROR: memory allocation failed"
		       "cannot get the I/O addr 0x%x\n",
		       __func__, (unsigned int)res->start));
		return -EBUSY;
	}

	addr = ioremap(res->start, resource_size(res));
	if (!addr) {
		GMAC_ERR(("%s: ERROR: memory mapping failed", __func__));
		ret = -ENOMEM;
		goto out_release_region;
	}

	if (pdev->dev.of_node) {
		plat_dat = devm_kzalloc(&pdev->dev,
					sizeof(struct plat_stmmacenet_data),
					GFP_KERNEL);
		if (!plat_dat) {
			GMAC_ERR(("%s: ERROR: no memory", __func__));
			ret = -ENOMEM;
			goto out_unmap;
		}

		ret = stmmac_probe_config_dt(pdev, plat_dat, &mac);
		if (ret) {
			GMAC_ERR(("%s: main dt probe failed", __func__));
			goto out_unmap;
		}
	} else {
		plat_dat = pdev->dev.platform_data;
	}

	/* Custom initialisation (if needed)*/
	if (plat_dat->init) {
		ret = plat_dat->init(pdev);
		if (unlikely(ret))
			goto out_unmap;
	}
    stmmac_clk_enable();
	priv = stmmac_dvr_probe(&(pdev->dev), plat_dat, addr);
	if (!priv) {
		GMAC_ERR(("%s: main driver probe failed", __func__));
		goto out_unmap;
	}

/*dev_addr is equal to mac only when support device tree */
	    /* Get MAC address if available (DT) */
	    if (mac)
		    memcpy(priv->dev->dev_addr, mac, ETH_ALEN);

	/* Get the MAC information */
	priv->dev->irq = platform_get_irq_byname(pdev, "macirq");
	if (priv->dev->irq == -ENXIO) {
		GMAC_ERR(("%s: ERROR: MAC IRQ configuration "
		       "information not found\n", __func__));
		ret = -ENXIO;
		goto out_unmap;
	}

	/*
	 * On some platforms e.g. SPEAr the wake up irq differs from the mac irq
	 * The external wake up irq can be passed through the platform code
	 * named as "eth_wake_irq"
	 *
	 * In case the wake up interrupt is not passed from the platform
	 * so the driver will continue to use the mac irq (ndev->irq)
	 */
	priv->wol_irq = platform_get_irq_byname(pdev, "eth_wake_irq");
	if (priv->wol_irq == -ENXIO)
		priv->wol_irq = priv->dev->irq;

	platform_set_drvdata(pdev, priv->dev);

    #ifdef CONFIG_GMAC_TEST
    /*gmac priv dev for test loopback add by wangweichao*/
    g_priv_dev = priv;
    #endif
	GMAC_DBG(("STMMAC platform driver registration completed"));

	return 0;

out_unmap:
	iounmap(addr);
	platform_set_drvdata(pdev, NULL);

out_release_region:
	release_mem_region(res->start, resource_size(res));

	return ret;
}

/**
 * stmmac_pltfr_remove
 * @pdev: platform device pointer
 * Description: this function calls the main to free the net resources
 * and calls the platforms hook and release the resources (e.g. mem).
 */
static int stmmac_pltfr_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct stmmac_priv *priv = netdev_priv(ndev);
	struct resource *res;
	int ret = stmmac_dvr_remove(ndev);

	if (priv->plat->exit)
		priv->plat->exit(pdev);

	if (priv->plat->exit)
		priv->plat->exit(pdev);

	platform_set_drvdata(pdev, NULL);

	iounmap((void *)priv->ioaddr);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

	return ret;
}

#ifdef CONFIG_PM
static int stmmac_pltfr_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_suspend(ndev);
}

static int stmmac_pltfr_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_resume(ndev);
}

int stmmac_pltfr_freeze(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_freeze(ndev);
}

int stmmac_pltfr_restore(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_restore(ndev);
}

static const struct dev_pm_ops stmmac_pltfr_pm_ops = {
	.suspend = stmmac_pltfr_suspend,
	.resume = stmmac_pltfr_resume,
	.freeze = stmmac_pltfr_freeze,
	.thaw = stmmac_pltfr_restore,
	.restore = stmmac_pltfr_restore,
};
#else
static const struct dev_pm_ops stmmac_pltfr_pm_ops = {};
#endif /* CONFIG_PM */

static const struct of_device_id stmmac_dt_ids[] = {
	{ .compatible = "st,spear600-gmac", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, stmmac_dt_ids);

static struct platform_driver stmmac_driver = {
	.probe = stmmac_pltfr_probe,
	.remove = stmmac_pltfr_remove,
	.driver = {
		   .name = STMMAC_RESOURCE_NAME,
		   .owner = THIS_MODULE,
		   .pm = &stmmac_pltfr_pm_ops,
		   .of_match_table = of_match_ptr(stmmac_dt_ids),
		   },
};

static struct resource stmmac_resources[] = {
     [0] = {
          .start = HI_GMAC_REGBASE_ADDR,
          .end   = HI_GMAC_REGBASE_ADDR + HI_GMAC_REG_SIZE - 1,
          .flags = IORESOURCE_MEM,
     },
     [1] = {
          .name  = "macirq",
          .start = INT_LVL_GMAC_SBD,
          .end   = INT_LVL_GMAC_SBD,
          .flags = IORESOURCE_IRQ,
     },
     [2] = {
          .name  = "eth_wake_irq",
          .start = INT_LVL_GMAC_PMT,
          .end   = INT_LVL_GMAC_PMT,
          .flags = IORESOURCE_IRQ,
     }
};

#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
extern int ar8035_phy_hwreset(void *priv);
#endif

static struct stmmac_mdio_bus_data stmmac_mdio_data = {
    .bus_id = 0,
#if (FEATURE_ON == MBB_FEATURE_ETH)
    .phy_mask = 0xFFFFFFFE,
#else
    .phy_mask = 0xFFFFFFFD,
#endif
#if (FEATURE_ON == MBB_FEATURE_ETH_PHY)
    .phy_reset = ar8035_phy_hwreset,
#endif
};
static struct plat_stmmacenet_data stmmac_ethernet_platform_data = {
     .bus_id = 0,
#if (FEATURE_ON == MBB_FEATURE_ETH)
     .phy_addr = 0,
#else
     .phy_addr = -1,
#endif
     .pbl = 16,
     .has_gmac = 1,
     .enh_desc = 1,
     .force_sf_dma_mode = 1,
     .mdio_bus_data = &stmmac_mdio_data,
     .interface = PHY_INTERFACE_MODE_RGMII,
};

static struct platform_device stmmac_device = {
     .name = STMMAC_RESOURCE_NAME,
     .id   = 0,
     .dev  = {
          .platform_data     = &stmmac_ethernet_platform_data,
          .dma_mask          = &stmmac_dma_mask,
          .coherent_dma_mask = 0xffffffffUL,
          //.release           = stmmac_platform_device_release,
     },
     .num_resources = ARRAY_SIZE(stmmac_resources),
     .resource      = stmmac_resources,
};

//module_platform_driver(stmmac_driver);

void stmmac_sysconfig(void)
{
//This reset only needed on 721 due to hardware reason, disable it on 711
//     hi_syscrg_dsp0_reset();
//     mdelay(20);
     hi_syscrg_gmac_reset();
     return;
}

static int __init stmmac_pltfr_init(void)
{
    int ret;

    stmmac_sysconfig();
    stmmac_power_get(); 
    stmmac_power_on();

    ret = platform_device_register(&stmmac_device);
    if (ret) {
        GMAC_ERR(("%s, stmmac device register fail:%d\n", __FUNCTION__, ret));
        return ret;
    }

    ret = platform_driver_register(&stmmac_driver);
    if (ret) {
        GMAC_ERR(("%s, stmmac driver register fail:%d\n", __FUNCTION__, ret));
        goto pltfr_dev_exit;
    }

    return ret;

pltfr_dev_exit:
    platform_device_unregister(&stmmac_device);
    //stmmac_power_off();
    //stmmac_power_put();
    return ret;
}

static void __exit stmmac_pltfr_exit(void)
{
     platform_driver_unregister(&stmmac_driver);

     platform_device_unregister(&stmmac_device);

     stmmac_power_off();
     stmmac_power_put();
}

module_init(stmmac_pltfr_init);
module_exit(stmmac_pltfr_exit);

MODULE_DESCRIPTION("STMMAC 10/100/1000 Ethernet PLATFORM driver");
MODULE_AUTHOR("Giuseppe Cavallaro <peppe.cavallaro@st.com>");
MODULE_LICENSE("GPL");
/*lint -restore*/
