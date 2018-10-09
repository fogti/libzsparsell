/** zsparsell Lexer.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "Lexer.hpp"
#include <escape/escape.hpp>
#include "hexcode.hpp"
#include "lexer_get_line.hpp"
#include <stdio.h>

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

  void Lexer::skip_multiline_cmt() noexcept {
    // skip multiline comment
    // . '/' '*'
    incr();
    incr();
    // '/' '*' .
    size_t cmtlvl = 1;
    while(!eof() && cmtlvl) {
      const char curc = *_pos;
      incr();
      switch(*_pos) {
        case '/':
          if(!eof() && *_pos == '*') {
            // recursive cmt
            cmtlvl++;
          }
          break;

        case '*':
          if(!eof() && *_pos == '/') {
            // end of recursive cmt
            cmtlvl--;
          }
          break;

        default: break;
      }
    }
  }

  void Lexer::skip_spaces() noexcept {
    LexPos::single old_pos;
    do {
      old_pos = _pos;
      while(!eof() && isspace(*_pos)) incr();
      const char *const nxtpos = _pos + 1;
      if(eof() || nxtpos == _end) break;

      // skip comments
      bool cmt = false;
      const char nxt = *nxtpos;
      switch(*_pos) {
        case '#':
          if(nxt == '!') cmt = true;
          break;
        case '/':
          switch(nxt) {
            case '/':
              cmt = true;
              break;
            case '*':
              skip_multiline_cmt();
              break;
            default: break;
          }
          break;
      }
      if(cmt) skip_to('\n');
    } while(old_pos != _pos);
  }

  void Lexer::skip_to(const char what) noexcept {
    while(!eof() && *_pos != what) incr();
  }

  bool Lexer::_read_qch_single(char &c) noexcept {
    c = *_pos;
    if(c == '\\') {
      incr();
      if(eof()) return false;
      c = unescape(*_pos);
    }
    incr();
    return true;
  }

  bool Lexer::read_qstring(string &str) {
    // we are only here if we got '"'
    incr();
    if(eof()) return false;

    str.clear();

    while(!eof() && *_pos != '"') {
      char tmp;
      if(_read_qch_single(tmp))
        str += tmp;
      else
        return false;
    }

    // (. '"') -> ('"' .)
    if(!eof()) incr();
    // we are now at the first non-quoted-string character
    return true;
  }

  bool Lexer::read_qchar(string &str) {
    // we are only here if we got '\'' --> quoted character
    incr();
    if(eof()) return false;
    char tmp;
    const bool ret = _read_qch_single(tmp);
    str = { tmp };
    return ret;
  }

  auto Lexer::read_number_generic(uintmax_t &ival, double &fval) noexcept
     -> number_type_t {
    read_number(ival);

    {
      const char *nxtpos = _pos + 1;
      if(eof() || !(*_pos == '.' && _end != nxtpos && isdigit(*nxtpos)))
        return LNT_INT;
    }

    // float parsing
    incr();
    fval = ival;
    size_t part = 10;

    // we are here directly after the dot ('.' . ...)
    do {
      fval += (*_pos - '0') / part;
      part *= 10;
      incr();
    } while(!eof() && isdigit(*_pos));

    return LNT_FLT;
  }

  bool Lexer::read_classified(string &str, const function<uint8_t (char)> &clfn) {
    LexPos::bound spos = _pos;
    const auto lcc = clfn(*_pos);

    incr();
    if(lcc > 1)
      while(!eof() && lcc == clfn(*_pos))
        incr();
    str.assign(spos, _pos);
    return lcc;
  }

  auto Lexer::get_line(const size_t line) const noexcept -> InputSlice {
    return lexer_get_line({_lineno, _begin, _end}, line, _pos);
  }

  // function which repeats char x times
  static inline void repchxt(FILE * const stream, const char c, const size_t n) noexcept {
    for(size_t i = 0; i < n; ++i) fputc(c, stream);
  }

  void Lexer::print_locmsg(const std::map<uint32_t, uint16_t> &colors, const LexerToken &token, const std::string &msg) const {
    fprintf(stderr, "line %zu: %s\n% 6zu │ ", token.line, msg.c_str(), token.line);

    const auto lslc = get_line(token.line);
    if(lslc.size() > token.col) {
      // 1. line start to mark start
      fwrite(lslc.begin(), token.col, 1, stderr);
      // 2. marked area
      const uint16_t color = colors.lower_bound(token.type)->second;
      // color : '0' - 1 = '/'
      if(color >> 8) fprintf(stderr, "\033[4%cm", (color >> 8) + '/');
      fprintf(stderr, "\033[%.2um\033[1m", static_cast<uint32_t>(color & 0xff));
      fwrite(lslc.begin() + token.col, token.cspan, 1, stderr);
      fputs("\033[0m", stderr);
      const size_t awp = token.col + token.cspan;
      if(lslc.size() > awp)
        fwrite(lslc.begin() + awp, lslc.size() - awp, 1, stderr);
    } else {
      fwrite(lslc.begin(), lslc.size(), 1, stderr);
    }

    // print position (for viewers without color display)
    fprintf(stderr, "\n...... │ ");
    repchxt(stderr, ' ', token.col);
    repchxt(stderr, '~', token.cspan);
    fputc('\n', stderr);
  }

  static int zs_is_octnum(const int c) noexcept {
    return '0' <= c && c < '8';
  }

#pragma GCC diagnostic ignored "-Wpedantic"

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

  auto Lexer::param_get_next(const unordered_map<string, uint32_t> &keywords, const std::function<uint8_t (char)> &clfn)
      -> LexerToken {
    LexerToken ret;
    skip_spaces();
    ret.line   = _lineno;
    ret.col    = _column;
    ret.val_bp = _pos;
    if(eof()) {
      ret.type = LT_EOF;
      return ret;
    }

    LexPos::bound spos = _pos;

    switch(static_cast<uint8_t>(*_pos)) {
      case '"':
        // quoted string
        ret.type = LT_STR;
        read_qstring(ret.val_s);
        break;

      case '\'':
        // quoted char
        ret.type = LT_STR;
        read_qchar(ret.val_s);
        break;

      case '0' ... '9':
        // number
        switch(read_number_generic(ret.val_i, ret.val_f)) {
          case LNT_INT:
            ret.type = LT_INT;
            break;
          case LNT_FLT:
            ret.type = LT_FLT;
            break;
        }
        break;

      default:
        ret.type = ([&, this]() -> uint32_t {
          if(!read_classified(ret.val_s, clfn))
            return LT_UKN;
          const auto it = keywords.find(ret.val_s);
          if(it != keywords.end())
            return it->second;
          else if(ret.val_s.size() == 1 && !isalnum(ret.val_s.front()))
            return ret.val_s.front();
          else
            return LT_ID;
        })();
    }

    ret.cspan = _pos - spos;
    return ret;
  }
}
