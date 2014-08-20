/* Dump registers.
   Copyright (C) 2000-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Jaeger <aj@suse.de>, 2000.

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

/* Copyright (c) 2012 Panasonic Mobile Communications Co., Ltd.
   2012-05-16 Modified for MIPS64 */


#include <sys/uio.h>
#include <_itoa.h>

/* We will print the register dump in this format:

[_ABIO32]
 R0   XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
 R8   XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
 R16  XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
 R24  XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
            pc       lo       hi
      XXXXXXXX XXXXXXXX XXXXXXXX

[else]
 R0   XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R4   XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R8   XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R12  XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R16  XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R20  XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 R24  XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
                    pc               lo               hi
      XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXX
 The FPU registers will not be printed.
*/

static void
hexvalue (unsigned long int value, char *buf, size_t len)
{
  char *cp = _itoa_word (value, buf + len, 16, 0);
  while (cp > buf)
    *--cp = '0';
}

#if _MIPS_SIM == _ABIO32
static void
register_dump (int fd, struct sigcontext *ctx)
{
  char regs[38][8];
  struct iovec iov[38 * 2 + 10];
  size_t nr = 0;
  int i;

#define ADD_STRING(str) \
  iov[nr].iov_base = (char *) str;					      \
  iov[nr].iov_len = strlen (str);					      \
  ++nr
#define ADD_MEM(str, len) \
  iov[nr].iov_base = str;						      \
  iov[nr].iov_len = len;						      \
  ++nr

  /* Generate strings of register contents.  */
  for (i = 0; i < 32; i++)
    hexvalue (ctx->sc_regs[i], regs[i], 8);
  hexvalue (ctx->sc_pc, regs[32], 8);
  hexvalue (ctx->sc_mdhi, regs[33], 8);
  hexvalue (ctx->sc_mdlo, regs[34], 8);

  /* Generate the output.  */
  ADD_STRING ("Register dump:\n\n R0   ");
  for (i = 0; i < 8; i++)
    {
      ADD_MEM (regs[i], 8);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R8   ");
  for (i = 8; i < 16; i++)
    {
      ADD_MEM (regs[i], 8);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R16  ");
  for (i = 16; i < 24; i++)
    {
      ADD_MEM (regs[i], 8);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R24  ");
  for (i = 24; i < 32; i++)
    {
      ADD_MEM (regs[i], 8);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n            pc       lo       hi\n      ");
  for (i = 32; i < 35; i++)
    {
      ADD_MEM (regs[i], 8);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n");

  /* Write the stuff out.  */
  writev (fd, iov, nr);
}
#else
static void
register_dump (int fd, ucontext_t *ctx)
{
  char regs[38][16];
  struct iovec iov[38 * 2 + 10];
  size_t nr = 0;
  int i;
  mcontext_t *reg = (mcontext_t *)&(ctx->uc_mcontext);


#define ADD_STRING(str) \
  iov[nr].iov_base = (char *) str;					      \
  iov[nr].iov_len = strlen (str);					      \
  ++nr
#define ADD_MEM(str, len) \
  iov[nr].iov_base = str;						      \
  iov[nr].iov_len = len;						      \
  ++nr

  /* Generate strings of register contents.  */
  for (i = 0; i < 32; i++)
    hexvalue (reg->gregs[i], regs[i], 16);
  hexvalue (reg->pc, regs[32], 16);
  hexvalue (reg->mdlo, regs[33], 16);
  hexvalue (reg->mdhi, regs[34], 16);

  /* Generate the output.  */
  ADD_STRING ("Register dump:\n\n R0   ");
  for (i = 0; i < 4; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R4   ");
  for (i = 4; i < 8; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R8   ");
  for (i = 8; i < 12; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R12  ");
  for (i = 12; i < 16; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R16  ");
  for (i = 16; i < 20; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R20  ");
  for (i = 20; i < 24; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R24  ");
  for (i = 24; i < 28; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n R28  ");
  for (i = 28; i < 32; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n                    pc               lo               hi\n      ");
  for (i = 32; i < 35; i++)
    {
      ADD_MEM (regs[i], 16);
      ADD_STRING (" ");
    }
  ADD_STRING ("\n");

  /* Write the stuff out.  */
  writev (fd, iov, nr);
}
#endif

#define REGISTER_DUMP register_dump (fd, ctx)
