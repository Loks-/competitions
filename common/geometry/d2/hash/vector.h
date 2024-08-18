#pragma once

#include "common/geometry/d2/vector.h"
#include "common/hash/base.h"
#include "common/hash/pair.h"

#include <utility>

namespace nhash {
template <class Policy, class T>
class Hash<Policy, geometry::d2::Vector<T>> {
 public:
  constexpr size_t operator()(const geometry::d2::Vector<T>& v) const {
    return Hash<Policy, std::pair<T, T>>{}({v.dx, v.dy});
  }
};
}  // namespace nhash
