/** zsparsell hash.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <zs/ll/hash.hpp>
namespace zsparsell {
  template<class T>
  using hash_combine = llzs::hash_combine<T>;

  template<typename T, typename... Types>
  using hash_combine = llzs::hash_combine<T, Types...>;

  template<typename... Types>
  using hash_val     = llzs::hash_val<Types...>;
}
