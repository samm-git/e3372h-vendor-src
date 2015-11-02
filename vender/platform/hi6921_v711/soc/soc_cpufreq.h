#ifndef __SOC_CPUFREQ_H
#define __SOC_CPUFREQ_H



/*采用同步调频方案控制宏*/
#ifndef CPUFREQ_IS_SYNC_DDR
#define CPUFREQ_IS_SYNC_DDR
#endif

/*区分采用哪个平台的频率值*/
#ifndef CPUFREQ_PLATFORM_HI6930_V711
#define CPUFREQ_PLATFORM_HI6930_V711
#endif


enum cpufreq_profile{
	BALONG_FREQ_PRO0 = 0,
	BALONG_FREQ_PRO1,
	BALONG_FREQ_PRO2,
	BALONG_FREQ_PRO3,
	BALONG_FREQ_PRO4,
	BALONG_FREQ_PRO5,
	BALONG_FREQ_PRO6,
	DC_RESV
};
#endif
