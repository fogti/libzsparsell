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

  void Lexer::read_number(uintmax_t &ival) noexcept {
    bool use_hex = false;
    ival = 0;
    if(*_pos == '0') {
      incr();
      if(eof() || *_pos != 'x') return;
      use_hex = true;
      incr();
    }

    if(use_hex) {
      uint8_t tmp;
      while(!eof() && decode_hexnum(*_pos, tmp)) {
        ival *= 16;
        ival += tmp;
        incr();
      }
    } else {
      while(!eof() && isdigit(*_pos)) {
        ival *= 10;
        ival += (*_pos - '0');
        incr();
      }
    }
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
}
