#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/range.h"

#include <array>

namespace nhash {
template <class Policy, class T, size_t size>
class Hash<Policy, std::array<T, size>> {
 public:
  constexpr size_t operator()(const std::array<T, size>& v) const {
    return HValueRange<Policy>(v.begin(), v.end());
  }
};
}  // namespace nhash
