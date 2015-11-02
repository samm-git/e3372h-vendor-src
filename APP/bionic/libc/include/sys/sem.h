#ifndef _SYS_SEM_H
#define _SYS_SEM_H	1


#include <stddef.h>
#include <sys/ipc.h>
#include <linux/sem.h>



__BEGIN_DECLS

/* Semaphore control operation.  */
extern int semctl (int __semid, int __semnum, int __cmd, ...);

/* Get semaphore.  */
extern int semget (key_t __key, int __nsems, int __semflg);

/* Operate on semaphore.  */
extern int semop (int __semid, struct sembuf *__sops, size_t __nsops);

/* Operate on semaphore with timeout.  */
extern int semtimedop (int __semid, struct sembuf *__sops, size_t __nsops,
		       const struct timespec *__timeout);


__END_DECLS

#endif /* sys/sem.h */
