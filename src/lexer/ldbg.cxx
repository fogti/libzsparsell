/** zsparsell lexer/ldbg.cxx
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
  void Lexer::lexdbg() {
    size_t lastline = 0;
    string pfl;
    while(true) {
      const auto token = get_next();
      if(token.type == LT_EOF) break;
      if(lastline != token.line) {
        printf("%s\033[0m\n\n", pfl.c_str());
        pfl.clear();
      }
      printf("L%zu:%zu,%zux\t", token.line, token.col, token.cspan);

      switch(token.type) {
        case LT_UKN:
          printf("unknown '%c'", *token.val_bp);
          (pfl += " \033[91m") += *token.val_bp;
          break;

        case LT_INT:
          printf("integer %lu", token.val_i);
          (pfl += " \033[92m") += to_string(token.val_i);
          break;

        case LT_FLT:
          printf("float %f", token.val_f);
          (pfl += " \033[93m") += to_string(token.val_f);
          break;

        case LT_ID:
          printf("identifier %s", token.val_s.c_str());
          (pfl += " \033[33m") += move(token.val_s);
          break;

        case LT_STR:
          {
            string escv = zsparsell::escape(token.val_s);
            printf("string \"%s\"", escv.c_str());
            (pfl += " \033[32m") += move(escv);
          }
          break;

        case 0x01 ... 0x7e:
          printf("got char '");
          pfl += " \033[0m";
          {
            auto ch = static_cast<char>(token.type);
            if(char calt = zsparsell::escape(ch)) {
              printf("\\%c", calt);
              (pfl += '\\') += calt;
            } else {
              putchar(ch);
              pfl += ch;
            }
          }
          printf("'");
          break;

        default:
          printf("got keyword %s", token.val_s.c_str());
          (pfl += " \033[35m") += move(token.val_s);
      }
      puts("");
      lastline = token.line;
    }
    printf("%s\033[0m\n\n", pfl.c_str());
  }

  void Lexer::lerrdbg() {
    while(true) {
      const auto token = get_next();
      if(token.type == LT_EOF) break;
      errmsg(token, string());
    }
  }
}
