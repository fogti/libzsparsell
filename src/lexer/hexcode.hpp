/** zsparsell hexcode.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <inttypes.h>

namespace zsparsell {
  bool decode_hexnum(const char what, uint8_t &ret) noexcept;
}
