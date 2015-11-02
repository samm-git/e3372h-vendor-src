#ifndef __CHG_BOOT_SMARTSTAR_COUL
#define __CHG_BOOT_SMARTSTAR_COUL

int wait_ss_coul_device_work(void);
int get_ss_bat_voltage(void);
int get_ss_bat_temperature(void);
int get_ss_bat_capacity_state(void);
int is_smartstar_battery_exist(void);
int check_ss_bat_present(void);

#endif
