#pragma once

#include "common/modular/static/modular.h"

#include <concepts>

/**
 * @brief Alias for a modular arithmetic class with a 32-bit prime modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit prime number as the modulus.
 *
 * @tparam mod The prime modulus. Must be greater than 1 and less than 2^32.
 */
template <uint64_t mod>
  requires((mod > 1) && (mod < (1ULL << 32)))
using TModular_P32 = modular::mstatic::Modular<mod, true, true>;

/**
 * @brief Alias for a modular arithmetic class with a 32-bit composite modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit composite number as the modulus.
 *
 * @tparam mod The composite modulus. Must be greater than 1 and less than 2^32.
 */
template <uint64_t mod>
  requires((mod > 1) && (mod < (1ULL << 32)))
using TModular_C32 = modular::mstatic::Modular<mod, false, true>;

/**
 * @brief Alias for a modular arithmetic class with a prime modulus of
 * 1000000007.
 *
 * This alias is used for performing modular arithmetic operations
 * with the prime number 1000000007 as the modulus.
 * This is a commonly used prime modulus in competitive programming.
 */
using TModularD = TModular_P32<1000000007>;
