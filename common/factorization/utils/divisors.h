#pragma once

#include "../base.h"

inline unsigned GetDivisorsCount(const TFactorization& factorization) {
  unsigned k = 1;
  for (auto p : factorization) k *= (p.second + 1);
  return k;
}

inline void GetDivisorsI(const TFactorization& factorization, unsigned fpi,
                         uint64_t current, std::vector<uint64_t>& output) {
  if (fpi < factorization.size()) {
    for (unsigned i = 0; i <= factorization[fpi].second; ++i) {
      GetDivisorsI(factorization, fpi + 1, current, output);
      current *= factorization[fpi].first;
    }
  } else {
    output.push_back(current);
  }
}

inline std::vector<uint64_t> GetDivisors(const TFactorization& factorization) {
  std::vector<uint64_t> v;
  GetDivisorsI(factorization, 0, 1, v);
  return v;
}
