#pragma once

#include "common/factorization/base.h"

namespace factorization {
namespace hidden {
inline void GetDivisorsI(const TFactorization& factorization, unsigned fpi,
                         uint64_t current, std::vector<uint64_t>& output) {
  if (fpi < factorization.size()) {
    for (unsigned i = 0; i <= factorization[fpi].power; ++i) {
      GetDivisorsI(factorization, fpi + 1, current, output);
      current *= factorization[fpi].prime;
    }
  } else {
    output.push_back(current);
  }
}
}  // namespace hidden
}  // namespace factorization

inline std::vector<uint64_t> GetDivisors(const TFactorization& factorization) {
  std::vector<uint64_t> v;
  factorization::hidden::GetDivisorsI(factorization, 0, 1, v);
  return v;
}
