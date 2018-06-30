// source: https://github.com/zserik/zxtw/src/escape.cxx

namespace zsparsell {
  char escape(char x) noexcept;
  char unescape(char x) noexcept;

  char escape(const char x) noexcept {
    switch(x) {
      case '\a': return 'a';
      case '\b': return 'b';
      case '\033': return 'e';
      case '\f': return 'f';
      case '\n': return 'n';
      case '\r': return 'r';
      case '\t': return 't';
      case '\v': return 'v';
      case '"':  return '"';
      case '\\': return '\\';
      default:   return 0;
    }
  }

  char unescape(const char x) noexcept {
    switch(x) {
      case 'a': return '\a';
      case 'b': return '\b';
      case 'e': return '\033';
      case 'f': return '\f';
      case 'n': return '\n';
      case 'r': return '\r';
      case 't': return '\t';
      case 'v': return '\v';
      // '"' and '\\' are handled implicit
      default:  return x;
    }
  }
}
