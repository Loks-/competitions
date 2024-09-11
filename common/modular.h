#pragma once

#include "common/modular/static/modular.h"

/**
 * @brief Alias for a modular arithmetic class with a 32-bit prime modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit prime number as the modulus.
 *
 * @tparam mod The prime modulus.
 */
template <uint64_t mod>
using TModular_P32 = modular::mstatic::Modular<mod, true, true>;

/**
 * @brief Alias for a modular arithmetic class with a 32-bit composite modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit composite number as the modulus.
 *
 * @tparam mod The composite modulus.
 */
template <uint64_t mod>
using TModular_C32 = modular::mstatic::Modular<mod, false, true>;

/**
 * @brief Alias for a modular arithmetic class with a prime modulus of
 * 1000000007.
 *
 * This alias is used for performing modular arithmetic operations
 * with the prime number 1000000007 as the modulus.
 */
using TModularD = TModular_P32<1000000007>;
