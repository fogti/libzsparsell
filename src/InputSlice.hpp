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
    LexPos::columno _col;
    LexPos::single _begin, _end;

   public:
    InputSlice(const LexPos::lineno line, const LexPos::columno col,
      LexPos::bound begos, LexPos::bound endos)
      : _line(line), _col(col), _begin(begos), _end(endos) { }

    auto to_string() const { return std::string(_begin, _end); }

    size_t size() const noexcept { return _end - _begin;  }
    bool empty()  const noexcept { return _begin == _end; }

    auto begin()  const noexcept { return _begin;      }
    auto end()    const noexcept { return _end;        }
    auto front()  const noexcept { return *_begin;     }
    auto back()   const noexcept { return *(_end - 1); }

    auto line()   const noexcept { return _line; }
    auto column() const noexcept { return _col;  }
  };
}
