/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

#include <stdio.h>
#include <stdlib.h>
/*lint -e451 */
#include <string.h>
#include <unistd.h>
#include <errno.h>
/*lint +e451 */
#include <dirent.h>
#include <sys/syscall.h>
#include <unistd.h>
/*lint -e613 -e620 -e752*/
#include "log.h"
#include "types.h"
#include "freq.h"
#include "common.h"
#include <cutils/properties.h>/*lint !e322 !e7*/



#define DEBUG         0

#define MAX_PATH (256)
#define FREQ_COUNT_MIN (2)

#if DEBUG
#  define DBG        printf
#else
#  define DBG(fmt,...)
#endif
int jump_freq_exit = 0 ;

enum CORE_STATUS
{
    CORE_DOWN = 0,
    CORE_UP = 1,
};

#define CPU_SYSFS            "/sys/bus/cpu/devices/"

#define AVAILABLE_FREQ                "/sys/bus/cpu/devices/cpu0/cpufreq/scaling_available_frequencies"

//#define CPU_MIN_FREQ                 "/cpufreq/scaling_min_freq"
//#define CPU_MAX_FREQ                "/cpufreq/scaling_max_freq"
#define CPU_CURR_FREQ                "/cpufreq/scaling_cur_freq"
#define CPU_GOVERNOR             "/cpufreq/scaling_governor"

#define DDR_TEST_SETAFFINITY  (__NR_SYSCALL_BASE+241) /* ARM arch specific */


ul avail_freqs[20] = {0};
ul min_freq = 0;
ul max_freq = 0;
int max_level = 0;
int cpu_count = 1;
unsigned int cpu_status = 1;

int read_string_from_file(char *filename, char *str, int maxlen )
{
    FILE *fd;
    int cnt = 0;

    fd = fopen(filename, "r");
    if (fd != NULL)
    {
        /* ensure NULL terminated, sizeof(buf) - 1 */
        cnt = (int)fread( str, 1, (size_t)(maxlen - 1), fd );
        DBG("read_string_from_file fread:%s,%d\n", str, cnt );
        str[maxlen-1] = 0;
        fclose(fd);
    }
    else
    {
        cnt = -1;
        printf("Error %d getting %s\n", errno, filename);
    }

    return cnt;
}

int write_string_to_file(const char *str, const char *filename )
{
    FILE *fd;
    int ret = 0;

    fd = fopen(filename , "w");
    if ( fd != NULL )
    {
        if ( !fwrite(str, strlen(str), 1ul, fd ) )
        {
            ret = - EINVAL;
        }

        fflush(fd);
        fclose(fd);
    }
    else
    {
        ret = -ENODEV;
    }

    DBG("write_string_to_file:%d\n", ret);

    return ret;
}

/* Update CPU hotplug 'online' sysfs node
   * cpu - core id
   * on - 0/1 for offline/online
   Return value: 0 if successful, -errno if error */
static int __cpu_up(int cpu, int on)
{
    int ret = 0;
    FILE *fd;
    char filename[MAX_PATH] = {0};
    char val[3] = {0};

    snprintf(filename, MAX_PATH, "/sys/devices/system/cpu/cpu%d/online", cpu);
    snprintf(val, 3, "%d\n", (on ? 1 : 0));

    fd = fopen(filename, "w");
    if (fd != NULL)
    {
        if (fwrite( val, sizeof(val), 1, fd) == 0)
        {
            ret = -EINVAL;
        }
        fflush(fd);
        fclose(fd);
    }
    else
    {
        ret = -ENODEV;
    }

    if (ret)
    {
        printf("Error %d setting %s online status to %s\n", errno, filename, val);
    }
    else
    {
        if ( on == CORE_UP )
        {
            cpu_status |= 1ul << cpu ;
        }
        else
        {
            cpu_status &= (~( 1ul << cpu )) & 0xFFFFFFFF;
        }
    }
    DBG("cpu %d : on:%d,status:%u\n", cpu, on, cpu_status );
    return ret;
}

/*
static void all_cpus_up( void )
{
        int cpu_id;

        for( cpu_id = 1; cpu_id < cpu_count; cpu_id++ )
        {
                __cpu_up( cpu_id, CORE_UP );
        }
}
*/

/* Utility function to read cpu
   * cpu - core id
   * on - 0/1 for offline/online
   Return value: 0/1 core status, -errno if error */
static int get_cpu_status(int cpu)
{
    int status = CORE_DOWN;
    FILE *fd;
    char filename[MAX_PATH] = {0};
    char buf[3] = {0};

    snprintf(filename, MAX_PATH, "/sys/devices/system/cpu/cpu%d/online", cpu);
    fd = fopen(filename, "r");
    if (fd != NULL)
    {
        /* ensure NULL terminated, sizeof(buf) - 1 */
        if (fread( buf, sizeof(buf) - 1, 1, fd ) == 0)
        {
            buf[0] = '0';
        }

        buf[1] = '\0';
        status = atoi(buf) ? CORE_UP : CORE_DOWN;
        DBG("get_cpu_status status:%d\n", status );

        fclose(fd);

    }
    else
    {
        printf("Error %d getting cpu %d online status at %s\n", errno, cpu, filename);
    }

    return status;
}

/* Get current number of online CPUs
   Return value: number of online CPUs */
static int get_num_cpus()
{
    DIR *tdir;
    int ncpus = 0;
    char cwd[MAX_PATH] = {0};
    int cpu = 0;
    struct dirent *cpu_dirent;

    if (!getcwd(cwd, sizeof(cwd)))
    {
        return 0;
    }

    chdir(CPU_SYSFS); /* Change dir to read the entries. Doesnt work
                                 otherwise */
    tdir = opendir(CPU_SYSFS);
    if (!tdir)
    {
        printf("Unable to open %s\n", CPU_SYSFS);
        return 0;
    }

    for ( cpu_dirent = readdir(tdir);cpu_dirent != NULL; cpu_dirent = readdir(tdir) )
    {
        if (strstr(cpu_dirent->d_name, "cpu") == NULL)
        {
            continue;
        }
        if (!sscanf(cpu_dirent->d_name, "cpu%d", &cpu))
        {
            continue;
        }
        ncpus++;
    }

    closedir(tdir);
    chdir(cwd); /* Restore current working dir */

    DBG("Number of cores %d\n", ncpus);

    return ncpus;
}


int get_available_freqs(void)
{
    FILE *fp;
    char linebuf[128];
    char *str_freq;
    int i = 0;

    fp = fopen( AVAILABLE_FREQ, "r");
    if (!fp)
    {
        printf("failed to open scaling_available_frequencies");
        return -1;
    }

    memset( (void*)linebuf, 0 , sizeof(linebuf) );
    fgets(linebuf, 128, fp);
    DBG("linebuf: %s\n", linebuf);

    str_freq = strtok(linebuf, " ");
    while (str_freq)
    {
        avail_freqs[i++] = strtoul(str_freq, NULL, 10);
        str_freq = strtok(NULL, " ");
    }

    if ( i < FREQ_COUNT_MIN )
    {
        return -1;
    }

    min_freq = avail_freqs[0];
    max_freq = avail_freqs[i-FREQ_COUNT_MIN];
    max_level = i - 1;

    fclose(fp);

    return 0;
}

void show_available_freqs(void)
{
    int i;

    printf("available frequencies:\n");
    for (i = 0; i < 20; i++)
    {
        if (avail_freqs[i])
        {
            printf("%lu ", avail_freqs[i]);
        }
    }
    printf("\n");
}

static ul get_current_freq( int cpu )
{
    FILE *fp;
    char filename[MAX_PATH];
    char linebuf[10] = {0};
    ul freq = 0;

    memset((void*)filename, 0 , sizeof(filename));
    snprintf(filename, MAX_PATH, "%scpu%d%s", CPU_SYSFS, cpu, CPU_CURR_FREQ );

    fp = fopen(filename, "r");
    if (!fp)
    {
        printf("failed to open cpu%d scaling_cur_freq\n", cpu );
        return freq;
    }
    fgets(linebuf, 10, fp);
    freq = strtoul(linebuf, NULL, 10);
    fclose(fp);
    DBG("get_current_freq %lu\n", freq);
    return freq;

}

void show_current_freq( void )
{
    int cpu_id;
    ul freq;

    printf("Current Frequency: ");
    for ( cpu_id = 0; cpu_id < cpu_count; cpu_id++ )
    {
        freq = get_current_freq(cpu_id);
        if ( freq )
        {
            printf(" (%d)%lu;", cpu_id, freq );
        }
        else
        {
            printf(" (%d)offline;", cpu_id );
        }
    }
    printf("\n");
}

void show_current_freq_buf( char *buf , int thread_id )
{
    int cpu_id;
    ul freq;
    extern int g_log_buffered;

    if ( thread_id == -1 )
    {
        log_put( g_log_buffered, buf, 0, "Current Frequency: ");
    }
    else
    {
        log_put( g_log_buffered, buf, 0, "[%d]Current Frequency: ", thread_id);
    }

    for ( cpu_id = 0; cpu_id < cpu_count; cpu_id++ )
    {
        freq = get_current_freq(cpu_id);
        if ( freq )
        {
            log_put( g_log_buffered, buf, 0, " (%d)%lu;", cpu_id, freq );
        }
        else
        {
            log_put( g_log_buffered, buf, 0, " (%d)offline;", cpu_id );
        }
    }
    log_put( g_log_buffered, buf, 0, "\n");
}

int is_freq_valid(ul freq)
{
    int i;
    DBG("is_freq_valid:%lu,%d\n", freq, max_level );
    for (i = 0; i < max_level; i++)
    {
        if (freq == avail_freqs[i])
        {
            return 1;
        }
    }

    return 0;
}

static void set_cpu_freq(ul value, int cpu, char *type)
{
    char filename[MAX_PATH];
    FILE *fp;
    char val[20] = {0};
    size_t ret;

    DBG("set_cpu_freq %lu,cpu:%d,type:%s\n", value, cpu, type );

    if ( get_cpu_status(cpu) == CORE_DOWN )
    {
        printf("set_cpu_freq exit as coredown");
        return;
    }

    memset((void*)filename, 0 , sizeof(filename));
    snprintf(filename, MAX_PATH,
             "/sys/bus/cpu/devices/cpu%d/cpufreq/scaling_%s_freq", cpu, type );
    fp = fopen(filename, "w");
    if ( !fp )
    {
        printf("set_cpu_freq %s return\n", filename );
        return;
    }
    snprintf( val, 20, "%lu", value );

    ret = fwrite( val , 1, sizeof(val), fp );
    if ( !ret )
    {
        printf("set_cpu_freq fwrite %lu\n", ret);
    }

    fflush( fp );
    fclose( fp );

}

int set_scaling_freqs(int cpu, ul min, ul max)
{
    DBG("cpu:%d ,min = %lu, max = %lu\n", cpu, min, max);

    if (max < min)
    {
        printf("max freq is lower min freq\n");
        return 0;
    }

    if (max < min_freq)
    {
        printf("max freq cannot lower than %lu\n", min_freq);
        return 0;
    }

    if (min > max_freq)
    {
        printf("min freq cannot higher than %lu\n", max_freq);
        return 0;
    }

    if (!is_freq_valid(min))
    {
        printf("invalid min freq: %lu\n", min);
        show_available_freqs();
        return 0;
    }

    if (!is_freq_valid(max))
    {
        printf("invalid max freq: %lu\n", max);
        show_available_freqs();
        return 0;
    }

    set_cpu_freq(max, cpu, "max");
    set_cpu_freq(min, cpu, "min");

    return 1;
}


static void set_all_cpu_freqs(ul min, ul max)
{
    int cpu_id;
    ul freq;

    DBG("set_all_cpu_freqs %d,%lu,%lu\n", cpu_count, min, max);

    for (cpu_id = 0;cpu_id < cpu_count;cpu_id++)
    {
        set_scaling_freqs(cpu_id, min, max );
    }

    freq = get_current_freq(0);

    log_put( 0, NULL, 1, "    set cpu freq to [%lu,%lu], curr [%lu]\n", min, max , freq );
}

static void set_cpu_attribute( void )
{
    int cpu_id;
    char filename[MAX_PATH];

    for ( cpu_id = 0 ; cpu_id < cpu_count; cpu_id++)
    {
        memset( (void*)filename , 0 , sizeof(filename) );
        sprintf( filename , "%scpu%d%s", CPU_SYSFS, cpu_id, CPU_GOVERNOR );
        write_string_to_file("ondemand", filename );
    }
}

void *fixed_freq(const void *args)
{
    ul min = ((struct fixed_freq *)args)->min;
    ul max = ((struct fixed_freq *)args)->max;
    extern int tests_finished;
    extern unsigned int cur_test_case;
    extern int time_is_up;

    do
    {
        set_all_cpu_freqs(min, max);

        sleep(10);
        }while(!(tests_finished||time_is_up));

    printf("Done!\n");

    if ( tests_finished )
    {
        return NULL;
    }

    if ( cur_test_case == M_TEST_SET_KERNEL )
    {
        write_string_to_file("0", "/sys/ddr_tester/state");
    }
    else
    {
        jump_freq_exit = 1 ;
    }

    return NULL;
}

void *auto_freq_jump(const void *args)
{
    ul freq = 0;
    int i, j, k;
    extern int tests_finished;
    extern unsigned int cur_test_case;
    extern int time_is_up;

    args = args;

    for (k = 1;k < max_level / 2; k++)
    {
        for (j = 0; j < 10; j++)
        {
            /* cpu0: level n, level n-k, ... level 1 */
            /* cpu1: level n, level n-k, ... level 1 */
            for (i = max_level - 1; i >= 0; i -= k)
            {
                freq = avail_freqs[i];
                if (!freq)
                {
                    continue;
                }
                /*DEBUG
                timestamp_print_time(stdout,0);
                printf("setfreq 2[%d]:k:%d,j:%d,i:%d,freq:%lu\n",times++,k,j,i,freq);
                fflush(stdout);
                log_put( 0, NULL, 1, "        set cpu freq to %lu [%d] \n",freq, times++ );
                */
                if (tests_finished)
                {
                    return NULL;
                }
                if ( time_is_up )
                {
                    break;
                }

                set_all_cpu_freqs(freq, freq);
                sleep(5);
            }

            /* cpu0: level 1, level 1+k, ... , level n */
            /* cpu1: level 1, level 1+k, ... , level n  */
            for (i = 0; i < max_level; i += k)
            {
                freq = avail_freqs[i];
                if (!freq)
                {
                    continue;
                }
                /* DEBUG
                timestamp_print_time(stdout,0);
                printf("setfreq 1[%d]:k:%d,j:%d,i:%d,freq:%lu\n",times++,k,j,i,freq);
                fflush(stdout);
                log_put( 0, NULL, 1, "        set cpu freq to %lu [%d] \n",freq, times++ );
                */
                if (tests_finished)
                {
                    return NULL;
                }
                if ( time_is_up )
                {
                    break;
                }

                set_all_cpu_freqs(freq, freq);
                sleep(5);
            }

            if ( time_is_up )
            {
                break;
            }
        }

        if ( time_is_up )
        {
            break;
        }
    }
    if ( tests_finished )
    {
        return NULL;
    }

    printf("Done![args=%lx]\n", (ul)args);
    if ( cur_test_case == M_TEST_SET_KERNEL )
    {
        write_string_to_file("0", "/sys/ddr_tester/state");
    }
    else
    {
        jump_freq_exit = 1 ;
    }
    return 0 ;
}

void prepare_test( void )
{
    int cpu_id;

    cpu_count = get_num_cpus();

    for ( cpu_id = 1; cpu_id < cpu_count; cpu_id++ )
    {
        if ( get_cpu_status(cpu_id) == CORE_UP )
        {
            cpu_status |= 1ul << cpu_id;
        }
        else
        {
            cpu_status &= ~(1ul << cpu_id) & 0xFFFFFFFF;
        }
    }

    if ( property_set("recovery.all_cpus_online", "true") < 0 )/*lint !e718*/
    {
        printf("Failed in boot up all cpus !\n");
    }

    sleep(1);

    set_cpu_attribute();
}

void exit_test( void )
{
    int cpu_id;

    for ( cpu_id = 1; cpu_id < cpu_count; cpu_id++ )
    {
        if ( ( cpu_status & (1ul << cpu_id)) == 0 )
        {
            __cpu_up( cpu_id , CORE_DOWN );
        }
    }
}

void run_in_cpu( unsigned int cpu_set )
{
    int ret;
    DBG("run_in_cpu %u\n", cpu_set);
    /*lint -e40 */
    ret = syscall(DDR_TEST_SETAFFINITY, 0, sizeof(cpu_set), &cpu_set);
    /*lint +e40 */
    if (ret < 0)
    {
        printf("Cannot set cpu affinity: %s\n", strerror(-ret));
    }

}
/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */

