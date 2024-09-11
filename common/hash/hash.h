#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/integral.h"
#include "common/hash/policy.h"

namespace nhash {

/**
 * @brief Alias for fast hash policy.
 *
 * This alias uses the PolicyFast policy for hashing objects of type T.
 *
 * @tparam T The type of the object to be hashed.
 */
template <class T>
using HashFast = Hash<PolicyFast, T>;

/**
 * @brief Alias for safe hash policy.
 *
 * This alias uses the PolicySafe policy for hashing objects of type T.
 *
 * @tparam T The type of the object to be hashed.
 */
template <class T>
using HashSafe = Hash<PolicySafe, T>;

}  // namespace nhash
