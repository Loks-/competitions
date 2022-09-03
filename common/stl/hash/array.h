#pragma once

#include "common/hash.h"

#include <array>

namespace std {
template <class T, size_t n>
struct hash<array<T, n>> {
  size_t operator()(const array<T, n>& v) const {
    hash<T> ht;
    size_t h = v.size();
    for (const T& t : v) h = HashCombine(h, ht(t));
    return h;
  }
};
}  // namespace std
