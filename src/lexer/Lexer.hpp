/** zsparsell Lexer.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "InputSlice.hpp"
#include <inttypes.h>
#include <functional>
#include <unordered_map>

// Lexer Token Types
#define LT_EOF 0x100 //      end of file
#define LT_UKN 0x101 // <bp> unknown

#define LT_INT 0x102 // <i>  integer
#define LT_FLT 0x103 // <f>  floating point number
#define LT_ID  0x104 // <s>  identifier
#define LT_STR 0x105 // <s>  string

namespace zsparsell {
  struct LexerToken {
    uint32_t type;

    // values
    std::string val_s;

    union {
      uintmax_t val_i;
      double    val_f;
      LexPos::single val_bp;
    };

    // position
    LexPos::lineno  line;
    LexPos::columno col;
    LexPos::colspan cspan;

    LexerToken():
      type(LT_UKN), val_i(0), line(0), col(0), cspan(0) { }
  };

  class Lexer {
    bool _read_qch_single(char &c) noexcept;

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
    void skip_multiline_cmt() noexcept;
    virtual void skip_spaces() noexcept;
    void skip_to(char what) noexcept;

    // read functions
    bool read_qstring(std::string &str);
    bool read_qchar(std::string &str);
    void read_number(uintmax_t &ival) noexcept;

    enum number_type_t { LNT_INT, LNT_FLT };
    auto read_number_generic(uintmax_t &ival, double &fval) noexcept
      -> number_type_t;

    // read word based on clssification
    // special return values of clfn(...)
    //  0 : no classification (type = LT_UKN)
    //  1 : single-char split (type = *_pos)
    bool read_classified(std::string &str, const std::function<uint8_t (char)> &clfn);

    // a parameterized get_next function
    auto param_get_next(const std::unordered_map<std::string, uint32_t> &keywords, const std::function<uint8_t (char)> &clfn)
      -> LexerToken;

   public:
    Lexer(const char * const filename, LexPos::bound begf,
                                       LexPos::bound endf);

    virtual ~Lexer() = default;

    /* get_line seeks to the start of the current line
     * @param  line    line number
     * @return slice @ whole line
     */
    auto get_line(size_t lineno) const noexcept -> InputSlice;

    virtual auto get_next() -> LexerToken = 0;
  };
}
