/* bits/typesizes.h -- underlying types for *_t.  Linux/AARCh64 version.
   Copyright (C) 2011-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Chris Metcalf <cmetcalf@tilera.com>, 2011.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_TYPES_H
# error "Never include <bits/typesizes.h> directly; use <sys/types.h> instead."
#endif

#ifndef	_BITS_TYPESIZES_H
#define	_BITS_TYPESIZES_H	1

/* See <bits/types.h> for the meaning of these macros.  This file exists so
   that <bits/types.h> need not vary across different GNU platforms.  */

/* ILP32 kernel interface is 64-bit.  */
#ifndef __LP64__
# define __SYSCALL_SLONG_TYPE	__SQUAD_TYPE
# define __SYSCALL_ULONG_TYPE	__UQUAD_TYPE
#else
# define __SYSCALL_SLONG_TYPE	__SLONGWORD_TYPE
# define __SYSCALL_ULONG_TYPE	__ULONGWORD_TYPE
#endif
#define __DEV_T_TYPE		__UQUAD_TYPE
#define __UID_T_TYPE		__U32_TYPE
#define __GID_T_TYPE		__U32_TYPE
#define __INO_T_TYPE		__SYSCALL_ULONG_TYPE
#define __INO64_T_TYPE		__UQUAD_TYPE
#define __MODE_T_TYPE		__U32_TYPE
#define __NLINK_T_TYPE		__U32_TYPE
#define __OFF_T_TYPE		__SYSCALL_SLONG_TYPE
#define __OFF64_T_TYPE		__SQUAD_TYPE
#define __PID_T_TYPE		__S32_TYPE
#define __RLIM_T_TYPE		__SYSCALL_ULONG_TYPE
#define __RLIM64_T_TYPE		__UQUAD_TYPE
#define __BLKCNT_T_TYPE		__SYSCALL_SLONG_TYPE
#define __BLKCNT64_T_TYPE	__SQUAD_TYPE
#define __FSBLKCNT_T_TYPE	__SYSCALL_ULONG_TYPE
#define __FSBLKCNT64_T_TYPE	__UQUAD_TYPE
#define __FSFILCNT_T_TYPE	__SYSCALL_ULONG_TYPE
#define __FSFILCNT64_T_TYPE	__UQUAD_TYPE
#define __FSWORD_T_TYPE		__SYSCALL_SLONG_TYPE
#define	__ID_T_TYPE		__U32_TYPE
#define __CLOCK_T_TYPE		__SYSCALL_SLONG_TYPE
#define __TIME_T_TYPE		__SYSCALL_SLONG_TYPE
#define __USECONDS_T_TYPE	__U32_TYPE
#define __SUSECONDS_T_TYPE	__SYSCALL_SLONG_TYPE
#define __DADDR_T_TYPE		__S32_TYPE
#define __SWBLK_T_TYPE		__SYSCALL_SLONG_TYPE
#define __KEY_T_TYPE		__S32_TYPE
#define __CLOCKID_T_TYPE	__S32_TYPE
#define __TIMER_T_TYPE		void *
#define __BLKSIZE_T_TYPE	__S32_TYPE
#define __FSID_T_TYPE		struct { int __val[2]; }
#define __SSIZE_T_TYPE		__SLONGWORD_TYPE

/* Number of descriptors that can fit in an `fd_set'.  */
#define	__FD_SETSIZE		1024
#define __FD_MASK_TYPE		__SYSCALL_SLONG_TYPE

/* Tell the libc code that off_t and off64_t are actually the same type
   for all ABI purposes, even if possibly expressed as different base types
   for C type-checking purposes.  */
#define __OFF_T_MATCHES_OFF64_T	1

/* Same for ino_t and ino64_t.  */
#define __INO_T_MATCHES_INO64_T	1

/* Same for __blkcnt_t and __blkcnt64_t.  */
#define __FSBLKCNT_T_TYPE_MATCHES_FSBLKCNT64_T_TYPE 1
#define __STATFS_MATCHES_STATFS64_TYPE 1


/* LP32 needs some padding in some cases */

#ifndef __LP64__

# undef __SIGINFO_INNER
# ifdef __AARCH64EB__
#  define __SIGINFO_INNER(type, field)		\
	__extension__ struct {			\
		int __pad_##field;		\
		type field;			\
	} __attribute__((aligned(8) ))
# else
#  define __SIGINFO_INNER(type, field)		\
	__extension__ struct {			\
		type field;			\
		int __pad_##field;		\
	} __attribute__((aligned(8) ))
# endif

# define __RUSAGE_LONG(__field)			\
		__SIGINFO_INNER(long, __field)

# define __SHMID_DS_SIZE_TYPE(__field)		\
		__SIGINFO_INNER(size_t, __field)

# undef __SIGINFO_VOIDPOINTER
# define __SIGINFO_VOIDPOINTER(field)		\
		__SIGINFO_INNER(void*, field)

# undef __SIGINFO_BAND
# define __SIGINFO_BAND(field)			\
	__SIGINFO_INNER(long, field)
#endif


#define __TIME_T_64_BITS

#define __SI_PAD_SIZE     ((__SI_MAX_SIZE / sizeof (int)) - 4)

#endif /* bits/typesizes.h */
