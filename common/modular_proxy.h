#pragma once

#include "common/modular/proxy/proxy.h"

/**
 * @brief Alias for a modular proxy class with a 32-bit prime modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit prime number as the modulus, using a proxy pattern.
 */
using TModularProxy_P32U = modular::proxy::Proxy<true, true, uint64_t>;

/**
 * @brief Alias for a modular proxy class with a 32-bit composite modulus.
 *
 * This alias is used for performing modular arithmetic operations
 * with a 32-bit composite number as the modulus, using a proxy pattern.
 */
using TModularProxy_C32U = modular::proxy::Proxy<false, true, uint64_t>;
