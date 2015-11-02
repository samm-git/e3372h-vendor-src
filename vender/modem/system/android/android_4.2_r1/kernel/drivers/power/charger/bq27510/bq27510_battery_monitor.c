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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/power_supply.h>
#include <linux/wakelock.h>
#include <linux/notifier.h>
#include <bq27510_battery.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/module.h>
#include <hisi_coul_drv.h>
#include <bq27510_battery_monitor.h>
#include "../chg_config.h"


#define WINDOW_LEN                   (10)
#define LOG_ARCH_DELAY_TIME          (60000)
static struct wake_lock low_power_lock;
static int is_low_power_locked = 0;
static unsigned int    capacity_filter[WINDOW_LEN];
static unsigned int    capacity_sum;
static unsigned int    suspend_capacity;

struct bq_bci_device_info {
    int    bat_voltage;
    int    bat_temperature;
    int    bat_exist;
    int    charge_status;
    u8     chargedone_stat;
    u16    monitoring_interval;
    unsigned int    capacity;
    unsigned int    capacity_filter_count;
    unsigned int    prev_capacity;
    unsigned int    charge_full_count;
    struct device   *dev;
    struct notifier_block    nb;
    struct delayed_work bq_bci_monitor_work;
};

struct bq_bci_device_info *g_bq_bci_dev = NULL;

BLOCKING_NOTIFIER_HEAD(notifier_list);

#define CHG_ODD_CAPACITY                (2)
#define CHG_CANT_FULL_THRESHOLD         (95) /*bad gasgauge ic can't report capacity correctly
                                              *when charging schedule near full-state.   */

#define BATTERY_PERCENT_SHUTDOWN        (2)

#define REACH_FULL_RESAMPLE_THRESHOLD   (90)
#define REACH_EMPTY_RESAMPLE_THRESHOLD  (10)
#define NORMAL_SAMPLE_INTERVAL          (10)
#define REACH_FULL_SAMPLE_INTERVAL      (30)
#define REACH_EMPTY_SAMPLE_INTERVAL     (5)
#define CHARGE_FULL_TIME                (40*60)/REACH_FULL_SAMPLE_INTERVAL   /*40min = 2400s/INTERVAL*/
#define BATT_OVERVOLTAGE_THRES          (4550)
#define BATT_LOWVOLTAGE_THRES           (3100)
#define ARCH_NAME_MAX                   (256)





static int calc_capacity_from_voltage(void)
{

    return 100;
}


/*only charge-work can not reach full(95%).Change capacity to full after 40min.*/
static int bq_force_full_timer(int curr_capacity, struct bq_bci_device_info *di)
{
    if(curr_capacity > CHG_CANT_FULL_THRESHOLD) {
        di->charge_full_count++;
        if(di->charge_full_count >= CHARGE_FULL_TIME){
            dev_info(di->dev,"FORCE_CHARGE_FULL = %d\n",curr_capacity);
            di->charge_full_count = CHARGE_FULL_TIME;
            curr_capacity = 100;
        }
    } else {
        di->charge_full_count = 0;
    }

    return curr_capacity;
}

static int bq_capacity_pulling_filter(int curr_capacity, struct bq_bci_device_info *di)
{
    int index = 0;
    di->bat_exist = is_hisi_battery_exist();

    if(!di->bat_exist){
        curr_capacity = calc_capacity_from_voltage();
        return curr_capacity;
    }
    index = di->capacity_filter_count%WINDOW_LEN;

    capacity_sum -= capacity_filter[index];
    capacity_filter[index] = curr_capacity;
    capacity_sum += capacity_filter[index];

    if (++di->capacity_filter_count >= WINDOW_LEN) {
        di->capacity_filter_count = 0;
    }

    /*rounding-off 0.5 method*/
    curr_capacity = (capacity_sum*10)/WINDOW_LEN;
    curr_capacity = (curr_capacity+5)/10;

    return curr_capacity;
}

static int capacity_changed(struct bq_bci_device_info *di)
{
    int curr_capacity = 0;
    int low_bat_flag = is_hisi_battery_reach_threshold();

    di->bat_exist = is_hisi_battery_exist();

    /* if battery is not present we assume it is on battery simulator
    *  if we are in factory mode, BAT FW is not updated yet, we use volt2Capacity
    */
    if (!di->bat_exist){
        curr_capacity = calc_capacity_from_voltage();
    } else {
        curr_capacity = hisi_battery_capacity();
    }

    if((low_bat_flag & BQ27510_FLAG_LOCK) != BQ27510_FLAG_LOCK && is_low_power_locked){
        wake_unlock(&low_power_lock);
        is_low_power_locked = 0;
    }

    /* Debouncing of power on init. */
    if (di->capacity == -1) {
        di->capacity = curr_capacity;
        di->prev_capacity = curr_capacity;
        return 1;
    }

    /*Only availability if the capacity changed*/
    if (curr_capacity != di->prev_capacity) {
        if (abs(di->prev_capacity -curr_capacity) >= CHG_ODD_CAPACITY){
            dev_info(di->dev,"prev_capacity = %d \n"
            "curr_capacity = %d \n" "curr_voltage = %d \n",
            di->prev_capacity, curr_capacity,hisi_battery_voltage());
        }
    }

    if (curr_capacity < 2 )
    {
        int battery_volt;

        battery_volt = hisi_battery_voltage();
        if (battery_volt < BAT_VOL_3500)
        {
            di->capacity = curr_capacity;
            return 1;
        }
        dev_err(di->dev, "error capacity reported, capacity = %d\n", curr_capacity);
        return 0;
    }

    switch(di->charge_status) {
    case POWER_SUPPLY_STATUS_CHARGING:
        curr_capacity = bq_force_full_timer(curr_capacity,di);
        break;

    case POWER_SUPPLY_STATUS_FULL:
        if(hisi_battery_current_avg() >= 0){
                curr_capacity = 100;
                dev_info(di->dev,"Force soc=100\n");

        }
        else
        {
            /*满电状态，如果soc在复充门限以上，则显示100%*/
            if(curr_capacity > CHG_CANT_FULL_THRESHOLD)
            {
                curr_capacity = 100;
            }
        }
        di->charge_full_count = 0;
        break;

    case POWER_SUPPLY_STATUS_DISCHARGING:
    case POWER_SUPPLY_STATUS_NOT_CHARGING:
        /*capacity-count will always decrease when discharging || notcharging*/
        if(di->prev_capacity < curr_capacity)
            return 0;
        di->charge_full_count = 0;
        break;

    default:
        dev_err(di->dev, "%s defualt run.\n", __func__);
        break;
    }

    /*change monitoring interval from 10s to 30s when capacity greater than 90%*/
    if(curr_capacity > REACH_FULL_RESAMPLE_THRESHOLD) {
        di->monitoring_interval = REACH_FULL_SAMPLE_INTERVAL;
    } else if(curr_capacity < REACH_EMPTY_RESAMPLE_THRESHOLD) {
        di->monitoring_interval = REACH_EMPTY_SAMPLE_INTERVAL;
    } else {
        di->monitoring_interval = NORMAL_SAMPLE_INTERVAL;
    }
    /*filter*/
    curr_capacity = bq_capacity_pulling_filter(curr_capacity,di);

    if(di->prev_capacity == curr_capacity)
        return 0;

    dev_info(di->dev,"Capacity Updated = %d, charge_full_count = %d, charge_status = %d\n",
                         curr_capacity, di->charge_full_count, di->charge_status);


    /*电池达到低电关机门限，迅速将soc平滑至关机门限*/
    if(chg_is_batt_in_state_of_emergency() && (curr_capacity > BATTERY_PERCENT_SHUTDOWN))//0%低电关机
    {
        curr_capacity = curr_capacity - 1;
        dev_err(di->dev, "emergency!!adjust Battery Capacity to %d Percents\n", curr_capacity);
    }
    /*电压未达到关机门限时，soc低于关机门限2%,则显示3%*/
    if ((curr_capacity <= BATTERY_PERCENT_SHUTDOWN) && (!chg_is_batt_in_state_of_emergency())) {
        curr_capacity = BATTERY_PERCENT_SHUTDOWN + 1;
        dev_err(di->dev, "no emergency!adjust Battery Capacity to %d Percents\n", curr_capacity);
    }
    /*未停充，暂显示99%*/
    if (di->charge_status == POWER_SUPPLY_STATUS_CHARGING && curr_capacity > 99 
        && di->charge_full_count < CHARGE_FULL_TIME){
        curr_capacity = 99;
    }    

    di->capacity = curr_capacity;
    di->prev_capacity = curr_capacity;
    return 1;
}

/*充电事件回调函数*/
int bq_charger_event_rcv (unsigned long evt)
{

    struct bq_bci_device_info *di = g_bq_bci_dev;

    if (!is_hisi_battery_exist()){
        return 0;
    }

    switch (evt) {
        
    case VCHRG_STOP_CHARGING_EVENT:
        di->charge_status = POWER_SUPPLY_STATUS_DISCHARGING;
        di->charge_full_count = 0;
        break;

    case VCHRG_START_CHARGING_EVENT:
        dev_err(di->dev, "%s charger event=%d\n", __func__,VCHRG_START_CHARGING_EVENT);
        di->charge_status = POWER_SUPPLY_STATUS_CHARGING;
        break;

    case VCHRG_NOT_CHARGING_EVENT:
        di->charge_status = POWER_SUPPLY_STATUS_NOT_CHARGING;
        break;
    case VCHRG_CHARGE_DONE_EVENT:
        dev_err(di->dev, "%s charger event=%d\n", __func__,VCHRG_CHARGE_DONE_EVENT);
        di->charge_status = POWER_SUPPLY_STATUS_FULL;
        break;        
    default:
        dev_err(di->dev, "%s defualt run.\n", __func__);
        break;
    }
    return 0;
}

/*低电中断处理函数*/
static int bq_battery_event_handler(struct notifier_block *nb, unsigned long event,
            void *_data)
{
    struct bq_bci_device_info *di = g_bq_bci_dev;
    
    switch (event) {
    case BATTERY_LOW_WARNING:
        dev_err(di->dev, "%s battery event=%d\n", __func__,BATTERY_LOW_WARNING);
        
        break;
    case BATTERY_LOW_SHUTDOWN:
        dev_err(di->dev, "%s battery event=%d\n", __func__,BATTERY_LOW_SHUTDOWN);
        wake_lock(&low_power_lock);
        is_low_power_locked = 1;        
        break;       
    default:
        dev_err(di->dev, "%s defualt run.\n", __func__);
        break;
    }        
    return 0;
}


static void bq_bci_battery_work(struct work_struct *work)
{
    struct bq_bci_device_info *di = container_of(work,
               struct bq_bci_device_info, bq_bci_monitor_work.work);


    schedule_delayed_work(&di->bq_bci_monitor_work,
                 msecs_to_jiffies(1000 * di->monitoring_interval));

    if (capacity_changed(di)){
        /*充电状态机保证soc能够及时上报*/
        dev_err(di->dev, "batt soc changed, new soc=%d \n", di->capacity);
        chg_set_sys_batt_capacity(di->capacity);
    }
}


int bq_bci_show_capacity(void)
{
    struct bq_bci_device_info *di = g_bq_bci_dev;

    if (di)
        return di->capacity;
    else
        return 0;
}
int bq_register_notifier(struct notifier_block *nb,
                         unsigned int events)
{
    return blocking_notifier_chain_register(&notifier_list, nb);
}
EXPORT_SYMBOL_GPL(bq_register_notifier);

int bq_unregister_notifier(struct notifier_block *nb,
                           unsigned int events)
{
    return blocking_notifier_chain_unregister(&notifier_list, nb);
}
EXPORT_SYMBOL_GPL(bq_unregister_notifier);

static char *bq_bci_supplied_to[] = {
    "bq_bci_battery",
};

static int bq_bci_battery_probe(struct platform_device *pdev)
{
    struct bq_bci_device_info *di;
    struct battery_charge_param_s param;
    int low_bat_flag = 0;
    int ret = 0;
    unsigned int i = 0;

    di = kzalloc(sizeof(*di), GFP_KERNEL);
    if (!di)
        return -ENOMEM;
    g_bq_bci_dev = di;



    di->monitoring_interval = NORMAL_SAMPLE_INTERVAL;
    di->dev = &pdev->dev;


    di->charge_status = POWER_SUPPLY_STATUS_DISCHARGING;

    di->capacity = -1;
    di->capacity_filter_count = 0;
    di->charge_full_count = 0;

    for(i=0;i<WINDOW_LEN;i++) {
            capacity_filter[i] = hisi_battery_capacity();
            capacity_sum += capacity_filter[i];
    }


    platform_set_drvdata(pdev, di);

    wake_lock_init(&low_power_lock, WAKE_LOCK_SUSPEND, "low_power_wake_lock");

    low_bat_flag = is_hisi_battery_reach_threshold();
    if(( low_bat_flag & BQ27510_FLAG_LOCK ) == BQ27510_FLAG_LOCK) {
        wake_lock(&low_power_lock);
        is_low_power_locked = 1;
    }


    INIT_DELAYED_WORK(&di->bq_bci_monitor_work,
                bq_bci_battery_work);
    schedule_delayed_work(&di->bq_bci_monitor_work, 0);

    di->nb.notifier_call = bq_battery_event_handler;/*battery event*/
    bq_register_notifier(&di->nb, 1);
    dev_err(&pdev->dev, "bq_bci probe ok!\n");

    return 0;
failed:
    cancel_delayed_work(&di->bq_bci_monitor_work);
    wake_lock_destroy(&low_power_lock);
    platform_set_drvdata(pdev, NULL);
    kfree(di);
    di = NULL;
    return ret;
}

static int bq_bci_battery_remove(struct platform_device *pdev)
{
    struct bq_bci_device_info *di = platform_get_drvdata(pdev);

    if (di == NULL) {
        dev_err(&pdev->dev, "di is NULL!\n");
        return -ENODEV;
    }

    bq_unregister_notifier(&di->nb, 1);
    cancel_delayed_work_sync(&di->bq_bci_monitor_work);
    wake_lock_destroy(&low_power_lock);
    platform_set_drvdata(pdev, NULL);
    kfree(di);
    di = NULL;

    return 0;
}

static void bq_bci_battery_shutdown(struct platform_device *pdev)
{
    struct bq_bci_device_info *di = platform_get_drvdata(pdev);

    if (di == NULL) {
        dev_err(&pdev->dev, "di is NULL!\n");
        return;
    }

    cancel_delayed_work_sync(&di->bq_bci_monitor_work);
    wake_lock_destroy(&low_power_lock);

    return;
}

#ifdef CONFIG_PM
static int bq_bci_battery_suspend(struct platform_device *pdev,pm_message_t state)
{
    struct bq_bci_device_info *di = platform_get_drvdata(pdev);
    
    dev_err(&pdev->dev, "suspend enter!\n");

    if (di == NULL) {
        dev_err(&pdev->dev, "di is NULL!\n");
        return -ENODEV;
    }
    suspend_capacity = hisi_battery_capacity();

    cancel_delayed_work(&di->bq_bci_monitor_work);

    return 0;
}

static int bq_bci_battery_resume(struct platform_device *pdev)
{
    struct bq_bci_device_info *di = platform_get_drvdata(pdev);
    int i = 0, resume_capacity = 0;
    
    dev_err(&pdev->dev, "resume enter!\n");

    if (di == NULL) {
        dev_err(&pdev->dev, "di is NULL!\n");
        return -ENODEV;
    }
    resume_capacity = hisi_battery_capacity();
    if(di->charge_status == POWER_SUPPLY_STATUS_DISCHARGING
        || di->charge_status == POWER_SUPPLY_STATUS_NOT_CHARGING) {
        if ((suspend_capacity - resume_capacity)>=2) {
            capacity_sum = 0;
            for(i=0;i<WINDOW_LEN;i++) {
                    capacity_filter[i] = resume_capacity;
                    capacity_sum += capacity_filter[i];
            }
        }
    }
    schedule_delayed_work(&di->bq_bci_monitor_work, 0);

    return 0;
}
#endif /* CONFIG_PM */

struct platform_device bq_bci_battery_dev = {
    .name	= "huawei,bq_bci_battery",
	.id	    = -1,
	.dev 	= {
		.platform_data	= NULL,
	}
};

static struct of_device_id bq_bci_battery_match_table[] =
{
    {
	.compatible = "huawei,bq_bci_battery",
	.data = NULL,
    },
    {
    },
};
static struct platform_driver bq_bci_battery_driver = {
    .probe = bq_bci_battery_probe,
    .remove = bq_bci_battery_remove,
#ifdef CONFIG_PM
    .suspend = bq_bci_battery_suspend,
    .resume = bq_bci_battery_resume,
#endif
    .shutdown = bq_bci_battery_shutdown,
    .driver = {
        .name = "huawei,bq_bci_battery",
	.owner = THIS_MODULE,
	.of_match_table = of_match_ptr(bq_bci_battery_match_table),
    },
};

static int __init bq_battery_init(void)
{
    int ret = -1;
    
    ret = platform_device_register(&bq_bci_battery_dev);
    if(ret)
    {
        printk(KERN_ERR "register bq_bci device failed\r\n");
        return ret;
    }
        
    ret = platform_driver_register(&bq_bci_battery_driver);
    if(ret)
    {
        printk(KERN_ERR "register bq_bci driver failed\r\n");
        platform_device_unregister(&bq_bci_battery_dev);
    }
    
    return ret;    
}

module_init(bq_battery_init);

static void __exit bq_battery_exit(void)
{
    platform_driver_unregister(&bq_bci_battery_driver);
}

module_exit(bq_battery_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:bq_bci");
MODULE_AUTHOR("HUAWEI Inc");


