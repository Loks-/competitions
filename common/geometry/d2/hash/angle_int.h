#pragma once

#include "common/geometry/d2/angle_int.h"
#include "common/hash/base.h"
#include "common/hash/pair.h"

#include <utility>

namespace nhash {
template <class Policy, class T>
class Hash<Policy, geometry::d2::IAngle<T>> {
 public:
  constexpr size_t operator()(const geometry::d2::IAngle<T>& v) const {
    return Hash<Policy, std::pair<T, T>>{}({v.dx, v.dy});
  }
};
}  // namespace nhash
