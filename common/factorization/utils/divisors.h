#pragma once

#include "common/factorization/base.h"
#include <functional>

inline std::vector<uint64_t> GetDivisors(const TFactorization& factorization) {
  std::vector<uint64_t> v;
  std::function<void(unsigned, uint64_t)> AddR = [&](unsigned fpi,
                                                     uint64_t current) -> void {
    if (fpi < factorization.size()) {
      for (unsigned i = 0; i <= factorization[fpi].power; ++i) {
        AddR(fpi + 1, current);
        current *= factorization[fpi].prime;
      }
    } else {
      v.push_back(current);
    }
  };
  AddR(0, 1);
  return v;
}
