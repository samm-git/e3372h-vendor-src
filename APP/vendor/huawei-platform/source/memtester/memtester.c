/* < DTS2013093005579 xiaohui 20131009 begin */
/* < DTS2013112300820 xiaohui 20131123 begin */

/*
 * memtester version 4
 *
 * Very simple but very effective user-space memory tester.
 * Originally by Simon Kirby <sim@stormix.com> <sim@neato.org>
 * Version 2 by Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Version 3 not publicly released.
 * Version 4 rewrite:
 * Copyright (C) 2004-2012 Charles Cazabon <charlesc-memtester@pyropus.ca>
 * Licensed under the terms of the GNU General Public License version 2 (only).
 * See the file COPYING for details.
 *
 */

#define __version__ "4.3.0"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
/*lint -e451 */
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
/*lint +e451 */

#include "types.h"
#include "sizes.h"
#include "tests.h"
#include "freq.h"
#include "log.h"
#include "common.h"

/*lint -e613 -e718*/
#define DBG printf
//#define DBG(fmt,...)

#define EXIT_FAIL_NONSTARTER    0x01
#define EXIT_FAIL_ADDRESSLINES  0x02
#define EXIT_FAIL_OTHERTEST     0x04
#define EXIT_FAIL_NORAM         0x08

struct test tests[] =
{
    {"prbs_data_align", test_prbs_data_align_test_7},
    {"prbs_data_flip", test_prbs_data_flip_test_7},
    {"walking_ones", test_walking_ones},
    {"variation_walking_ones", test_variation_walking_ones},
    {"test_own_addr", test_own_addr},
    { "Random Value", test_random_value },
    { "Compare XOR", test_xor_comparison },
    { "Compare SUB", test_sub_comparison },
    { "Compare MUL", test_mul_comparison },
    { "Compare DIV", test_div_comparison },
    { "Compare OR", test_or_comparison },
    { "Compare AND", test_and_comparison },
    { "Sequential Increment", test_seqinc_comparison },
    { "Solid Bits", test_solidbits_comparison },
    { "Block Sequential", test_blockseq_comparison },
    { "Checkerboard", test_checkerboard_comparison },
    { "Bit Spread", test_bitspread_comparison },
    { "Bit Flip", test_bitflip_comparison },
    { "Walking Ones", test_walkbits1_comparison },
    { "Walking Zeroes", test_walkbits0_comparison },
#ifdef TEST_NARROW_WRITES
    { "8-bit Writes", test_8bit_wide_random },
    { "16-bit Writes", test_16bit_wide_random },
#endif
    { NULL, NULL }
};

/* Sanity checks and portability helper macros. */
#ifdef _SC_VERSION
void check_posix_system(void)
{
    if (sysconf(_SC_VERSION) < 198808L)
    {
        fprintf(stderr, "A POSIX system is required.  Don't be surprised if "
                "this craps out.\n");
        fprintf(stderr, "_SC_VERSION is %d\n", sysconf(_SC_VERSION));
    }
}
#else
#define check_posix_system()
#endif

#ifdef _SC_PAGE_SIZE
size_t memtester_pagesize(void)
{
    size_t pagesize;
    int size = sysconf(_SC_PAGE_SIZE);
    if (size == -1)
    {
        perror("get page size failed");
        exit(EXIT_FAIL_NONSTARTER);
    }
    pagesize = (size_t)size;
    printf("pagesize is %lu\n", pagesize);
    return pagesize;
}
#else
size_t memtester_pagesize(void)
{
    printf("sysconf(_SC_PAGE_SIZE) not supported; using pagesize of 8192\n");
    return 8192;
}
#endif

/* Some systems don't define MAP_LOCKED.  Define it to 0 here
   so it's just a no-op when ORed with other constants. */
#ifndef MAP_LOCKED
#define MAP_LOCKED 0
#endif

/* Function declarations */
void usage(char *me);

/* Global vars - so tests have access to this information */

unsigned int tests_set = M_TEST_SET_USER;
unsigned int cur_test_case = 0;

int use_phys = 0;
off_t physaddrbase = 0;
extern int cpu_count;
extern int jump_freq_exit;
int assist_count = 0;
int have_assist = 0;
pthread_t assist_tids[MAX_THREADS];
int thread_count = 1;
int is_auto_freq = 0;
int is_fix_freq = 0;
pthread_t fix_tid = 0;
pthread_t auto_tid = 0;
int time_is_up = 0;
int g_log_buffered = 0;
int tests_broken = 0;
struct fixed_freq g_fix_freq =
{
    0, 0
};
int g_duration = 0;

typedef struct s_bufpool
{
    void volatile *buf;
    size_t bufsize;
}t_bufpool;

t_bufpool bufpool[MAX_THREADS];

typedef struct s_para
{
    int thread_id ;
    ul  loops ;
    ul  testmask;
    int break_when_error;
}t_tests_para;
t_tests_para tests_para[MAX_THREADS];

int assist_para[MAX_THREADS];

pthread_t run_tids[MAX_THREADS];


int exit_code[MAX_THREADS] = {0};
int tests_finished;
int run_tests_thread( int thread_cnt, ul  loops, ul  testmask, int break_when_error );
int wait_thread_finished( int thread_nt );
int run_assist_thread(  int thread_nt );
int wait_assist_finished( int thread_cnt );
void bufpool_init( void volatile *buf, size_t bufsize, size_t thread_cnt, size_t pagesize );
void run_in_cpu( unsigned int cpu_id );
int run_kernel_tests( size_t buf_size, int thread_cnt, int assist_cnt, ul  loops);
int run_auto_freq_thread( void );
int run_userspace_tests( int thread_cnt, int assist_cnt, ul loops, ul testmask, int break_when_error );
void sigalarm_fn(int sig);
void print_kernel_progress( void );

extern int init_module(void *, unsigned long, const char *);
extern void *load_file(const char *fn, unsigned *sz);


/* Function definitions */
void usage(char *me)
{
    fprintf(stderr, "\n"
            /*  "Usage: %s [-p physaddrbase [-d device]] <mem>[B|K|M|G] [loops]\n" */
            "Usage: %s [-j threadcount] [-k assistcount] [-t alg_type] [-l duration] [-c] [-b] [-a] <mem>[B|K|M|G] [loops]\n"
            "         -a           auto frequency jump                     \n"
            "         -f min--max  fix frequencies from min to max         \n"
            "         -j XX        XX is thread count for running algorithm\n"
            "                      if XX=0 or XX>=cpus,it's same to cpu count\n"
            "                      default is 1                            \n"
            "         -k XX        XX is thread count for assistance       \n"
            "                      max is 32, and default is 0             \n"
            "         -t XX        XX is bitmap of userpace and kernel alg \n"
            "                      1:userspace(default),2:kernel,3:both    \n"
            "                      1 is userspace, 2 is kernel,3 for both  \n"
            "         -s           show available frequencies              \n"
            "         -l           duration                                \n"
            "         -b           break on error                          \n"
            "         -c           cache the log , default no cache        \n",

            me);
    fflush(stdout);
    exit(EXIT_FAIL_NONSTARTER);
}

static const char *TEMPORARY_LOG_FILE = "/hardwaretest/memtest.log";

int main(int argc, char **argv)
{
    ul loops;
    size_t pagesize, wantraw, wantmb, wantbytes, wantbytes_orig;
    size_t bufsize = 0;
    char *memsuffix, *addrsuffix, *loopsuffix;
    size_t pagesizemask;
    void volatile *buf, *aligned = NULL;
    int do_mlock = 1, done_mem = 0;
    int memfd, opt, memshift;
    size_t maxbytes = 0xFFFFFFFF; /* addressable memory, in bytes */
    size_t maxmb = (maxbytes >> 20) + 1; /* addressable memory, in MB */
    /* Device to mmap memory from with -p, default is normal core */
    char *device_name = "/dev/mem";
    struct stat statbuf;
    int device_specified = 0;
    char *env_testmask;
    ul testmask = 0;
    int thread_id = 0;
    int break_when_error = 0 ;
    int status = 0;
    int status_userspace = 0;

    freopen(TEMPORARY_LOG_FILE, "a", stdout);
    setbuf(stdout, NULL);
    freopen(TEMPORARY_LOG_FILE, "a", stderr);
    setbuf(stderr, NULL);


    get_args( &argc , &argv ) ;
    printf("memtester version " __version__ " (%d-bit)\n", UL_LEN);
    printf("Copyright (C) 2001-2012 Charles Cazabon.\n");
    printf("Licensed under the GNU General Public License version 2 (only).\n");
    printf("\n");
    check_posix_system();
    pagesize = (size_t)memtester_pagesize();
    pagesizemask = ~(pagesize - 1);
    printf("pagesizemask is 0x%tx\n", pagesizemask);
    fflush(stdout);
    get_available_freqs();

//single CPU delete by ouyangxun
#if 0
    prepare_test();
#endif


    /* If MEMTESTER_TEST_MASK is set, we use its value as a mask of which
       tests we run.
     */

    env_testmask = getenv("MEMTESTER_TEST_MASK");
    if ( env_testmask )
    {
        errno = 0;
        testmask = strtoul(env_testmask, 0, 0);
        if (errno)
        {
            fprintf(stderr, "error parsing MEMTESTER_TEST_MASK %s: %s\n",
                    env_testmask, strerror(errno));
            usage(argv[0]); /* doesn't return */
        }
        printf("using testmask 0x%lx\n", testmask);
    }

    while ((opt = getopt(argc, argv, "p:d:f:j:k:l:t:sab")) != -1)
    {
        switch (opt)
        {
            case 'p':
                errno = 0;
                physaddrbase = (off_t) strtoull(optarg, &addrsuffix, 16);
                if (errno != 0)
                {
                    fprintf(stderr,
                            "failed to parse physaddrbase arg; should be hex "
                            "address (0x123...)\n");
                    usage(argv[0]); /* doesn't return */
                }
                if (*addrsuffix != '\0')
                {
                    /* got an invalid character in the address */
                    fprintf(stderr,
                            "failed to parse physaddrbase arg; should be hex "
                            "address (0x123...)\n");
                    usage(argv[0]); /* doesn't return */
                }
                if ((size_t)physaddrbase & (pagesize - 1))
                {
                    fprintf(stderr,
                            "bad physaddrbase arg; does not start on page "
                            "boundary\n");
                    usage(argv[0]); /* doesn't return */
                }
                /* okay, got address */
                use_phys = 1;
                break;
            case 'd':
                if (stat(optarg, &statbuf))
                {
                    fprintf(stderr, "can not use %s as device: %s\n", optarg,
                            strerror(errno));
                    usage(argv[0]); /* doesn't return */
                }
                else
                {
                    if (!S_ISCHR(statbuf.st_mode))
                    {
                        fprintf(stderr, "can not mmap non-char device %s\n",
                                optarg);
                        usage(argv[0]); /* doesn't return */
                    }
                    else
                    {
                        device_name = optarg;
                        device_specified = 1;
                    }
                }
                break;
            case 's':
                show_available_freqs();
                exit(EXIT_FAIL_NONSTARTER);
                break;
            case 'f':
            {
                char *str_freq;

                if ( is_auto_freq )
                {
                    usage(argv[0]);
                }

                /* parse min and max frequencies from args */
                str_freq = strtok(optarg, "--");
                if (str_freq)
                {
                    g_fix_freq.min = strtoul(str_freq, NULL, 10);
                    str_freq = strtok(NULL, "--");
                    if (str_freq)
                    {
                        g_fix_freq.max = strtoul(str_freq, NULL, 10);
                    }
                    else
                    {
                        fprintf(stdout, "need max freq\n");
                        usage(argv[0]);
                    }
                }
                else
                {
                    fprintf(stdout, "need specify min/max freqs\n");
                    usage(argv[0]);
                }
                printf("got fix freq %lu,%lu\n", g_fix_freq.min, g_fix_freq.max );
                is_fix_freq = 1;
                break;
            }
            case 'j':
            {
                errno = 0;
                thread_count = atoi(optarg);
                if (errno != 0)
                {
                    fprintf(stderr,
                            "failed to parse thread count arg; should be integer \n");
                    usage(argv[0]);
                }
                if (thread_count < 0 )
                {
                    fprintf(stderr, "bad thread count \n");
                    usage(argv[0]); /* doesn't return */
                }
                if ( thread_count == 0 || thread_count > cpu_count )
                {
                    thread_count = cpu_count;
                }

                if ( thread_count > MAX_THREADS )
                {
                    thread_count = MAX_THREADS;
                }

                break;
            }
            case 'k':
            {
                errno = 0;
                assist_count = atoi(optarg);
                if (errno != 0)
                {
                    fprintf(stderr,
                            "failed to parse assist count arg; should be integer \n");
                    usage(argv[0]);
                }
                if (assist_count < 0 )
                {
                    fprintf(stderr, "bad thread count \n");
                    usage(argv[0]); /* doesn't return */
                }
                if ( assist_count > MAX_THREADS )
                {
                    printf("Assist_count is larger than %d,set to %d\n", MAX_THREADS, MAX_THREADS );
                    assist_count = MAX_THREADS;
                }
                if ( assist_count != 0 )
                {
                    have_assist = 1;
                }
                else
                {
                    have_assist = 0;
                }

                break;
            }
            case 'l':
            {
                g_duration = 0;

                errno = 0;
                g_duration = atoi(optarg);
                if (errno != 0)
                {
                    fprintf(stderr,
                            "failed to parse duration arg; should be minitus \n");
                    usage(argv[0]);
                }
                if (g_duration <= 0 )
                {
                    fprintf(stderr, "bad duration,need to be larger than 0 \n");
                    usage(argv[0]); /* doesn't return */
                }
                g_duration *= 60;  //change to seconds
                signal( SIGALRM, sigalarm_fn );
                alarm( (unsigned int)g_duration );
                break;
            }
            case 't':
            {
                errno = 0;
                tests_set = (unsigned int)atoi(optarg);
                if (errno != 0)
                {
                    fprintf(stderr,
                            "failed to parse assist count arg; should be integer \n");
                    usage(argv[0]);
                }
                if ( !(tests_set & M_TEST_SET_ALL) )
                {
                    usage(argv[0]); /* doesn't return */
                }

                tests_set = tests_set & M_TEST_SET_ALL;

                break;
            }
            case 'a':
            {
                if ( is_fix_freq )
                {
                    usage(argv[0]);
                }

                is_auto_freq = 1;
                break;
            }
            case 'b':
            {
                break_when_error = 1 ;
                printf("break test when errors\n");
                break ;
            }
            case 'c':
            {
                g_log_buffered = 1;
                break;
            }
            default: /* '?' */
                usage(argv[0]); /* doesn't return */
        }
    }

    if (device_specified && !use_phys)
    {
        fprintf(stderr,
                "for mem device, physaddrbase (-p) must be specified\n");
        usage(argv[0]); /* doesn't return */
    }
    if (optind >= argc)
    {
        fprintf(stderr, "need memory argument, in MB\n");
        usage(argv[0]); /* doesn't return */
    }
    errno = 0;
    wantraw = (size_t) strtoul(argv[optind], &memsuffix, 0);
    if (errno != 0)
    {
        fprintf(stderr, "failed to parse memory argument");
        usage(argv[0]); /* doesn't return */
    }
    switch (*memsuffix)
    {
        case 'G':
        case 'g':
            memshift = 30; /* gigabytes */
            break;
        case 'M':
        case 'm':
            memshift = 20; /* megabytes */
            break;
        case 'K':
        case 'k':
            memshift = 10; /* kilobytes */
            break;
        case 'B':
        case 'b':
            memshift = 0; /* bytes*/
            break;
        case '\0':  /* no suffix */
            memshift = 20; /* megabytes */
            break;
        default:
            /* bad suffix */
            memshift = 0;
            usage(argv[0]); /* doesn't return */
    }
    wantbytes_orig = wantbytes = ((size_t) wantraw << memshift);
    wantmb = (wantbytes_orig >> 20);
    optind++;
    if (wantmb > maxmb)
    {
        fprintf(stderr, "This system can only address %llu MB.\n", (ull) maxmb);
        exit(EXIT_FAIL_NONSTARTER);
    }
    if (wantbytes < pagesize)
    {
        fprintf(stderr, "bytes %ld < pagesize %ld -- memory argument too large?\n",
                (long int)wantbytes, (long int)pagesize);
        exit(EXIT_FAIL_NONSTARTER);
    }
    if (optind >= argc)
    {
        loops = 0;
    }
    else
    {
        errno = 0;
        loops = strtoul(argv[optind], &loopsuffix, 0);
        if (errno != 0)
        {
            fprintf(stderr, "failed to parse number of loops");
            usage(argv[0]); /* doesn't return */
        }

        if ( (*loopsuffix != '\0') && (*loopsuffix != '\n' ) )
        {
            fprintf(stderr, "loop suffix %c\n", *loopsuffix);
            usage(argv[0]); /* doesn't return */
        }
    }

    if ( !is_fix_freq && !is_auto_freq && !g_duration && !loops )
    {
        loops = 1;
    }

    printf("want %lluMB (%llu bytes)\n", (ull) wantmb, (ull) wantbytes);
    buf = NULL;

    if ( g_log_buffered )
    {
        if (log_pool_init() < 0)
        {
            printf( " log pool init failed \n");
            exit(EXIT_FAIL_NORAM);
        }
    }

    if (use_phys)
    {
        memfd = open(device_name, O_RDWR | O_SYNC);
        if (memfd == -1)
        {
            fprintf(stderr, "failed to open %s for physical memory: %s\n",
                    device_name, strerror(errno));
            exit(EXIT_FAIL_NONSTARTER);
        }
        buf = (void volatile *) mmap(0, wantbytes, PROT_READ | PROT_WRITE,
                                     MAP_SHARED | MAP_LOCKED, memfd,
                                     physaddrbase);
        if (buf == MAP_FAILED)
        {
            fprintf(stderr, "failed to mmap %s for physical memory: %s\n",
                    device_name, strerror(errno));
            exit(EXIT_FAIL_NONSTARTER);
        }

        if (mlock((void *) buf, wantbytes) < 0)
        {
            fprintf(stderr, "failed to mlock mmap'ed space\n");
            do_mlock = 0;
        }

        bufsize = wantbytes; /* accept no less */
        aligned = buf;
        done_mem = 1;
    }

    while (!done_mem)
    {
        while (!buf && wantbytes)
        {
            buf = (void volatile *) malloc(wantbytes);
            if (!buf)
            {
                wantbytes -= pagesize;
            }
        }
        bufsize = wantbytes;
        printf("got  %lluMB (%llu bytes)", (ull) wantbytes >> 20,
               (ull) wantbytes);
        if (do_mlock)
        {
            printf(", trying mlock ...");
            if ((size_t) buf % pagesize)
            {
                /* printf("aligning to page -- was 0x%tx\n", buf); */
                aligned = (ulv volatile *) ((size_t) buf & pagesizemask) + pagesize;
                /* printf("  now 0x%tx -- lost %d bytes\n", aligned,
                 *      (size_t) aligned - (size_t) buf);
                 */
                bufsize -= ((size_t) aligned - (size_t) buf);
            }
            else
            {
                aligned = buf;
            }
            /* Try mlock */
            if (mlock((void *) aligned, bufsize) < 0)
            {
                switch (errno)
                {
                    case EAGAIN: /* BSDs */
                        printf("over system/pre-process limit, reducing...\n");
                        free((void *) buf);
                        buf = NULL;
                        wantbytes -= pagesize;
                        break;
                    case ENOMEM:
                        printf("too many pages, reducing...\n");
                        free((void *) buf);
                        buf = NULL;
                        wantbytes -= pagesize;
                        break;
                    case EPERM:
                        printf("insufficient permission.\n");
                        printf("Trying again, unlocked:\n");
                        do_mlock = 0;
                        free((void *) buf);
                        buf = NULL;
                        wantbytes = wantbytes_orig;
                        break;
                    default:
                        printf("failed for unknown reason.\n");
                        do_mlock = 0;
                        done_mem = 1;
                }
            }
            else
            {
                printf("locked.\n");
                done_mem = 1;
            }
        }
        else
        {
            done_mem = 1;
            printf("\n");
        }
    }

    if (!do_mlock)
    {
        fprintf(stderr, "Continuing with unlocked memory; testing "
                "will be slower and less reliable.\n");
    }

    bufpool_init( aligned, bufsize , (size_t)thread_count, pagesize );

    if ( ( tests_set & M_TEST_SET_USER ) && !time_is_up  )
    {
        status = run_userspace_tests( thread_count, assist_count, loops, testmask, break_when_error );
        status_userspace = status;
        status = 0;
    }

    if (do_mlock)
    {
        munlock((void *)aligned, bufsize);
    }

    if ( buf )
    {
        if ( use_phys )
        {
            munmap( buf, wantbytes );
        }
        else
        {
            free( buf );
        }
    }

    if ( ( tests_set & M_TEST_SET_KERNEL) && !time_is_up )
    {
        if ( !( status && break_when_error) )
        {
            status |= run_kernel_tests( wantbytes, thread_count , assist_count , loops );
        }
    }


    timestamp_print_time(stdout, thread_id);
    printf("Done.%d\n", status);

    exit_test();
    if ( g_log_buffered )
    {
        log_pool_destroy();
    }

    if (0 == status)
    {
        status = status_userspace;
    }

    exit(status);

}

void *run_tests( const void *args )
{
    int thread_id = ((t_tests_para*)args)->thread_id;
    ul  loops = ((t_tests_para*)args)->loops;
    ul  testmask = ((t_tests_para*)args)->testmask;
    int break_when_error = ((t_tests_para*)args)->break_when_error;
    void volatile *aligned = bufpool[thread_id].buf;
    size_t bufsize = bufpool[thread_id].bufsize;
    size_t halflen;
    size_t count;
    ulv *bufa, *bufb;
    ul loop, i;
    unsigned int cpu_set;
    int pos;
    char *cur_log_buf = 0;
    char log_line[256];

    DBG("this is thread %d\n", thread_id );

    pos = ( 1 + thread_id ) % cpu_count;
    cpu_set = 1ul << pos;/*lint !e620*/
    run_in_cpu( cpu_set );

    halflen = bufsize / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);

    if ( g_log_buffered )
    {
        cur_log_buf = log_buffer_get( thread_id );

        if ( !cur_log_buf )
        {
            log_put( 0 , 0, 1, "[%d]No buffer for log\n", thread_id );
            g_log_buffered = 0;
        }
    }

    DBG("loops is %d,jump_freq_exit is %d\n", loops, jump_freq_exit);
    for (loop = 1; ((!loops) || loop <= loops); loop++)
    {
        if ( jump_freq_exit )
        {
            break ;
        }

        DBG("enter for cycle.\n");
        timestamp_print_time_buf(cur_log_buf, thread_id);

        log_put( g_log_buffered, cur_log_buf, 1, "[%d]Loop %lu", thread_id, loop );

        if (loops)
        {
            log_put( g_log_buffered, cur_log_buf, 0, "/%lu", loops);
        }

        log_put( g_log_buffered, cur_log_buf, 0, ":\n");

        show_current_freq_buf( cur_log_buf , thread_id );

        //log_put( g_log_buffered, cur_log_buf, 1, "[%d]  %-20s: ",thread_id, "Stuck Address");

        DBG("test_stuck_address enter.\n");
        if (!test_stuck_address(aligned, bufsize / sizeof(ul), cur_log_buf, thread_id))
        {
            log_put( g_log_buffered, cur_log_buf, 0 , "ok\n");
        }
        else
        {
            exit_code[thread_id] |= EXIT_FAIL_ADDRESSLINES;
            if ( 1 == break_when_error)
            {
                break ;
            }
        }
        for (i = 0;;i++)
        {

            if (!tests[i].name)
            {
                break;
            }
            DBG("test is %s.\n", tests[i].name);
            /* If using a custom testmask, only run this test if the
               bit corresponding to this test was set by the user.
             */
            if (testmask && (!((1ul << i) & testmask))) /*lint !e620*/
            {
                continue;
            }

            //log_put( g_log_buffered, cur_log_buf, 1, "[%d]  %-20s:",thread_id, tests[i].name);

            if (!tests[i].fp(bufa, bufb, count, cur_log_buf, log_line, thread_id))
            {
                log_put( g_log_buffered, cur_log_buf, 0, "ok\n");
                if ( tests_broken )
                {
                    break;
                }
            }
            else
            {
                exit_code[thread_id] |= EXIT_FAIL_OTHERTEST;
                if ( 1 == break_when_error)
                {
                    tests_broken = 1;
                    break ;
                }
            }
        }

        log_put( g_log_buffered, cur_log_buf, 0, "\n");

        log_flush_buf( g_log_buffered, thread_id );

        if ( tests_broken )
        {
            break;
        }

        if ( 1 == break_when_error && 0 != exit_code[thread_id] )
        {
            break ;
        }

    }

    log_put( g_log_buffered, cur_log_buf, 1, "[%d] exit test:%d\n", thread_id, exit_code[thread_id]);
    timestamp_print_time_buf(cur_log_buf, thread_id);
    log_flush_buf( g_log_buffered, thread_id );

    return NULL;
}

int run_fix_freq_thread( void )
{
    int err;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* create a thread to adjust frequencies */
    err = pthread_create(&fix_tid, &attr, fixed_freq, (void *) & g_fix_freq);
    if (err)
    {
        perror("fail to create thread.");
        exit( EXIT_FAIL_NONSTARTER );
    }
    pthread_attr_destroy(&attr);

    return 0;
}

int run_auto_freq_thread( void)
{
    int err;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* create a thread to do auto frequencies adjust */
    err = pthread_create(&auto_tid, &attr, auto_freq_jump, NULL);
    if (err)
    {
        perror("fail to create thread.");
        exit(EXIT_FAIL_NONSTARTER);
    }
    pthread_attr_destroy(&attr);

    return 0;
}

int run_tests_thread( int thread_cnt, ul  loops, ul  testmask, int break_when_error )
{
    int ret = 0;
    int err;
    int thread_id;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    DBG("run_tests_thread enter, this is thread is %d\n", thread_cnt);
    for ( thread_id = 0 ; thread_id < thread_cnt; thread_id++ )
    {
        tests_para[thread_id].thread_id = thread_id;
        tests_para[thread_id].loops = loops;
        tests_para[thread_id].testmask = testmask;
        tests_para[thread_id].break_when_error = break_when_error;

        err = pthread_create(&run_tids[thread_id], &attr, run_tests, &tests_para[thread_id]);
        if (err)
        {
            perror("fail to create thread.");
            ret = EXIT_FAIL_NONSTARTER;
        }
    }
    pthread_attr_destroy(&attr);

    return ret;
}

int wait_thread_finished( int thread_cnt )
{
    int thread_id;
    int ret = 0;
    for ( thread_id = 0 ; thread_id < thread_cnt; thread_id++ )
    {
        pthread_join( run_tids[thread_id] , NULL );
        DBG( "wait_thread_finished thread[%d] joined\n", thread_id );
        ret |= exit_code[thread_id];
    }
    tests_finished = 1;
    return ret;
}


void bufpool_init( void volatile *buf, size_t bufsize, size_t thread_cnt, size_t pagesize )
{
    int thread_id;
    size_t per_page_count, page_count;
    ulv *next_start;
    size_t per_buf_size;

    page_count = ( bufsize + pagesize - 1 ) / pagesize;
    per_page_count = ( page_count + thread_cnt - 1 ) / thread_cnt;
    per_buf_size = pagesize * per_page_count;
    next_start = buf;
    for ( thread_id = 0 ; thread_id < (int)(thread_cnt - 1); thread_id++ )
    {
        bufpool[thread_id].bufsize = per_buf_size;
        bufpool[thread_id].buf = next_start;
        next_start = (void *)( (size_t)bufpool[thread_id].buf + per_buf_size);
        DBG("bufpool_init thread:%d, buf:%lx, size:%lx\n", thread_id,
            (size_t)bufpool[thread_id].buf, bufpool[thread_id].bufsize );
    }
    bufpool[thread_id].bufsize = (size_t)buf + bufsize - (size_t)next_start;
    bufpool[thread_id].buf = (void volatile *)next_start;
    DBG("bufpool_init thread:%d, buf:%lx, size:%lx\n", thread_id,
        (size_t)bufpool[thread_id].buf, bufpool[thread_id].bufsize );
}

void sigalarm_fn(int sig)
{
    time_is_up = 1;

    printf(" times is up %d\n", sig);
}

void *assist_thread_func( const void *args)
{
    int thread_id = *((int*)args);
    size_t pagesize = (size_t)memtester_pagesize();
    size_t pagesizemask = ~(pagesize - 1);
    ulv *bufa, *bufb;
    void volatile *buf, *aligned;
    size_t halflen, count, bufsize;
    int pos;
    unsigned int cpu_set;

    DBG("assistant_thread_func enter %d\n", thread_id);

    pos = ( 1 + thread_id ) % cpu_count;
    cpu_set = 1ul << pos;/*lint !e620*/
    run_in_cpu( cpu_set );

    bufsize = 512 * 1024;
    buf = (void volatile *) malloc(bufsize);
    if ( !buf )
    {
        return NULL;
    }

    if ((size_t) buf % pagesize)
    {
        aligned = (ulv volatile *) ((size_t) buf & pagesizemask) + pagesize;
        bufsize -= ((size_t) aligned - (size_t) buf);
    }
    else
    {
        aligned = buf;
    }
    halflen = bufsize / 2;
    count = halflen / sizeof(ul);
    bufa = (ulv *) aligned;
    bufb = (ulv *) ((size_t) aligned + halflen);

    while ( (!jump_freq_exit) && (!tests_finished) )
    {
        test_assist(bufa, bufb, count);
    }

    DBG("assistant_thread_func exit\n");

    free( buf );

    return NULL;
}

int run_assist_thread(  int thread_cnt )
{
    int err;
    int thread_id;

    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for ( thread_id = 0 ; thread_id < thread_cnt; thread_id++ )
    {
        assist_para[thread_id] = thread_id;
        err = pthread_create(&assist_tids[thread_id], &attr, assist_thread_func, &assist_para[thread_id]);
        if (err)
        {
            perror("fail to create thread.");
            exit(EXIT_FAIL_NONSTARTER);
        }
    }

    pthread_attr_destroy(&attr);

    return 0;
}

int wait_assist_finished( int thread_cnt )
{
    int thread_id;
    for ( thread_id = 0 ; thread_id < thread_cnt; thread_id++ )
    {
        pthread_join( assist_tids[thread_id] , NULL );
        DBG("wait_assist_finished %d", thread_id );
    }

    return 0;
}


int run_userspace_tests( int thread_cnt, int assist_cnt, ul loops, ul testmask, int break_when_error )
{
    int status;

    cur_test_case = M_TEST_SET_USER;


    if ( is_auto_freq )
    {
        run_auto_freq_thread();
    }
    else if ( is_fix_freq )
    {
        run_fix_freq_thread();
    }

    if ( have_assist )
    {
        run_assist_thread( assist_cnt );
    }

    run_tests_thread( thread_cnt, loops, testmask, break_when_error );

    status = wait_thread_finished( thread_cnt );

    if ( have_assist )
    {
        wait_assist_finished( assist_cnt );
    }

    if ( fix_tid )
    {
        pthread_join( fix_tid, NULL );
    }

    if ( auto_tid )
    {
        pthread_join( auto_tid, NULL );
    }

    return status;
}

size_t get_tests_count( void )
{
    return sizeof(tests) / sizeof(struct test);
}


static int insmod(const char *filename, const char *args, const char * tag)
{
    void *module;
    unsigned int size;
    int ret;

    tag = tag;

    module = (void*)load_file(filename, &size);

    if (!module)
    {
        printf("Cannot load file: %s,%s\n", filename, tag);
        return -1;
    }

    errno = 0 ;
    ret = init_module(module, size, args);

    if ( ret )
    {
        printf("init_module (%s:%u) failed:%d, err:%s\n", filename, size, ret, strerror(errno) );
    }

    free(module);

    return ret;
}

static int rmmod(const char *modname)
{
    int ret = -1;
    int maxtry = 10;

    while (maxtry-- > 0)
    {
        ret = delete_module(modname, O_NONBLOCK | O_EXCL);
        if (ret < 0 && errno == EAGAIN)
        {
            usleep(500000);
        }
        else
        {
            break;
        }
    }

    if (ret != 0)
    {
        printf("Unable to unload driver module \"%s\": %s\n",
               modname, strerror(errno));
    }

    return ret;
}

int run_kernel_tests( size_t buf_size, int thread_cnt, int assist_cnt, ul  loops)
{
    char buf[128];
    char str[8] = {0};
    int ret = 0;
    int state = 0;

    log_put( 0 , 0, 1, "Enter run_kernel_tests\n");
    log_flush_buf( 0, -1 );

    cur_test_case = M_TEST_SET_KERNEL;

    tests_finished = 0;

    if (is_auto_freq)
    {
        run_auto_freq_thread();
    }
    else if ( is_fix_freq )
    {
        run_fix_freq_thread();
    }

    DBG("Enter run_kernel_tests\n");
    insmod("/system/bin/ddrtest.ko", "", "ddrtest");

    sprintf( buf, "0x%lx", buf_size );
    write_string_to_file( buf, "/sys/ddr_tester/ram_size");

    sprintf( buf, "%d", thread_cnt );
    write_string_to_file( buf, "/sys/ddr_tester/test_threads");

    sprintf( buf, "%d", assist_cnt );
    write_string_to_file( buf, "/sys/ddr_tester/assist_threads");

    sprintf( buf, "%lu", loops );
    write_string_to_file( buf, "/sys/ddr_tester/loops");

    sprintf( buf, "1" );
    write_string_to_file( buf, "/sys/ddr_tester/state");

//add by ouyangxun for the state updae to 1 slow
    sleep(3);
    do
    {
        ret = read_string_from_file( "/sys/ddr_tester/state", str, 2 );
        if ( ret > 0 )
        {
            state = atoi( str );
        }
        else
        {
            printf("Read state failed\n");
            break;
        }

        print_kernel_progress();

        sleep(1);
    }while( state );    

    tests_finished = 1;

    if ( auto_tid )
    {
        pthread_join( auto_tid, NULL );
    }

    read_string_from_file( "/sys/ddr_tester/status", str, 2 );
    ret = atoi( str );

    log_put( 0 , 0, 1, "Exit run_kernel_tests:%d\n", ret);
    log_flush_buf( 0, -1 );

    DBG("rmmod ddrtest");
    rmmod("ddrtest");

    return ret;
}

void print_kernel_progress( void )
{
    char buf[128];
    static char progress[128] = {0};

    memset( buf, 0, sizeof( buf ) );
    read_string_from_file( "/sys/ddr_tester/progress", buf, sizeof(buf) );
    if ( strcmp( buf, progress ) != 0 )
    {
        timestamp_print_time_mini(0, -1);
        log_put( 0 , 0, 0, " Test %s is running\n", buf );
        DBG("Test %s is running\n", buf);
        memset( progress, 0, sizeof( progress ) );
        sprintf( progress, "%s", buf );
    }
}

/* DTS2013112300820 xiaohui 20131009 end > */
/* DTS2013093005579 xiaohui 20131009 end > */

