#pragma once

#include "common/geometry/d3/point.h"
#include "common/hash/array.h"
#include "common/hash/base.h"

#include <array>

namespace nhash {
template <class Policy, class T>
class Hash<Policy, geometry::d3::Point<T>> {
 public:
  constexpr size_t operator()(const geometry::d3::Point<T>& p) const {
    return Hash<Policy, std::array<T, 3>>{}({p.x, p.y, p.z});
  }
};
}  // namespace nhash
