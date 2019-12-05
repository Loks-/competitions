#include "tester/prime_generation.h"

#include "common/base.h"
#include "common/factorization/primality_test.h"
#include "common/factorization/primes_list.h"
#include <vector>

std::vector<uint64_t> GetPrimes_PrimalityTest(uint64_t maxn) {
  std::vector<uint64_t> vp;
  if (maxn >= 2) vp.push_back(2);
  for (uint64_t i = 3; i <= maxn; i += 2) {
    if (IsPrime(i)) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_PrimesList(uint64_t maxn) {
  return factorization::PrimesList(maxn).GetPrimes();
}
