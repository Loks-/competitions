#pragma once

#include "common/geometry/d2/point.h"
#include "common/hash/base.h"
#include "common/hash/pair.h"

#include <utility>

namespace nhash {
template <class Policy, class T>
class Hash<Policy, geometry::d2::Point<T>> {
 public:
  constexpr size_t operator()(const geometry::d2::Point<T>& p) const {
    return Hash<Policy, std::pair<T, T>>{}({p.x, p.y});
  }
};
}  // namespace nhash
