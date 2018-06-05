#pragma once
/** zsparsell escape.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#include <string>
namespace zsparsell {
  /* escape_single
   * escapes a single character
   * if there is a valid escape sequence, it returns the escape char,
   *  which should be pretended with a '\\' (backslash)
   * otherwise it returns '\0'
   */
  char escape(char x) noexcept;

  auto escape(const char *s) noexcept -> char*;
  auto escape(const std::string &s) -> std::string;

  /* unescape_single
   * unescapes a single character
   * @param x = character after '\\' (backslash)
   * @return x <or> real character
   */
  char unescape(char x) noexcept;

  auto unescape(const char *s) noexcept -> char*;
  auto unescape(const std::string &s) -> std::string;
}
