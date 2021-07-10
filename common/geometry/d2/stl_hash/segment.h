#pragma once

#include "common/geometry/d2/segment.h"
#include "common/geometry/d2/stl_hash/point.h"
#include "common/hash.h"

namespace std {
template <class T, bool closed>
struct hash<geometry::d2::Segment<T, closed>> {
  size_t operator()(const geometry::d2::Segment<T, closed>& value) const {
    return HashCombine(hash<geometry::d2::Point<T>>{}(value.p1),
                       hash<geometry::d2::Point<T>>{}(value.p2));
  }
};
}  // namespace std
