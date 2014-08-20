#include <stdarg.h>
#include <sys/sem.h>
#include <ipc_priv.h>
#include <sysdep.h>

/* Define a `union semun' that is for glibc here.  */
union semun
{
  int val;			/* value for SETVAL */
  struct semid_ds *buf;		/* buffer for IPC_STAT & IPC_SET */
  unsigned short int *array;	/* array for GETALL & SETALL */
  struct seminfo *__buf;	/* buffer for IPC_INFO */
};

union semun_kernel
{
  int val;			/* value for SETVAL */
  unsigned long long kern_ptr;
};

#include <bp-checks.h>
#include <bp-semctl.h>		/* definition of CHECK_SEMCTL needs union semum */

int __semctl (int semid, int semnum, int cmd, ...);

int
__semctl (int semid, int semnum, int cmd, ...)
{
  union semun arg;
  union semun_kernel arg_real;
  va_list ap;

  va_start (ap, cmd);

  /* Get the argument.  */
  arg = va_arg (ap, union semun);

  va_end (ap);

  arg_real.kern_ptr = 0;
  if (cmd == SETVAL)
    arg_real.val = arg.val;
  else
    arg_real.kern_ptr = (unsigned long long)(uintptr_t)arg.array;

  return INLINE_SYSCALL (semctl, 4, semid, semnum, cmd,
			 arg_real.kern_ptr);
}

#include <shlib-compat.h>
versioned_symbol (libc, __semctl, semctl, GLIBC_2_0);
