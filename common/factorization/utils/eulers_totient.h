#pragma once

#include "common/factorization/base.h"

// Number of relative prime integers to n.
constexpr uint64_t EulersTotient(uint64_t n,
                                 const TFactorization& factorization) {
  uint64_t output = n;
  for (auto p : factorization) output -= output / p.prime;
  return output;
}
