/* libzsparsell/lowlevel/mman.h - a simple mman.h wrapper */
#pragma once
#include "config.h"

#ifdef WIN32
/* mman-win32 - see src/lowlevel/win32_mman.c */
# ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#  define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
# endif

/* All the headers include this file. */
# ifndef _MSC_VER
#  include <_mingw.h>
# endif

# include <sys/types.h>

# define S_IRWXG (S_IRWXU >> 3)
# define S_IRWXO (S_IRWXG >> 3)

# ifdef __cplusplus
extern "C" {
# endif

# define PROT_NONE       0
# define PROT_READ       1
# define PROT_WRITE      2
# define PROT_EXEC       4

# define MAP_FILE        0
# define MAP_SHARED      1
# define MAP_PRIVATE     2
# define MAP_TYPE        0xf
# define MAP_FIXED       0x10
# define MAP_ANONYMOUS   0x20
# define MAP_ANON        MAP_ANONYMOUS

# define MAP_FAILED      ((void *)-1)

void*   mmap(void *addr, const size_t len, const int prot, const int flags, const int fildes, const off_t off);
int     munmap(void *addr, const size_t len);

# ifdef __cplusplus
};
# endif
/* end of win32 mmap */
# define madvise(A,L,V)
# define MADV_SEQUENTIAL
#else
# include <sys/mman.h>
#endif
