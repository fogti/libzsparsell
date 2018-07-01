/** zsparsell lexer/get_line.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "lexer_get_line.hpp"

namespace zsparsell {

auto lexer_get_line(const InputSlice &origin,
      const LexPos::lineno line, LexPos::bound _pos)
    noexcept -> InputSlice
{
  // line start and line end
  LexPos::single lst, lnd;

  if(line == origin.line()) {
    lnd = lst = _pos;

    if(_pos == origin.begin()) {
      // done with search for lst
      goto contlnd;
    }

    /* handle the case _pos -> '\n'
     *  if _pos -> '\n' -> EOL, go 1 backwards
     *  else            -> normal char, go 1 backwards doesn't matter
     */
    --lst;
    do {
      if(*lst == '\n') {
        // start of line found
        //  if _pos -> '\n' && (_pos - 1) -> '\n' -> slice empty
        ++lst;
        break;
      }
      --lst;
    } while(lst != origin.begin());
  } else {
    lst = origin.begin();

    // skip to line
    for(size_t i = 0; i < line; ++i) {
      while(lst != origin.end() && *lst != '\n') ++lst;
      if(lst == origin.end()) break;
      ++lst;
    }
    lnd = lst;
  }

 contlnd:
  while(lnd != origin.end() && *lnd != '\n') ++lnd;
  return InputSlice{line, lst, lnd};
}

}
