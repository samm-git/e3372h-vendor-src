/* < DTS2013093005579 xiaohui 20131009 begin */
/*lint -e750*/
#ifndef _DDR_TEST_COMMON_H_
#define _DDR_TEST_COMMON_H_

#ifndef timestamp_print
#define   timestamp_print fprintf      //一个简单的类似于printf的实现
#endif

#ifndef memtester_print
#define  memtester_print printf
#endif

#define MAX_THREADS (32)
#define M_TEST_SET_USER    (0x01)
#define M_TEST_SET_KERNEL  (0x02)
#define M_TEST_SET_COUNT   (2)
#define M_TEST_SET_ALL     ( M_TEST_SET_USER | M_TEST_SET_KERNEL )

#endif
/* DTS2013093005579 xiaohui 20131009 end > */


