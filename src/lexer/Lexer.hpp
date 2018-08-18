/** zsparsell Lexer.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "InputSlice.hpp"
namespace zsparsell {
  class Lexer {
   protected:
    const char * const _filename;
    LexPos::lineno  _lineno;
    LexPos::columno _column;
    LexPos::bound   _begin, _end;
    LexPos::single  _pos;

    bool eof() const noexcept { return _pos == _end; }

    // ++_pos and keeps track of column and line
    void incr() noexcept;

    // skip functions
    virtual void skip_spaces() noexcept = 0;
    void skip_to(char what) noexcept;

    // read functions
    bool read_qstring(std::string &str);
    void read_number(uintmax_t &ival) noexcept;

    // read double after dot
    void read_fladot(double &fval) noexcept;

   public:
    Lexer(const char * const filename, LexPos::bound begf,
                                       LexPos::bound endf);

    virtual ~Lexer() = default;

    /* get_line seeks to the start of the current line
     * @param  line    line number
     * @return slice @ whole line
     */
    auto get_line(size_t lineno) const noexcept -> InputSlice;
  };
}
