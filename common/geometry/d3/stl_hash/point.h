#pragma once

#include "common/geometry/d3/point.h"
#include "common/hash.h"

namespace std {
template <class T>
struct hash<geometry::d3::Point<T>> {
  constexpr size_t operator()(const geometry::d3::Point<T>& value) const {
    return HashCombine(HashCombine(hash<T>{}(value.x), hash<T>{}(value.y)),
                       hash<T>{}(value.z));
  }
};
}  // namespace std
