#pragma once

#include "common/hash/combine.h"

namespace tester {

constexpr void hash_combine(size_t& seed, size_t value) {
  nhash::CombineHI(seed, value,
                   nhash::TMetaCombine<nhash::EPolicyCombine::XOR>{});
}

}  // namespace tester
