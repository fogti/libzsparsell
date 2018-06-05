// mman.c - win32 mmap implementation
#include <windows.h>
#include <errno.h>
#include <io.h>
#include "mman.h"

#ifndef FILE_MAP_EXECUTE
#define FILE_MAP_EXECUTE    0x0020
#endif /* FILE_MAP_EXECUTE */

static void __map_mman_error() {
  // TODO: implement
  errno = (DWORD) GetLastError();
}

static DWORD __map_mmap_prot_page(const int prot) {
  DWORD protect = 0;

  if(prot != PROT_NONE)
    protect = (
      (prot & PROT_EXEC)
      ? ((prot & PROT_WRITE) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ)
      : ((prot & PROT_WRITE) ? PAGE_READWRITE         : PAGE_READONLY)
    );

  return protect;
}

static DWORD __map_mmap_prot_file(const int prot) {
  DWORD desiredAccess = 0;

  if(prot == PROT_NONE)
    return desiredAccess;

  if(prot & PROT_READ)
    desiredAccess |= FILE_MAP_READ;
  if(prot & PROT_WRITE)
    desiredAccess |= FILE_MAP_WRITE;
  if(prot & PROT_EXEC)
    desiredAccess |= FILE_MAP_EXECUTE;

  return desiredAccess;
}

void* mmap(void *addr, const size_t len, const int prot, const int flags, const int fildes, const off_t off) {
  HANDLE fm, h;

  void * map = MAP_FAILED;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4293)
#endif

  const DWORD dwFileOffsetLow = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)off : (DWORD)(off & 0xFFFFFFFFL);
  const DWORD dwFileOffsetHigh = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)0 : (DWORD)((off >> 32) & 0xFFFFFFFFL);
  const DWORD protect = __map_mmap_prot_page(prot);
  const DWORD desiredAccess = __map_mmap_prot_file(prot);

  const off_t maxSize = off + (off_t)len;

  const DWORD dwMaxSizeLow = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)maxSize : (DWORD)(maxSize & 0xFFFFFFFFL);
  const DWORD dwMaxSizeHigh = (sizeof(off_t) <= sizeof(DWORD)) ?
                    (DWORD)0 : (DWORD)((maxSize >> 32) & 0xFFFFFFFFL);

#ifdef _MSC_VER
#pragma warning(pop)
#endif

  errno = 0;

  if(len == 0
     /* Unsupported flag combinations */
     || (flags & MAP_FIXED) != 0
     /* Usupported protection combinations */
     || prot == PROT_EXEC)
  {
    errno = EINVAL;
    return MAP_FAILED;
  }

  h = ((flags & MAP_ANONYMOUS) == 0) ?
        (HANDLE)_get_osfhandle(fildes) : INVALID_HANDLE_VALUE;

  if((flags & MAP_ANONYMOUS) == 0 && h == INVALID_HANDLE_VALUE) {
    errno = EBADF;
    return MAP_FAILED;
  }

  fm = CreateFileMapping(h, NULL, protect, dwMaxSizeHigh, dwMaxSizeLow, NULL);

  if(!fm) {
    __map_mman_error();
    return MAP_FAILED;
  }

  map = MapViewOfFile(fm, desiredAccess, dwFileOffsetHigh, dwFileOffsetLow, len);

  CloseHandle(fm);

  if(!map) {
    __map_mman_error();
    return MAP_FAILED;
  }

  return map;
}

int munmap(void *addr, const size_t len) {
  if(UnmapViewOfFile(addr))
    return 0;

  __map_mman_error();

  return -1;
}
