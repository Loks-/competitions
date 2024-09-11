#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/mix.h"
#include "common/hash/policy.h"
#include "common/template.h"

namespace nhash {

/**
 * @brief Combines a seed with a hash value using XOR policy.
 *
 * This function combines a seed with a hash value using the XOR policy. See
 * boost 1.33 for more information.
 *
 * @param seed The seed to be combined.
 * @param hvalue The hash value to combine with the seed.
 * @param TMetaCombine<EPolicyCombine::XOR> The XOR policy.
 */
constexpr void CombineHI(size_t& seed, size_t hvalue,
                         TMetaCombine<EPolicyCombine::XOR>) {
  seed ^= (hvalue + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

/**
 * @brief Combines a seed with a hash value using MIX policy.
 *
 * This function combines a seed with a hash value using the MIX policy.
 *
 * @param seed The seed to be combined.
 * @param hvalue The hash value to combine with the seed.
 * @param TMetaCombine<EPolicyCombine::MIX> The MIX policy.
 */
constexpr void CombineHI(size_t& seed, size_t hvalue,
                         TMetaCombine<EPolicyCombine::MIX>) {
  seed = Mix(seed + 0x9e3779b9 + hvalue);
}

/**
 * @brief Combines a seed with a hash value using the specified policy.
 *
 * This function combines a seed with a hash value using the policy specified in
 * the template parameter.
 *
 * @tparam Policy The policy to use for combining the seed and hash value.
 * @param seed The seed to be combined.
 * @param hvalue The hash value to combine with the seed.
 */
template <class Policy>
constexpr void CombineH(size_t& seed, size_t hvalue) {
  CombineHI(seed, hvalue, TMetaCombine<Policy::combine>{});
}

/**
 * @brief Combines a seed with the hash value of an object using the specified
 * policy.
 *
 * This function combines a seed with the hash value of an object using the
 * policy specified in the template parameter.
 *
 * @tparam Policy The policy to use for combining the seed and hash value.
 * @tparam T The type of the object to be hashed.
 * @param seed The seed to be combined.
 * @param value The object whose hash value is to be combined with the seed.
 */
template <class Policy, class T>
constexpr void CombineV(size_t& seed, const T& value) {
  CombineH<Policy>(seed, Hash<Policy, T>{}(value));
}

/**
 * @brief Combines a seed with a hash value using the safe policy.
 *
 * This function combines a seed with a hash value using the safe policy (MIX).
 *
 * @param seed The seed to be combined.
 * @param hvalue The hash value to combine with the seed.
 */
constexpr void DCombineH(size_t& seed, size_t hvalue) {
  return CombineH<nhash::PolicySafe>(seed, hvalue);
}

}  // namespace nhash
