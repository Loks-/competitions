#pragma once

#include "common/hash/combine.h"

namespace tester {

constexpr size_t hash_combine(size_t& seed, size_t value) {
  nhash::CombineHI(seed, value,
                   nhash::TMetaCombine<nhash::EPolicyCombine::XOR>{});
  return seed;
}

}  // namespace tester
