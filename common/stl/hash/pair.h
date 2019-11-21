#pragma once

#include "common/hash.h"
#include <utility>

namespace std {
template <class T1, class T2>
struct hash<pair<T1, T2>> {
  size_t operator()(const pair<T1, T2>& value) const {
    return HashCombine(hash<T1>{}(value.first), hash<T2>{}(value.second));
  }
};
}  // namespace std
