ifeq ($(subdir),csu)
sysdep_routines      += __read_tp libc-__read_tp
static-only-routines += __read_tp
shared-only-routines += libc-__read_tp
endif

ifeq ($(subdir),elf)
sysdep_routines	     += dl-vdso
sysdep-rtld-routines += __read_tp
ifeq ($(build-shared),yes)
# This is needed for DSO loading from static binaries.
sysdep-dl-routines += dl-static
sysdep_routines += dl-static
sysdep-rtld-routines += dl-static
endif
endif

ifeq ($(subdir),misc)
sysdep_headers += sys/elf.h gnu/stubs.h
endif

ifeq ($(subdir),stdlib)
gen-as-const-headers += ucontext_i.sym
endif

abi-variants := lp64 lp64_be
abi-variants += ilp32 ilp32_be

# Get value of default-abi.
include $(common-objpfx)default-abi.make


ifeq (,$(filter $(default-abi),$(abi-variants)))
Unknown ABI, must be one of $(abi-variants)
endif

abi-lp64-options := -D__LP64__ -U__AARCH64EB__
abi-lp64-condition := __WORDSIZE == 64 && !defined __AARCH64EB__ 
abi-lp64-ld-soname := ld-linux-aarch64.so.1

abi-lp64_be-options := -D__LP64__ -D__AARCH64EB__
abi-lp64_be-condition := __WORDSIZE == 64 && defined __AARCH64EB__
abi-lp64_be-ld-soname := ld-linux-aarch64_be.so.1

abi-ilp32-options := -U__LP64__ -U__AARCH64EB__
abi-ilp32-condition := __WORDSIZE == 32 && !defined __AARCH64EB__
abi-ilp32-ld-soname := ld-linux-aarch64_ilp32.so.1

abi-ilp32_be-options := -U__LP64__ -D__AARCH64EB__
abi-ilp32_be-condition := __WORDSIZE == 32 && defined __AARCH64EB__
abi-ilp32_be-ld-soname := ld-linux-aarch64_be_ilp32.so.1
