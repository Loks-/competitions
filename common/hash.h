#pragma once

#include "common/hash/hash.h"
#include "common/hash/value.h"

template <class T>
using DHash = nhash::HashSafe<T>;

#include <functional>
constexpr size_t HashCombine(size_t hvalue1, size_t hvalue2) {
  return hvalue1 ^ (hvalue2 + 0x9e3779b9 + (hvalue1 << 6) + (hvalue1 >> 2));
}
