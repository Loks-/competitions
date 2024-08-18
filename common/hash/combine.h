#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/mix.h"
#include "common/hash/policy.h"
#include "common/template.h"

namespace nhash {
// Boost 1.33
constexpr void CombineHI(size_t& seed, size_t hvalue,
                         TFakeCombine<EPolicyCombine::XOR>) {
  seed ^= (hvalue + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

constexpr void CombineHI(size_t& seed, size_t hvalue,
                         TFakeCombine<EPolicyCombine::MIX>) {
  seed = Mix(seed + 0x9e3779b9 + hvalue);
}

template <class Policy>
constexpr void CombineH(size_t& seed, size_t hvalue) {
  CombineHI(seed, hvalue, TFakeCombine<Policy::combine>{});
}

template <class Policy, class T>
constexpr void CombineV(size_t& seed, const T& value) {
  CombineH<Policy>(seed, Hash<Policy, T>{}(value));
}

constexpr void DCombineH(size_t& seed, size_t hvalue) {
  return CombineH<nhash::PolicySafe>(seed, hvalue);
}
}  // namespace nhash
