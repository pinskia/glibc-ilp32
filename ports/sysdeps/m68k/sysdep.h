/* Assembler macros for m68k.
   Copyright (C) 1998, 2003, 2010, 2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#include <sysdeps/generic/sysdep.h>

#ifdef __ASSEMBLER__

/* Syntactic details of assembler.  */

/* ELF uses byte-counts for .align, most others use log2 of count of bytes.  */
# define ALIGNARG(log2) 1<<log2
/* For ELF we need the `.type' directive to make shared libs work right.  */
# define ASM_TYPE_DIRECTIVE(name,typearg) .type name,typearg
# define ASM_SIZE_DIRECTIVE(name) .size name,.-name

/* Define an entry point visible from C.

   There is currently a bug in gdb which prevents us from specifying
   incomplete stabs information.  Fake some entries here which specify
   the current source file.  */
# define ENTRY(name)							      \
  .globl C_SYMBOL_NAME(name);						      \
  ASM_TYPE_DIRECTIVE (C_SYMBOL_NAME(name),@function);			      \
  .align ALIGNARG(2);							      \
  C_LABEL(name)								      \
  cfi_startproc;							      \
  CALL_MCOUNT

# undef END
# define END(name)							      \
  cfi_endproc;								      \
  ASM_SIZE_DIRECTIVE(name)


/* If compiled for profiling, call `_mcount' at the start of each function.  */
# ifdef	PROF
/* The mcount code relies on a normal frame pointer being on the stack
   to locate our caller, so push one just for its benefit.  */
#  define CALL_MCOUNT \
  move.l %fp, -(%sp);							      \
  cfi_adjust_cfa_offset (4);  cfi_rel_offset (%fp, 0);			      \
  move.l %sp, %fp;							      \
  jbsr JUMPTARGET (_mcount);						      \
  move.l (%sp)+, %fp;							      \
  cfi_adjust_cfa_offset (-4); cfi_restore (%fp);
# else
#  define CALL_MCOUNT		/* Do nothing.  */
# endif

# define PSEUDO(name, syscall_name, args)				      \
  .globl __syscall_error;						      \
  ENTRY (name)								      \
    DO_CALL (syscall_name, args);					      \
    jcc JUMPTARGET(__syscall_error)

# undef PSEUDO_END
# define PSEUDO_END(name)						      \
  END (name)

# undef JUMPTARGET
# ifdef PIC
#  define JUMPTARGET(name)	name##@PLTPC
# else
#  define JUMPTARGET(name)	name
# endif

#endif	/* __ASSEMBLER__ */
