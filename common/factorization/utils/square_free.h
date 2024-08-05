#pragma once

#include "common/factorization/base.h"

constexpr bool SquareFree(const TFactorization& factorization) {
  for (auto p : factorization) {
    if (p.power > 1) return false;
  }
  return true;
}
