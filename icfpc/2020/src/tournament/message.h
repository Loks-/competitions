#pragma once

#include "common/base.h"
#include "common/data_structures/recursive_vector.h"

namespace tournament {
const int64_t PAIR_CODE = (1ll << 48);
using Message = ds::RecursiveVector<int64_t>;

void Print(const Message& m);
}  // namespace tournament
