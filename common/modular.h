#pragma once

#include "common/modular/static/modular.h"

#include <concepts>

/**
 * @brief Alias for a modular arithmetic class with a 32-bit prime modulus.
 *
 * This alias provides a type-safe modular arithmetic implementation optimized
 * for 32-bit prime moduli. It supports all basic arithmetic operations (+, -,
 * *, /) with automatic modular reduction.
 *
 * @tparam modulus The prime modulus. Must be greater than 1 and less than 2^32.
 *                 The modulus must be prime for division operations to work
 * correctly.
 */
template <uint64_t modulus>
  requires((modulus > 1) && (modulus < (1ULL << 32)))
using ModularPrime32 = modular::mstatic::Modular<modulus, true, true>;

/**
 * @brief Alias for a modular arithmetic class with a 32-bit composite modulus.
 *
 * This alias provides a type-safe modular arithmetic implementation optimized
 * for 32-bit composite moduli. It supports all basic arithmetic operations (+,
 * -, *, /) with automatic modular reduction.
 *
 * @tparam modulus The composite modulus. Must be greater than 1 and less than
 * 2^32.
 */
template <uint64_t modulus>
  requires((modulus > 1) && (modulus < (1ULL << 32)))
using ModularComposite32 = modular::mstatic::Modular<modulus, false, true>;

/**
 * @brief Alias for a modular arithmetic class with the default prime modulus
 * 1000000007.
 *
 * This alias provides a type-safe modular arithmetic implementation using
 * the prime number 1000000007 as the modulus. This is a commonly used prime
 * modulus in competitive programming due to its properties:
 * - It's large enough to avoid overflow in most cases
 * - It's prime, allowing efficient division operations
 * - It's easy to remember (10^9 + 7)
 *
 * Example usage:
 * @code
 * ModularDefault a(5), b(3);
 * auto sum = a + b;      // Modular addition
 * auto prod = a * b;     // Modular multiplication
 * auto quotient = a / b; // Modular division (efficient for prime modulus)
 * @endcode
 */
using ModularDefault = ModularPrime32<1000000007>;
