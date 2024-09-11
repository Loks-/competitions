#pragma once

#include "common/hash/hash.h"
#include "common/hash/value.h"

/**
 * @brief A default hash class to use.
 *
 * This is an alias for nhash::HashSafe<T>, providing a default
 * hash implementation for the given type T.
 *
 * @tparam T The type to hash.
 */
template <class T>
using DHash = nhash::HashSafe<T>;
