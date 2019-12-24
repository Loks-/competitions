#include "common/base.h"
#include "common/factorization/base.h"
#include "common/factorization/primality_test.h"
#include "common/factorization/table/primes.h"
#include "common/factorization/table/utils/factorize_extended.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

inline std::vector<unsigned> PrimesPowers(const PrimesTable& pt, uint64_t n) {
  thread_local factorization::PrimalityTest primality_test;
  if (n <= 1) return {};
  TFactorization f = FactorizeExtended(pt, n, true);
  std::vector<unsigned> output;
  for (auto p : f) output.push_back(p.power);
  n = f.back().prime;
  if (n <= pt.GetSquaredTableSize()) return output;
  if (primality_test.IsPrime(n)) return output;
  uint64_t nsqrt = USqrt(n);
  if (nsqrt * nsqrt == n)
    output.back() = 2;
  else
    output.push_back(1);
  return output;
}
