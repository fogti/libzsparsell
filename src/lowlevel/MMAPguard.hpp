#pragma once
/** zsparsell MMAPguard.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include <stddef.h> // size_t

namespace zsparsell {
  class MMAPguard final {
    void * _addr;
    size_t _len;

   public:
    MMAPguard(): _addr(0), _len(0) { }
    MMAPguard(const MMAPguard &o) = delete;
    ~MMAPguard();

    bool assign(int fd, size_t len) noexcept;

    auto get() const noexcept { return static_cast<const char*>(_addr); }
    auto size() const noexcept { return _len; }
    auto begin() const noexcept { return get(); }
    auto end() const noexcept { return get() + _len; }

    void advise(int adv) const noexcept;
  };
}
