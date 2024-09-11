#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/combine.h"
#include "common/hash/policy.h"

#include <utility>

namespace nhash {

/**
 * @brief Specialization of Hash for std::pair.
 *
 * This specialization provides a hash function for std::pair based on the
 * specified policy.
 *
 * @tparam Policy The policy class that defines the hashing behavior.
 * @tparam T1 The type of the first element of the pair.
 * @tparam T2 The type of the second element of the pair.
 */
template <class Policy, class T1, class T2>
class Hash<Policy, std::pair<T1, T2>> {
 public:
  /**
   * @brief Computes the hash value for a std::pair.
   *
   * This function combines the hash values of the first and second elements of
   * the pair using the specified policy.
   *
   * @param p The pair to be hashed.
   * @return The combined hash value of the pair.
   */
  constexpr size_t operator()(const std::pair<T1, T2>& p) const {
    size_t seed = 0;
    CombineV<TProxyHelperITI<Policy>, T1>(seed, p.first);
    CombineV<TProxyHelperITI<Policy>, T2>(seed, p.second);
    return seed;
  }
};

}  // namespace nhash
