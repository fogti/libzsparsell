/** zsparsell Parser.cxx
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include "Parser.hpp"
#include <string.h>
using namespace std;

namespace zsparsell {
  void Parser::lookahead(size_t n) {
    if(_lextoks.size() >= n) return;
    const size_t rest = n - _lextoks.size();
    for(size_t i = 0; i < rest; ++i) lookahead_next();
  }

  void Parser::skip(size_t n) noexcept {
    if(_lextoks.size() <= n)
      _lextoks.clear();
    else
      _lextoks.erase(_lextoks.cbegin(), _lextoks.cbegin() + n);
  }

  void Parser::skip_to(uint32_t type) {
    for(lookahead(2); _lextoks.front().type != type &&
                      _lextoks.front().type != LT_EOF; lookahead_next())
      skip(1);
  }

  void Parser::errmsg_missing(const char * const non_terminal, const size_t tokn,
      const char * const pre, const string &missing) {
    string full_pre, errtt;
    if(const size_t x = strlen(pre)) {
      full_pre.reserve(x + 7);
      (full_pre += " after ") += pre;
    }
    errtt.reserve(strlen(non_terminal) + missing.size() + 10);
    errtt += non_terminal;
    (errtt += ": missing ") += missing;
    errtt += move(full_pre);
    _lexer->errmsg(_lextoks.at(tokn), move(errtt));
    if(missing.size() == 3 && missing[0] == '\'' && missing[2] == '\'') {
      skip_to(missing[1]); skip(1);
    }
  }
}
