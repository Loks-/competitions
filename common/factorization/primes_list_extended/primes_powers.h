#pragma once

#include "common/factorization/primality_test.h"
#include "common/factorization/primes_list.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

namespace factorization {
namespace ple {
class PrimesPowers : public PrimesList {
 protected:
  PrimalityTest primality_test;

 public:
  PrimesPowers(uint64_t size = 3000000) : PrimesList(size) {}

  std::vector<unsigned> Get(uint64_t n) const {
    if (n <= 1) return {};
    TFactorization f = PrimesList::Factorize(n, true);
    std::vector<unsigned> output;
    for (auto p : f) output.push_back(p.power);
    n = f.back().prime;
    if (n <= PrimesList::squared_table_size) return output;
    if (primality_test.IsPrime(n)) return output;
    uint64_t nsqrt = USqrt(n);
    if (nsqrt * nsqrt == n)
      output.back() = 2;
    else
      output.push_back(1);
    return output;
  }
};
}  // namespace ple
}  // namespace factorization
