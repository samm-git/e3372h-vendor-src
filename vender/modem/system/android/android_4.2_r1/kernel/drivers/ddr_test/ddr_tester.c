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
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h>
#include <linux/kernel.h>

#include "ddr_alg.h"

#define KOBJ_ATTR_RO(_name)  static struct kobj_attribute _name##_attr = \
                    __ATTR_RO(_name)

#define KOBJ_ATTR_RW(_name)  static struct kobj_attribute _name##_attr = \
                    __ATTR(_name, 0644, _name##_show, _name##_store)

#define M_LOG_FILE "/cache/kernel_test.log"

#define M_ORDER (MAX_ORDER - 1)
#define M_BLOCK_SIZE ( PAGE_SIZE * ( 1<<M_ORDER))

#define M_BUFFER_MIN (16*1024*1024)//定义一个bank的大小，各平台不一样

#define M_LOG_MIN (M_BLOCK_SIZE)
#define M_LOG_BUF_MAX (10)

#define M_TEST_ALL 0xFFFFFFFF

#define M_ALLOC_DYNAMIC 0
#define M_ALLOC_PHISICAL 1

#define M_THREAD_MAX  (32)

struct ddr_tester_data
{
    struct kobject  *obj;
    struct task_struct *tsk[M_THREAD_MAX];
    struct task_struct *atsk[M_THREAD_MAX];
    int tsk_para[M_THREAD_MAX];
    int atsk_para[M_THREAD_MAX];
    int thread_state[M_THREAD_MAX];
    char *buffer;
    unsigned long current_algorithm;
    int state;
    int allocation_type;
    unsigned long ram_size;
    int test_threads;
    int assist_threads;
    char *log_buf;
    unsigned long log_pos;
    unsigned long loops;
    struct file  *filp;
    mm_segment_t oldfs;
    struct mutex log_lock;
    int status;
    int progress[M_THREAD_MAX];
    int blocks_count;
};

struct ddr_tester_data *test_data = NULL;
volatile int exiting_test = 0;
unsigned long alloc_blocks[1024];
char blocks_useinfo[1024];
///////////////// Test Algorithm //////////////////////////////////////

struct test tests[] =
{
    {"data_align", test_prbs_data_align_test_7},
    {"data_flip", test_prbs_data_flip_test_7},
    {"walking", test_walking_ones},
    {"variation_walking", test_variation_walking_ones},
    {"own_addr", test_own_addr},
    {"bit_equalizing", ddr_test_bit_equalizing},
    {"march_ic", ddr_test_march_ic},
    {"march_lr", ddr_test_march_lr},
    {"march_raw", ddr_test_march_raw},
    {"march_sl", ddr_test_march_sl},
    {"march_sr", ddr_test_march_sr},
    {"march_ss", ddr_test_march_ss},
    {"march_g", ddr_test_march_g},
    { NULL, NULL }
};

struct test *get_ddr_tests(void)
{
    return tests;
}

///////////////////////////////////////////////////////
/*****************************************
  @func_name:  flush_data_to_file
  @para: char* filename
  @func: write the test data to the file
  @return  int  success:0,fail:-1
******************************************/
static int flush_data_to_file(void)
{
    uint16_t length;
    struct file  *filp = test_data->filp;

    length = strlen(test_data->log_buf) + 1 ;
    printk("%s: write_data_to_file: length %d\n", __FUNCTION__, length);

    /* write data */
    if (filp->f_op->write(filp, test_data->log_buf, length, &filp->f_pos) != length)
    {
        printk("%s: file write error\n", __FUNCTION__);
        filp_close(filp, NULL);
        set_fs(test_data->oldfs);
        return -1;
    }

    return 0;
}


int put_log( const char *fmt, ...)
{
    va_list args;
    int r;
    char *buf;
    int pos_offset;
    char line[128] = {0};

    mutex_lock(&test_data->log_lock);
    va_start(args, fmt);
    r = vsnprintf( line, INT_MAX, fmt, args );
    pos_offset = test_data->log_pos;
    buf = test_data->log_buf;

    if ( ( pos_offset + r ) > M_BLOCK_SIZE )
    {
        flush_data_to_file();
        pos_offset = 0;
        test_data->log_pos = 0;
    }
    else
    {
        sprintf( &buf[pos_offset] , "%s", line );
    }

    test_data->log_pos += r;
    test_data->log_buf[test_data->log_pos] = 0;
    va_end(args);
    mutex_unlock(&test_data->log_lock);


    return r;

}

static int occupy_buffer( unsigned long *blocks, int *count )
{
    char *buffer = NULL;
    gfp_t gfp_flags;
    unsigned long pa = 0;
    unsigned long last_pa = 0;
    unsigned long last_va = 0;
    int i = 0;
    int j = 0;


    gfp_flags = GFP_KERNEL | __GFP_ZERO;
    //__GFP_COMP |
    //__GFP_ZERO | __GFP_NOWARN | __GFP_NORETRY;

    while ( 1 )
    {
        buffer = (char *) __get_free_pages(gfp_flags, M_ORDER );

        if (!buffer )
        {
            printk(KERN_ERR "not enough mem,i=%d\n", i);
            put_log(KERN_ERR "not enough mem,i=%d\n", i);

            break;
        }

        alloc_blocks[i++] = (unsigned long)buffer;


        pa = __pa( buffer );
        if ( ( last_pa + M_BLOCK_SIZE ) != pa )
        {
            printk(KERN_ERR "allocate buffer:%lx->%lx, size:%lu ------last:%lx->%lx\n", (unsigned long)buffer, pa, M_BLOCK_SIZE, last_va, last_pa );
        }
        else
        {
            printk(KERN_ERR "allocate buffer:%lx->%lx, size:%lu\n", (unsigned long)buffer, pa, M_BLOCK_SIZE );
        }



        if (3 == j) break;//连续分配四次，每次大小2^M_ORDERpages

        last_pa = pa;
        last_va = (unsigned long)buffer;
        j++;


    }
    *count = i;

    return 0;
}

static void free_occupied_buffer( void )
{
    int count = test_data->blocks_count;
    int i;
    for ( i = 0; i < count; i++ )
    {
        if ( alloc_blocks[i] )
        {
            free_pages( alloc_blocks[i], M_ORDER );
            alloc_blocks[i] = 0;
        }
    }
    test_data->blocks_count = 0;
}

static void sort_buffer( unsigned long *blocks, int count )
{
    int i;
    int j = count - 1;
    unsigned long temp;

    for ( i = count - 1; i > 0; i-- )
    {
        for ( j = count - 1; j > 0; j-- )
        {
            if ( blocks[j] > blocks[j-1] )
            {
                temp = blocks[j];
                blocks[j] = blocks[j-1];
                blocks[j-1] = temp;
            }
        }
    }
}

static char *search_buffer( unsigned long *blocks, int count , int need )
{
    int i, j, k, get, ok = 0;

    for ( i = count - 1; i > 0; i = j )
    {
        for ( j = i - 1; j >= 0; j-- )
        {
            if ( ( blocks[j+1] + M_BLOCK_SIZE ) != blocks[j] )
            {
                break;
            }
        }
        if ( -1 == j )
        {
            j = 0;
        }


        get = i - j + 1;
        printk(KERN_ERR "search_buffer [%d]%lx->[%d]%lx\n", i, blocks[i], j, blocks[j] );
        put_log("search_buffer [%d]%lx->[%d]%lx\n", i, blocks[i], j, blocks[j] );
        if ( get >= need )
        {
            for ( k = j ; k <= i ; k++ )
            {
                blocks_useinfo[k] = 1;
            }

            ok = 1;
            break;
        }
    }
    if ( ok )
    {
        return (char *)blocks[i];
    }
    else
    {
        return 0;
    }
}

static void walk_buffer( char *buffer, int count )
{
    int i;
    for ( i = 0; i < count; i++ )
    {
        buffer[i*PAGE_SIZE] = 0;
    }

}

static char *tester_alloc_buffer( unsigned long *blocks, unsigned long size )
{
    int blocks_count, blocks_need;
    char *test_buf = NULL;

    blocks_need = ( size + M_BLOCK_SIZE - 1 ) / M_BLOCK_SIZE;

    occupy_buffer( blocks, &blocks_count );
    test_data->blocks_count = blocks_count;

    put_log(" M_BLOCK_SIZE = %lx\n", M_BLOCK_SIZE );
    put_log(" size = %lx\n", M_BLOCK_SIZE );
    put_log(" blocks_need = %d\n", blocks_need );
    put_log(" blocks_count = %d\n", blocks_count );

    if ( blocks_need > blocks_count )
    {
        return NULL;
    }

    sort_buffer( blocks, blocks_count );

    test_buf = search_buffer( blocks, blocks_count , blocks_need );
    if ( test_buf == NULL )
    {
        put_log(" search buffer failed\n" );
        return NULL;
    }

    walk_buffer(test_buf, blocks_need*(M_BLOCK_SIZE / PAGE_SIZE));

    return test_buf;
}

static void update_state( void )
{
    int i;
    int state = 0;

    for ( i = 0; i < test_data->test_threads; i++ )
    {
        if ( test_data->thread_state[i] )
        {
            state = 1;
            break;
        }
    }

    test_data->state = state;
}

static int run_tests_thread(void * para)
{
    struct test *available_test = get_ddr_tests();
    int i = 0;
    int thread_id = *(int*)para;
    unsigned long pages = test_data->ram_size / PAGE_SIZE;
    unsigned int per_pages = pages / test_data->test_threads;
    unsigned char *buffer_start_a = test_data->buffer + PAGE_SIZE * per_pages * thread_id;
    unsigned char *buffer_start_b = test_data->buffer + PAGE_SIZE * per_pages * thread_id + PAGE_SIZE * per_pages / 2;
    unsigned long count = per_pages * PAGE_SIZE / (2 * sizeof(ulv));
    unsigned long loops = test_data->loops;
    int forever;
    int status = 0;

    if ( 0 == loops )
    {
        forever = 1;
    }
    else
    {
        forever = 0;
    }

    printk(KERN_ERR " run_tests_thread %d,%d,%lx,size:%lx,pages:%lx.\n ", thread_id, forever, loops, test_data->ram_size, pages );
    test_data->thread_state[thread_id] = 1;
    set_current_state(TASK_INTERRUPTIBLE);
    while ( !kthread_should_stop() )
    {
        i = 0;

        put_log("[%d]One loops begin -------- \n", thread_id );
        while ( available_test[i].name )
        {
            if ( test_data->current_algorithm & ( 1 << i ) )
            {
                printk(KERN_ERR "[%d]Current is testing %s, start_a:%lx,start_b:%lx\n", thread_id, available_test[i].name,
                       (unsigned long)buffer_start_a, (unsigned long)buffer_start_b);
                test_data->progress[thread_id] = i;
                //put_log("[%d]Current is testing %s, start_a:%lx,start_b:%lx\n", thread_id, available_test[i].name,
                //                    (unsigned long)buffer_start_a, (unsigned long)buffer_start_b);
                status = available_test[i].fp((ulv*)buffer_start_a, (ulv*)buffer_start_b, (size_t)count);
                printk(KERN_ERR "test fun status:%d, exiting_test:%d\n", status, exiting_test);
                test_data->status |= status;
                if ( status  != 0 )
                {
                    printk(KERN_ERR " status fail exit \n" );
                    exiting_test = 1;
                }

                if ( exiting_test || kthread_should_stop() )
                {
                    printk(KERN_ERR " exit test\n" );
                    break;
                }
                schedule_timeout_interruptible(1);
            }
            i++;
        }
        if ( !forever )
        {
            loops--;
            if ( loops == 0 )
            {
                break;
            }
        }
        if ( exiting_test )
        {
            break;
        }

    }
    test_data->thread_state[thread_id] = 0;
    update_state();
    printk(KERN_ERR "Finished thread %d tests\n", thread_id );
    put_log( "Finished thread %d tests:%d\n", thread_id, status );
    if ( 0 == test_data->state)
    {
        printk(KERN_ERR "Finished all tests\n" );
        put_log("Finished all tests\n" );
        flush_data_to_file();
        free_occupied_buffer();
    }
    test_data->tsk[thread_id] = NULL;
    return 0;
}

static int run_assist_thread(void * para)
{
    char bufa[128];
    char bufb[128];
    int count = 128 / sizeof(ulv);
    int i;
    char temp, *p1, *p2;
    int thread_id = *(int*)para;

    set_current_state(TASK_INTERRUPTIBLE);

    while ( !kthread_should_stop() )
    {
        p1 = bufa;
        p2 = bufb;
        for (i = 0; i < count; i++)
        {
            if (exiting_test)
            {
                break;
            }

            temp = *p1;
            *p2++ = ~temp + i;
            temp = *p2;
            *p1++ = ~temp + i;
        }
        if ( exiting_test )
        {
            break;
        }
    }
    printk(KERN_ERR "Finished %d assist tests\n", thread_id );
    test_data->atsk[thread_id] = NULL;
    return 0;

}

int run_tests(void)
{
    int ret = 0;
    int i;
    int thread_cnt = test_data->test_threads;
    int assist_cnt = test_data->assist_threads;
    int page_cnt, per_page;

    memset( (void*)alloc_blocks , 0 , sizeof(alloc_blocks) );
    memset( (void*)blocks_useinfo, 0 , sizeof(blocks_useinfo) );

    page_cnt = (test_data->ram_size + PAGE_SIZE - 1) / PAGE_SIZE;
    per_page = (page_cnt + 2 * thread_cnt - 1) / (2 * thread_cnt);
    test_data->ram_size = per_page * 2 * thread_cnt * PAGE_SIZE;
    test_data->buffer = tester_alloc_buffer( alloc_blocks, test_data->ram_size );
    if ( !test_data->buffer )
    {
        printk(KERN_ERR "No buffer %lx\n", test_data->ram_size );
        put_log(" No buffer %lx\n", test_data->ram_size );
        test_data->state = 0;
        return -1;
    }

    for ( i = 0 ; i < thread_cnt; i++ )
    {
        printk(KERN_ERR " create thread %d, %d", i, nr_cpu_ids);
        test_data->tsk_para[i] = i;
        test_data->tsk[i] = kthread_create(run_tests_thread, (void*) & (test_data->tsk_para[i]), "run_tests_thread");
        if ( IS_ERR(test_data->tsk[i]) )
        {
            ret = -2;
            test_data->tsk[i] = NULL;
            test_data->state = 0;
            put_log("Create thread failed\n" );
        }
        else
        {
            wake_up_process(test_data->tsk[i]);
        }
    }

    for ( i = 0 ; i < assist_cnt; i++ )
    {
        printk(KERN_ERR " create assist %d", i);
        test_data->atsk_para[i] = i;
        test_data->atsk[i] = kthread_create(run_assist_thread, (void*) & (test_data->atsk_para[i]), "run_assist_thread");
        if ( IS_ERR(test_data->atsk[i]) )
        {
            test_data->atsk[i] = NULL;
            put_log(" Create assist thread failed\n" );
        }
        else
        {
            wake_up_process(test_data->atsk[i]);
        }
    }
    return ret;
}

int stop_tests(void)
{
    int ret = 0;
    int i;
    int thread_cnt = test_data->test_threads;
    int assist_cnt = test_data->assist_threads;

    if ( 0 == test_data->state )
    {
        return 0;
    }

    for ( i = 0; i < assist_cnt; i++ )
    {
        if ( test_data->atsk[i] )
        {
            kthread_stop( test_data->atsk[i] );
        }
    }

    for ( i = 0; i < thread_cnt; i++ )
    {
        if ( test_data->tsk[i] )
        {
            kthread_stop( test_data->tsk[i] );
        }
    }


    return ret;
}

static ssize_t available_algorithm_show(struct kobject *kobj,
                                        struct kobj_attribute *attr, char *buf)
{
    struct test *available_test = get_ddr_tests();
    int i = 0;
    int count = 0;
    char *dest = buf;

    while ( available_test[i].name )
    {
        count += snprintf(dest, 40, "[0x%02x]%s\n", 1 << i, available_test[i].name);
        dest = buf + count;
        i++;
    }

    return count;
}

static ssize_t current_algorithm_show(struct kobject *kobj,
                                      struct kobj_attribute *attr, char *buf)
{
    return snprintf(buf, 8, "0x%02lx\n", test_data->current_algorithm );
}

static ssize_t current_algorithm_store(struct kobject *kobj,
                                       struct kobj_attribute *attr,
                                       const char *buf, size_t count)
{
    test_data->current_algorithm = simple_strtoull( buf , NULL, 16 );
    return count;
}

static ssize_t state_show(struct kobject *kobj,
                          struct kobj_attribute *attr, char *buf)
{
    update_state();
    return snprintf(buf, 8, "%d\n", test_data->state );
}

static ssize_t state_store(
    struct kobject *kobj,
    struct kobj_attribute *attr,
    const char *buf, size_t count)
{
    int st;
    int ret;
    sscanf(buf, "%d", &st);

    printk("ddrteset st = %d.", st);
    if ( 1 == st && 0 == test_data->state )
    {
        test_data->state = 1;
        exiting_test = 0;
        ret = run_tests();
        if ( ret < 0 )
        {
            test_data->state = 0;
            free_occupied_buffer();
            flush_data_to_file();
        }
    }
    else if ( 0 == st && 1 == test_data->state )
    {
        exiting_test = 1;
        stop_tests();
        test_data->state = 0;
        free_occupied_buffer();
    }
    return count;
}

static ssize_t ram_size_show(struct kobject *kobj,
                             struct kobj_attribute *attr, char *buf)
{
    return snprintf( buf, 20, "0x%lx\n", test_data->ram_size );
}

static ssize_t ram_size_store(struct kobject *kobj,
                              struct kobj_attribute *attr,
                              const char *buf, size_t count)
{
    unsigned long st;
    sscanf(buf, "0x%lx", &st);

    if ( st >= 0 && st <= (1024*1024*1024) )
    {
        test_data->ram_size = st;
    }

    if ( test_data->ram_size < M_BUFFER_MIN )
    {
        test_data->ram_size = M_BUFFER_MIN;
    }


    return count;
}

static ssize_t test_threads_show(struct kobject *kobj,
                                 struct kobj_attribute *attr, char *buf)
{
    return snprintf( buf, 20, "%d\n", test_data->test_threads );
}

static ssize_t test_threads_store(struct kobject *kobj,
                                  struct kobj_attribute *attr,
                                  const char *buf, size_t count)
{
    int st;
    sscanf(buf, "%d", &st);

    if ( st >= 0 && st <= M_THREAD_MAX )
    {
        test_data->test_threads = st;
    }

    return count;
}

static ssize_t assist_threads_show(struct kobject *kobj,
                                   struct kobj_attribute *attr, char *buf)
{
    return snprintf( buf, 20, "%d\n", test_data->assist_threads );
}

static ssize_t assist_threads_store(struct kobject *kobj,
                                    struct kobj_attribute *attr,
                                    const char *buf, size_t count)
{
    int st;
    sscanf(buf, "%d", &st);

    if ( st >= 0 && st <= M_THREAD_MAX )
    {
        test_data->assist_threads = st;
    }

    return count;
}

static ssize_t loops_show(struct kobject *kobj,
                          struct kobj_attribute *attr, char *buf)
{
    return snprintf( buf, 20, "%lu\n", test_data->loops );
}

static ssize_t loops_store(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           const char *buf, size_t count)
{
    unsigned long st;
    sscanf(buf, "%lu", &st);

    test_data->loops = st;

    return count;
}

static ssize_t status_show(struct kobject *kobj,
                           struct kobj_attribute *attr, char *buf)
{
    return snprintf( buf, 20, "%d\n", test_data->status ? 1 : 0 );
}


static ssize_t progress_show(struct kobject *kobj,
                             struct kobj_attribute *attr, char *buf)
{
    int i;
    ssize_t cnt = 0;

    for ( i = 0; i < test_data->test_threads; i++ )
    {
        cnt += snprintf( buf + cnt, 20, "[%02d]%02d ", i, test_data->progress[i] );
    }

    return cnt;
}


KOBJ_ATTR_RO(available_algorithm);
KOBJ_ATTR_RW(current_algorithm);
KOBJ_ATTR_RW(state);
KOBJ_ATTR_RW(ram_size);
KOBJ_ATTR_RW(test_threads);
KOBJ_ATTR_RW(assist_threads);
KOBJ_ATTR_RW(loops);
KOBJ_ATTR_RO(status);
KOBJ_ATTR_RO(progress);


static struct attribute *ddr_tester_attrs[] =
{
    &available_algorithm_attr.attr,
    &current_algorithm_attr.attr,
    &state_attr.attr,
    &ram_size_attr.attr,
    &test_threads_attr.attr,
    &assist_threads_attr.attr,
    &loops_attr.attr,
    &status_attr.attr,
    &progress_attr.attr,
    NULL
};

static struct attribute_group ddr_tester_group =
{
    .attrs = ddr_tester_attrs,
         };


static int __init ddr_tester_init(void)
{
    int ret = 0;
    gfp_t gfp_flags;
    struct file *filp;

    test_data = kzalloc(sizeof(*test_data), GFP_KERNEL);
    if (!test_data)
    {
        ret = -ENOMEM;
        goto err_data;
    }

    gfp_flags = GFP_KERNEL | __GFP_COMP |
                __GFP_ZERO | __GFP_NOWARN | __GFP_NORETRY;

    printk(KERN_ERR "M_ORDER = %d\n", M_ORDER );
    //put_log(KERN_ERR "M_ORDER = %d\n", M_ORDER );


    test_data->log_buf = (char *) __get_free_pages(gfp_flags, M_ORDER );
    if (!test_data->log_buf)
    {
        printk(KERN_ERR "not enough mem for log\n");
        //put_log(KERN_ERR "not enough mem for log\n");

        ret = -ENOMEM;
        goto err_log;
    }
    test_data->log_pos = 0;
    test_data->current_algorithm = M_TEST_ALL;
    test_data->state = 0;
    test_data->allocation_type = M_ALLOC_DYNAMIC;
    test_data->ram_size = M_BUFFER_MIN;
    test_data->test_threads = 1;
    test_data->assist_threads = 0;
    test_data->buffer = NULL;
    test_data->status = 0;
    test_data->blocks_count = 0;
    mutex_init( &(test_data->log_lock) );
    test_data->oldfs = get_fs();
    set_fs(KERNEL_DS);
    filp = filp_open( M_LOG_FILE, O_RDWR | O_CREAT | O_APPEND, 0);
    if (IS_ERR(filp))
    {
        printk("%s: file %s filp_open error\n", __FUNCTION__, M_LOG_FILE);
        goto err_file1;
    }

    if (!filp->f_op)
    {
        printk("%s: File Operation Method Error\n", __FUNCTION__);
        goto err_file2;
    }
    test_data->filp = filp;

    //create kobject
    test_data->obj = kobject_create_and_add("ddr_tester", NULL);
    if (!test_data->obj)
    {
        ret = -ENOMEM;
        goto err_kobj;
    }

    ret = sysfs_create_group(test_data->obj, &ddr_tester_group);
    if (ret)
    {
        goto err_sysfs;
    }
    return 0;

err_sysfs:
    kobject_put(test_data->obj);
err_kobj:
    free_page((unsigned long)test_data->log_buf);
err_file2:
    filp_close(filp, NULL);
err_file1:
    set_fs(test_data->oldfs);
err_log:
    kfree(test_data);

err_data:
    return ret;
}

static void __exit ddr_tester_exit(void)
{
    if ( 1 == test_data->state )
    {
        stop_tests();
        free_occupied_buffer();
    }

    if ( test_data->log_buf )
    {
        free_page((unsigned long)test_data->log_buf);
    }
    if ( test_data->filp )
    {
        filp_close(test_data->filp, NULL);
    }

    set_fs(test_data->oldfs);
    sysfs_remove_group(test_data->obj, &ddr_tester_group);
    kobject_put( test_data->obj );
    kfree(test_data);
}

module_init(ddr_tester_init);
module_exit(ddr_tester_exit);

MODULE_DESCRIPTION("ddr tester");
MODULE_LICENSE("GPL");



