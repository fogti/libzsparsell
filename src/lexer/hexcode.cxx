/** zsparsell hexcode.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "hexcode.hpp"

namespace zsparsell {

// disable warning about case X ... Y
#pragma GCC diagnostic ignored "-Wpedantic"
bool decode_hexnum(const char what, uint8_t &ret) noexcept {
  char dif;
  switch(what) {
    case '0' ... '9':
      dif = '0'; break;

    case 'A' ... 'F':
      dif = '7'; break;

    case 'a' ... 'f':
      dif = 'W'; break;

    default:
      ret = 0;
      return false;
  }

  ret = what - dif;
  return true;
}

}
