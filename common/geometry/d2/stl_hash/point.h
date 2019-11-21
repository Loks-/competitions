#pragma once

#include "common/geometry/d2/point.h"
#include "common/hash.h"

namespace std {
template <class T>
struct hash<geometry::d2::Point<T>> {
  size_t operator()(const geometry::d2::Point<T>& value) const {
    return HashCombine(hash<T>{}(value.x), hash<T>{}(value.y));
  }
};
}  // namespace std
