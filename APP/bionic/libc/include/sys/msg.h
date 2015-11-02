#ifndef _SYS_MSG_H
#define _SYS_MSG_H


#include <stddef.h>
#include <sys/ipc.h>
#include <linux/msg.h>


__BEGIN_DECLS


extern int msgctl (int __msqid, int __cmd, struct msqid_ds *__buf);


extern int msgget (key_t __key, int __msgflg);


extern ssize_t msgrcv (int __msqid, void *__msgp, size_t __msgsz,
		       long int __msgtyp, int __msgflg);

extern int msgsnd (int __msqid, const void *__msgp, size_t __msgsz,
		   int __msgflg);

__END_DECLS

#endif /* sys/msg.h */
