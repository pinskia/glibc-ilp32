/* sendfile64 is alias of sendfile syscall.  */

/* The _NR_sendfile64 is not defined for n64 abi, calling sendfile64
   fails with "system call not defined" error message. Default it to
   sendfile(). Copied from sysdeps/unix/sysv/linux/wordsize-64 directory. */
