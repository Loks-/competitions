#pragma once

#include "common/base.h"

#include <string>

template <class T = uint64_t>
inline T Parse(const std::string& s, T base = 10) {
  T x = 0;
  for (char c : s) {
    assert(((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')));
    x *= base;
    if ((c >= '0') && (c <= '9')) x += (c - '0');
    if ((c >= 'A') && (c <= 'Z')) x += (c - 'A') + 10;
  }
  return x;
}
