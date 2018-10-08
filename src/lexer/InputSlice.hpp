/** zsparsell InputSlice.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include "LexPos.hpp"
#include <string>

namespace zsparsell {
  class InputSlice final {
    LexPos::lineno _line;
    LexPos::single _begin, _end;

   public:
    InputSlice(const LexPos::lineno line, LexPos::bound begos, LexPos::bound endos)
      : _line(line), _begin(begos), _end(endos) { }

    auto to_string() const { return std::string(_begin, _end); }

    size_t size() const noexcept { return _end - _begin;  }
    bool empty()  const noexcept { return _begin == _end; }
    auto line()   const noexcept { return _line; }

    auto begin()  const noexcept { return _begin;      }
    auto end()    const noexcept { return _end;        }
    auto front()  const noexcept { return *_begin;     }
    auto back()   const noexcept { return *(_end - 1); }
  };
}
