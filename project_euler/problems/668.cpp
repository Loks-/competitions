#include "common/factorization/primes_count_quotients.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

// Time: O(N^(3/4))
// Memory: O(N^(1/2))
int main_668() {
  uint64_t N = PowU<uint64_t>(10, 10), s = N;
  factorization::PrimesCountQuotients pcq(N);
  for (uint64_t p = 2; p <= pcq.nsqrt; ++p) {
    if (pcq.vcr[p] != pcq.vcr[p - 1]) s -= p;
  }
  for (uint64_t q = 1; q <= pcq.nsqrt2; ++q) {
    s -= q * (pcq.GetQ(q) - pcq.GetQ(q + 1));
  }
  std::cout << s << std::endl;
  return 0;
}
