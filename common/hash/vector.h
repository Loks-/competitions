#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/range.h"

#include <vector>

namespace nhash {
template <class Policy, class T, class TAllocator>
class Hash<Policy, std::vector<T, TAllocator>> {
 public:
  constexpr size_t operator()(const std::vector<T, TAllocator>& v) const {
    return HValueRange<Policy>(v.begin(), v.end());
  }
};
}  // namespace nhash
