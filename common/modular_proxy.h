#pragma once

#include "common/modular/proxy/proxy.h"

/**
 * @brief Alias for a modular proxy class with a 32-bit prime modulus.
 *
 * This alias provides a type-safe modular arithmetic implementation using
 * a proxy pattern, optimized for 32-bit prime moduli. It supports all basic
 * arithmetic operations (+, -, *, /) with automatic modular reduction.
 *
 * @tparam ValueType The type used for internal calculations (uint64_t).
 *                   Must be large enough to handle intermediate results.
 */
using ModularProxyPrime32 = modular::proxy::Proxy<true, true, uint64_t>;

/**
 * @brief Alias for a modular proxy class with a 32-bit composite modulus.
 *
 * This alias provides a type-safe modular arithmetic implementation using
 * a proxy pattern, optimized for 32-bit composite moduli. It supports all
 * basic arithmetic operations (+, -, *, /) with automatic modular reduction.
 *
 * @tparam ValueType The type used for internal calculations (uint64_t).
 *                   Must be large enough to handle intermediate results.
 */
using ModularProxyComposite32 = modular::proxy::Proxy<false, true, uint64_t>;
