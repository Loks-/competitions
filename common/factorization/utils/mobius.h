#pragma once

#include "common/factorization/base.h"

// Number of relative prime integers to n.
constexpr int Mobius(const TFactorization& factorization) {
  for (auto p : factorization)
    if (p.power > 1) return 0;
  return (factorization.size() & 1) ? 1 : -1;
}
