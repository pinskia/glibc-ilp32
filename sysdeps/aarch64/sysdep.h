/* Copyright (C) 1997-2012 Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _AARCH64_SYSDEP_H
#define _AARCH64_SYSDEP_H

#include <sysdeps/generic/sysdep.h>

#ifdef __LP64__
#define AARCH64_R(NAME)		R_AARCH64_ ## NAME
#else
#define AARCH64_R(NAME)		R_AARCH64_P32_ ## NAME
#endif

#ifdef	__ASSEMBLER__

/* Help tackle the pointer size difference between ELF64 and ELF32.  */
#ifdef __LP64__
#define PTR_REG(n)	x##n
#define PTR_SIZE	8
#define PTR_LOG_SIZE	3
#else
#define PTR_REG(n)	w##n
#define PTR_SIZE	4
#define PTR_LOG_SIZE	2
#define AARCH64_R(NAME)		R_AARCH64_P32_ ## NAME
#endif

/* Syntactic details of assembler.  */

#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

/* Define an entry point visible from C.  */
#define ENTRY(name)						\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .align 4;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C.  */
#define ENTRY_ALIGN(name, align)				\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

/* Define an entry point visible from C with a specified alignment and
   pre-padding with NOPs.  This can be used to ensure that a critical
   loop within a function is cache line aligned.  Note this version
   does not adjust the padding if CALL_MCOUNT is defined. */

#define ENTRY_ALIGN_AND_PAD(name, align, padding)		\
  .globl C_SYMBOL_NAME(name);					\
  .type C_SYMBOL_NAME(name),%function;				\
  .p2align align;						\
  .rep padding;							\
  nop;								\
  .endr;							\
  C_LABEL(name)							\
  cfi_startproc;						\
  CALL_MCOUNT

#undef	END
#define END(name)						\
  cfi_endproc;							\
  ASM_SIZE_DIRECTIVE(name)

/* If compiled for profiling, call `mcount' at the start of each function.  */
#ifdef	PROF
# define CALL_MCOUNT						\
	str	x30, [sp, #-16]!;				\
	bl	mcount;						\
	ldr	x30, [sp], #16	;
#else
# define CALL_MCOUNT		/* Do nothing.  */
#endif

/* Local label name for asm code.  */
#ifndef L
# define L(name)         .L##name
#endif

/* Load or store to/from a pc-relative EXPR into/from R, using T.  */
#ifdef __LP64__
#define LDST_PCREL(OP, R, R32, T, EXPR)  \
	adrp	T, EXPR;    \
	OP	R, [T, #:lo12:EXPR];
#else
#define LDST_PCREL(OP,  R, R32, T, EXPR)  \
	adrp	T, EXPR;     \
	OP	R32, [T, #:lo12:EXPR];
#endif

/* Load or store to/from a got-relative EXPR into/from R, using T.  */
#ifdef __LP64__         
#define LDST_GLOBAL(OP, R, T, T32, EXPR) \
	adrp	T, :got:EXPR; \
	ldr	T, [T, #:got_lo12:EXPR];\
	OP	R, [T];
#else
#define LDST_GLOBAL(OP, R, T, T32, EXPR)     \
	adrp	T, :got:EXPR; \
	ldr	T32, [T, #:got_lo12:EXPR];\
	OP	R, [T];
#endif

/* Since C identifiers are not normally prefixed with an underscore
   on this system, the asm identifier `syscall_error' intrudes on the
   C name space.  Make sure we use an innocuous name.  */
#define syscall_error	__syscall_error
#define mcount		_mcount

#endif	/* __ASSEMBLER__ */


#endif
