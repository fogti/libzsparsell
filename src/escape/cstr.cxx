#include "escape.hpp"

#include <stdlib.h>
#include <string.h>

using namespace std;

namespace zsparsell {
  typedef size_t (*escape_cstr_fnt)(const char *s, char *ret);
  static char * escape_wrap(const char * const s, bool x2, escape_cstr_fnt fn) noexcept {
    char * tmp = static_cast<char*>(calloc((1 + x2) * strlen(s) + 1, 1));
    if(!tmp) return nullptr;
    char * ret = static_cast<char*>(realloc(tmp, fn(s, tmp) + 1));
    return ret ? ret : tmp;
  }

  static size_t escape_intern(const char * const s, char * const ret) noexcept {
    size_t pos = 0;
    for(const char *i = s; *i; ++i) {
      char tmp = escape(*i);
      if(tmp) ret[pos++] = '\\';
      else    tmp = *i;
      ret[pos++] = tmp;
    }
    return pos;
  }

  static size_t unescape_intern(const char * const s, char * const ret) noexcept {
    size_t pos = 0;
    for(const char *i = s; *i; ++i) {
      char tmp = *i;
      if(*i == '\\') {
        if(!*(++i)) break;
        tmp = unescape(*i);
      }
      ret[pos++] = tmp;
    }
    return pos;
  }

  auto escape(const char * const s) noexcept -> char *
    { return escape_wrap(s, true, escape_intern); }

  auto unescape(const char * const s) noexcept -> char *
    { return escape_wrap(s, false, unescape_intern); }
}
