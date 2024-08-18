#pragma once

#include "common/base.h"
#include "common/hash/base.h"
#include "common/hash/integral.h"
#include "common/hash/policy.h"

namespace nhash {
template <class T>
using HashFast = Hash<PolicyFast, T>;

template <class T>
using HashSafe = Hash<PolicySafe, T>;
}  // namespace nhash
