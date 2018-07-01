/** zsparsell lexer_get_line.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "InputSlice.hpp"

namespace zsparsell {

  /* lexer_get_line: get line from source
   *
   * @param origin { line, 0, _begin, _end }
   * @param line   current line
   * @param pos    current position
   */
  auto lexer_get_line(const InputSlice &origin,
      const LexPos::lineno line, LexPos::bound _pos)
    noexcept -> InputSlice;

}
