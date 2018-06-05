/** libzsparsell lowlevel/MMAPguard.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/

#include "mman.h"
#include "MMAPguard.hpp"

namespace zsparsell {
  MMAPguard::~MMAPguard() {
    if(_addr && _len) munmap(_addr, _len);
  }

  bool MMAPguard::assign(const int fd, const size_t len) noexcept {
    if(!len) return false;
    void *const addr = mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
    if(addr == reinterpret_cast<void*>(-1)) return false;
    if(_addr && _len) munmap(_addr, _len);
    _addr = addr; _len  = len;
    return true;
  }

  void MMAPguard::advise(const int adv) const noexcept {
    if(_addr && _len)
      madvise(_addr, _len, adv);
  }
}
