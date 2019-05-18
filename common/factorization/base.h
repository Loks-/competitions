#pragma once

#include "common/base.h"
#include <vector>

namespace factorization {
struct PrimePower {
  uint64_t prime;
  unsigned power;
};
}  // namespace factorization

using TFactorization = std::vector<factorization::PrimePower>;
