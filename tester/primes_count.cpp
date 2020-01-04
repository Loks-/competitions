#include "tester/primes_count.h"

#include "common/assert_exception.h"
#include "common/factorization/primes_generator.h"
#include "common/factorization/primes_range.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"
#include <algorithm>
#include <functional>
#include <vector>

#include <iostream>

uint64_t PrimesCount_Table(uint64_t n) {
  const std::vector<uint64_t> table{
      0,      4,       25,       168,       1229,          9592,          78498,
      664579, 5761455, 50847534, 455052511, 4118054813ull, 37607912018ull};
  uint64_t n10 = 1;
  for (unsigned i = 0; i < table.size(); ++i) {
    if (n == n10) return table[i];
    n10 *= 10;
  }
  Assert(false, "Unknown n for table");
  return 0;
}

uint64_t PrimesCount_PrimesGeneration(uint64_t n) {
  uint64_t c = 0;
  for (uint64_t p : factorization::PrimesRange(n)) ++c;
  return c;
}

uint64_t PrimesCount_Legendre(uint64_t n) {
  uint64_t nsqrt = USqrt(n);
  auto primes = GeneratePrimes(nsqrt);

  std::function<uint64_t(uint64_t, unsigned)> Count =
      [&](uint64_t k, unsigned i) -> uint64_t {
    uint64_t s = k;
    for (; i < primes.size(); ++i) {
      uint64_t p = primes[i], kp = k / p;
      if (kp == 0) break;
      s -= Count(kp, i + 1);
    }
    return s;
  };

  return Count(n, 0) + primes.size() - 1;
}

uint64_t PrimesCount_LucyHedgehog(uint64_t n) {
  uint64_t nsqrt = USqrt(n);
  auto primes = GeneratePrimes(nsqrt);

  std::function<uint64_t(uint64_t, unsigned)> Count =
      [&](uint64_t k, unsigned i) -> uint64_t {
    uint64_t s = k - 1;
    for (unsigned j = 0; j < i; ++j) {
      uint64_t p = primes[j], kp = k / p;
      if (kp < p) break;
      s -= Count(kp, j) - j;
    }
    return s;
  };

  return Count(n, primes.size());
}

uint64_t PrimesCount_Meissel(uint64_t n) {
  if (n < 2) return 0;
  uint64_t ncbrt = UCbrt(n), nsqrt = USqrt(n), ncbrt2 = n / ncbrt;
  auto primes = GeneratePrimes(ncbrt2);
  uint64_t k =
      std::upper_bound(primes.begin(), primes.end(), ncbrt) - primes.begin();
  uint64_t l =
      std::upper_bound(primes.begin(), primes.end(), nsqrt) - primes.begin();

  std::function<uint64_t(uint64_t, unsigned)> F = [&](uint64_t k,
                                                      unsigned i) -> uint64_t {
    uint64_t s = k;
    for (unsigned j = 0; (j < i) && (primes[j] <= k); ++j) {
      s -= F(k / primes[j], j);
    }
    return s;
  };

  uint64_t s = F(n, k) + (l * (l - 1)) / 2 - ((k - 1) * (k - 2)) / 2;
  for (uint64_t i = k; i < l; ++i) {
    s -= std::upper_bound(primes.begin(), primes.end(), n / primes[i]) -
         primes.begin();
  }
  return s;
}
