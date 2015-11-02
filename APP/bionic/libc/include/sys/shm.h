#ifndef _SYS_SHM_H
#define _SYS_SHM_H	1


#include <stddef.h>
#include <sys/ipc.h>
#include <linux/shm.h>


__BEGIN_DECLS


/* Shared memory control operation.  */
extern int shmctl (int __shmid, int __cmd, struct shmid_ds *__buf);

/* Get shared memory segment.  */
extern int shmget (key_t __key, size_t __size, int __shmflg);

/* Attach shared memory segment.  */
extern void *shmat (int __shmid, const void *__shmaddr, int __shmflg);

/* Detach shared memory segment.  */
extern int shmdt (const void *__shmaddr);

__END_DECLS

#endif /* sys/shm.h */
