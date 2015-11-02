#include "balongv7r2/chg_boot.h"
#include "balongv7r2/lcd.h"
#include <boot/boot.h>


#if (MBB_OLED == FEATURE_ON)
void oledshowforceupdate(void)
{
   /* oled 屏强制升级显示 */
    oled_boot_show_update();
}
#endif
chg_batt_check_type chg_battery_check_boot()
{
    return CHARGE_BATTERY_GOOD;
}

#if (MBB_COMMON == FEATURE_ON)
void lcdPowerOnByString(char* string)
{
   /* lcd 屏强制升级显示 */ 
#if (MBB_LCD == FEATURE_ON)
    lcd_power_on(string);
#elif (MBB_OLED == FEATURE_ON)
    oledshowforceupdate();
#endif
}
#endif

s32 display_logo( void )
{
#if (MBB_COMMON == FEATURE_ON)

#if (MBB_LCD == FEATURE_ON)
    return lcd_power_on(NULL);
#elif (MBB_OLED == FEATURE_ON)
    /* OLED 屏开机logo显示  */
    return oled_power_on(NULL);
#endif

#else
    return lcd_power_on();
#endif
}

void display_string(unsigned char* string)
{
#if (MBB_COMMON == FEATURE_ON)
#if (MBB_LCD == FEATURE_ON)
    (void)balong_lcd_display_allblack();
#endif
#endif
#if (MBB_LCD == FEATURE_ON)
    lcd_show_string(string);
#endif
}

void display_upgrade( void )
{
    return ;
}

void boot_show_battery_bad( void )
{
    display_string("BAD BATTERY");
}

void boot_show_battery_over_tmp()
{
    display_string("OVER HEATED");
}

void boot_show_battery_low( void )
{
    display_string("LOW BATTERY");
}

