#pragma once

#include "common/base.h"
#include "common/hash/combine.h"
#include "common/hash/policy.h"

#include <iterator>

namespace nhash {
template <class Policy, class T>
constexpr size_t HValueRange(const T& begin, const T& end) {
  size_t seed = 0;
  for (auto it = begin; it != end;)
    CombineV<TProxyHelperITI<Policy>,
             typename std::iterator_traits<T>::value_type>(seed, *it++);
  return seed;
}
}  // namespace nhash
