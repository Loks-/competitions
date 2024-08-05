#pragma once

#include "common/factorization/base.h"

constexpr unsigned DivisorsCount(const TFactorization& factorization) {
  unsigned k = 1;
  for (auto p : factorization) k *= (p.power + 1);
  return k;
}
