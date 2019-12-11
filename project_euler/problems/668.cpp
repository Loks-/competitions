#include "common/factorization/primes_range.h"
#include "common/stl/base.h"

int main_668() {
  uint64_t n = 10000000000, s = n;
  for (uint64_t p : factorization::PrimesRange(n)) s -= std::min(p, n / p);
  std::cout << s << std::endl;
  return 0;
}
