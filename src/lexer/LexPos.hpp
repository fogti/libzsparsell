/** zsparsell LexPos.hpp
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#include <stddef.h>
namespace zsparsell {
  // anything in namespace LexPos is a typedef
  namespace LexPos {
    // single  : a simple single char pointer
    typedef const char * single;

    // bound   : a char pointer which marks bounds of input
    typedef const single bound;

    // ref     : a reference to a char pointer (used in backtracing)
    typedef single&      ref;

    // lineno  : type for line numbers
    typedef size_t       lineno;

    // columno : type for column numbers
    typedef size_t       columno;

    // colspan : type for column span
    typedef columno      colspan;
  }
}
