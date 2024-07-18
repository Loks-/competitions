#pragma once

#include "common/geometry/d2/vector.h"
#include "common/hash.h"

namespace std {
template <class T>
struct hash<geometry::d2::Vector<T>> {
  size_t operator()(const geometry::d2::Vector<T>& value) const {
    return HashCombine(hash<T>{}(value.dx), hash<T>{}(value.dy));
  }
};
}  // namespace std
