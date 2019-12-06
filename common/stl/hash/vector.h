#pragma once

#include "common/hash.h"
#include <vector>

namespace std {
template <class T>
struct hash<vector<T>> {
  size_t operator()(const vector<T>& v) const {
    hash<T> ht;
    size_t h = v.size();
    for (const T& t : v) h = HashCombine(h, ht(t));
    return h;
  }
};
}  // namespace std
