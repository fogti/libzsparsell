/** zsparsell Lexer.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "Lexer.hpp"
#include <escape/escape.hpp>
#include "hexcode.hpp"
#include "lexer_get_line.hpp"

using namespace std;

namespace zsparsell {
  Lexer::Lexer(const char * const filename, LexPos::bound begf, LexPos::bound endf)
    : _filename(filename), _lineno(0), _column(0), _begin(begf), _end(endf), _pos(begf) { }

  [[gnu::hot]] void Lexer::incr() noexcept {
    if(eof()) return;
    if(*_pos == '\n') {
      ++_lineno;
      _column = 0;
    } else {
      ++_column;
    }
    ++_pos;
  }

  void Lexer::skip_to(const char what) noexcept {
    while(!eof() && *_pos != what) incr();
  }

  bool Lexer::read_qstring(string &str) {
    // we are only here if we got '"'
    incr();
    if(eof()) return false;

    str.clear();

    while(!eof() && *_pos != '"') {
      if(*_pos == '\\') {
        incr();
        if(eof()) return false;
        str += unescape(*_pos);
      } else {
        str += *_pos;
      }
      incr();
    }

    // (. '"') -> ('"' .)
    if(!eof()) incr();
    // we are now at the first non-quoted-string character
    return true;
  }

  bool Lexer::fladot_follows() const noexcept {
    const char *nxtpos = _pos + 1;
    return !eof() && *_pos == '.' && _end != nxtpos && isdigit(*nxtpos);
  }

  void Lexer::read_fladot(double &fval) noexcept {
    // we are here directly after the dot ('.' . ...)
    size_t part = 10;
    do {
      fval += (*_pos - '0') / part;
      part *= 10;
      incr();
    } while(!eof() && isdigit(*_pos));
  }

  auto Lexer::read_number_generic(uintmax_t &ival, double &fval) noexcept
     -> number_type_t {
    read_number(ival);
    if(!fladot_follows())
      return LNT_INT;

    // float
    incr();
    fval = ival;
    read_fladot(fval);
    return LNT_FLT;
  }

  bool Lexer::read_classified(string &str, const function<uint8_t (char)> clfn) {
    LexPos::bound spos = _pos;
    const auto lcc = clfn(*_pos);

    incr();
    if(lcc)
      while(!eof() && lcc == clfn(*_pos))
        incr();
    str.assign(spos, _pos);
    return lcc;
  }

  auto Lexer::get_line(const size_t line) const noexcept -> InputSlice {
    return lexer_get_line({_lineno, _begin, _end}, line, _pos);
  }

#pragma GCC diagnostic ignored "-Wpedantic"
  static int zs_is_octnum(const int c) noexcept {
    return '0' <= c && c < '8';
  }

  void Lexer::read_number(uintmax_t &ival) noexcept {
    enum detail_numtype_t { LDNT_INT, LDNT_HEX, LDNT_OCT } use_format = LDNT_INT;
    uint8_t radix = 10;
    int (*clchkfn)(int) = isdigit;
    ival = 0;
    if(*_pos == '0') {
      incr();
      if(eof()) return;
      switch(*_pos) {
        case '0' ... '7':
          use_format = LDNT_OCT;
          radix = 8;
          clchkfn = zs_is_octnum;
          break;
        case 'x':
          use_format = LDNT_HEX;
          radix = 16;
          incr();
          break;
        default:
          return;
      }
    }

    uint8_t tmp;
    switch(use_format) {
     case LDNT_HEX:
      while(!eof() && decode_hexnum(*_pos, tmp)) {
        ival *= radix;
        ival += tmp;
        incr();
      }
      break;

     case LDNT_OCT:
     case LDNT_INT:
       while(!eof() && clchkfn(*_pos)) {
        ival *= radix;
        ival += (*_pos - '0');
        incr();
      }
      break;
    }
  }
}
