/** zsparsell Parser.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "Lexer.hpp"
#include <deque>

namespace zsparsell {
  class Parser {
   protected:
    Lexer *_lexer;
    std::deque<LexerToken> _lextoks;

    void lookahead(size_t n);
    void lookahead_next() {
      _lextoks.emplace_back(_lexer->get_next());
    }
    uint32_t lookahead_type(const size_t n) {
      lookahead(n);
      return _lextoks.front().type;
    }

    void skip(size_t n) noexcept;
    void skip_to(uint32_t type);

    void errmsg_missing(const char * const non_terminal, const size_t tokn,
      const char * const pre, const std::string &missing);

   public:
    explicit Parser(Lexer * lexer): _lexer(lexer) { }
    virtual ~Parser() = default;
  };
}
