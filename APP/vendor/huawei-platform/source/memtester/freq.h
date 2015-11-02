/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

#ifndef __FREQ_H_
#define __FREQ_H_/*lint !e750*/

struct fixed_freq
{
    ul min;/*lint !e10*/
    ul max;/*lint !e148 !e38 !e10*/
};
extern int jump_freq_exit ;

int get_available_freqs(void);
void show_available_freqs(void);
void show_current_freq(void);
int is_freq_valid(ul freq);/*lint !e101 !e132 !e10*/
int set_scaling_freqs(int cpu, ul max, ul min);/*lint !e101 !e132 !e49 !e10*/
void *auto_freq_jump(const void *args);
void *fixed_freq(const void *args);

#endif
/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */


