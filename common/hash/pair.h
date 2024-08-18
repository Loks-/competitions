#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/combine.h"
#include "common/hash/policy.h"

#include <utility>

namespace nhash {
template <class Policy, class T1, class T2>
class Hash<Policy, std::pair<T1, T2>> {
 public:
  constexpr size_t operator()(const std::pair<T1, T2>& p) const {
    size_t seed = 0;
    CombineV<TProxyHelperITI<Policy>, T1>(seed, p.first);
    CombineV<TProxyHelperITI<Policy>, T2>(seed, p.second);
    return seed;
  }
};
}  // namespace nhash
