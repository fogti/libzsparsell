// source: https://github.com/zserik/zxtw/src/escape.cxx

namespace zsparsell {
  char escape(char x) noexcept;
  char unescape(char x) noexcept;

# ifdef CRT
#  undef CRT
# endif
# define CRT(F,T) case F: return T;
  char escape(const char x) noexcept {
    switch(x) {
      CRT('\a', 'a')
      CRT('\b', 'b')
      CRT('\033', 'e')
      CRT('\f', 'f')
      CRT('\n', 'n')
      CRT('\r', 'r')
      CRT('\t', 't')
      CRT('\v', 'v')
      CRT('"' , '"')
      CRT('\\', '\\')
      default: return 0;
    }
  }

  char unescape(const char x) noexcept {
    switch(x) {
      CRT('a', '\a')
      CRT('b', '\b')
      CRT('e', '\033')
      CRT('f', '\f')
      CRT('n', '\n')
      CRT('r', '\r')
      CRT('t', '\t')
      CRT('v', '\v')
      // '"' and '\\' are handled implicit
      default:  return x;
    }
  }
}
