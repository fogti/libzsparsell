/** zsparsell hexcode.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "hexcode.hpp"
#include <ctype.h>

namespace zsparsell {

// disable warning about case X ... Y
#pragma GCC diagnostic ignored "-Wpedantic"
bool decode_hexnum(const char what, uint8_t &ret) noexcept {
  ret = 0;
  switch(const char tlw = tolower(what)) {
    case '0' ... '9':
      // as usual
      ret = what - '0';
      break;

    case 'a' ... 'f':
      ret = 10 + (tlw - 'a');
      break;

    default:
      return false;
  }

  return true;
}

}
