#pragma once

#include "common/base.h"
#include "common/hash/base.h"

namespace nhash {
template <class Policy, class T>
constexpr size_t HValue(const T& x) {
  return Hash<Policy, T>{}(x);
}
}  // namespace nhash
