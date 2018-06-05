#pragma once
/** zsparsell reverse.hpp --- Reversed iterable
    (C) 2018 Erik Zscheile
    License: MIT
 **/
namespace zsparsell {
  template <typename T>
  struct reversion_wrapper { T& iterable; };

  template <typename T>
  auto begin(reversion_wrapper<T> w) {
    using std::rbegin;
    return rbegin(w.iterable);
  }

  template <typename T>
  auto end(reversion_wrapper<T> w) {
    using std::rend;
    return rend(w.iterable);
  }

  template <typename T>
  reversion_wrapper<T> reverse(T&& iterable) { return { iterable }; }
}
