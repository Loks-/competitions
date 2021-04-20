#pragma once

#include "common/geometry/d2/angle_int.h"
#include "common/hash.h"

namespace std {
template <class T>
struct hash<geometry::d2::IAngle<T>> {
  size_t operator()(const geometry::d2::IAngle<T>& value) const {
    return HashCombine(hash<T>{}(value.dx), hash<T>{}(value.dy));
  }
};
}  // namespace std
