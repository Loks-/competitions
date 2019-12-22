#include "common/factorization/primes_range.h"
#include "common/stl/base.h"

// Time: O(N * log(log(N)))
// Memory: O(N^(1/2))
int main_668() {
  uint64_t N = 10000000000, s = N;
  for (uint64_t p : factorization::PrimesRange(N)) s -= std::min(p, N / p);
  std::cout << s << std::endl;
  return 0;
}
