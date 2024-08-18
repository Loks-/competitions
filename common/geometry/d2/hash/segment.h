#pragma once

#include "common/geometry/d2/point.h"
#include "common/geometry/d2/segment.h"
#include "common/hash/base.h"
#include "common/hash/pair.h"

#include <utility>

namespace nhash {
template <class Policy, class T, bool closed>
class Hash<Policy, geometry::d2::Segment<T, closed>> {
 public:
  constexpr size_t operator()(const geometry::d2::Segment<T, closed>& s) const {
    return Hash<Policy,
                std::pair<geometry::d2::Point<T>, geometry::d2::Point<T>>>{}(
        {s.p1, s.p2});
  }
};
}  // namespace nhash
