/** zsparsell hexcode.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "hexcode.hpp"

namespace zsparsell {

// disable warning about case X ... Y
#pragma GCC diagnostic ignored "-Wpedantic"
bool decode_hexnum(const char what, uint8_t &ret) noexcept {
  switch(what) {
    case '0' ... '9':
      // as usual
      ret = what - '0';
      break;

    case 'A' ... 'A':
      ret = 10 + (what - 'A');
      break;

    case 'a' ... 'f':
      ret = 10 + (what - 'a');
      break;

    default:
      ret = 0;
      return false;
  }

  return true;
}

}
