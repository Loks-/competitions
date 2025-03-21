#pragma once

#include "common/hash/hash.h"
#include "common/hash/value.h"

/**
 * @brief A default hash class to use.
 *
 * This is an alias for nhash::HashSafe<T>, providing a default
 * hash implementation for the given type T.
 *
 * @tparam ValueType The type to hash.
 */
template <typename ValueType>
using DHash = nhash::HashSafe<ValueType>;
