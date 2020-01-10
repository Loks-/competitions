#include "tester/primes_count.h"

#include "common/assert_exception.h"
#include "common/factorization/primes_count_quotients.h"
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

  std::function<uint64_t(uint64_t, unsigned)> Count = [&](
      uint64_t k, unsigned i) -> uint64_t {
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

uint64_t PrimesCount_LucyHedgehogRecursive(uint64_t n) {
  uint64_t nsqrt = USqrt(n);
  auto primes = GeneratePrimes(nsqrt);

  std::function<uint64_t(uint64_t, unsigned)> Count = [&](
      uint64_t k, unsigned i) -> uint64_t {
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

// Add cache for Count(k, i) where p_i^2 > k.
uint64_t PrimesCount_LucyHedgehogRecursive2(uint64_t n) {
  uint64_t nsqrt = USqrt(n);
  auto primes = GeneratePrimes(nsqrt);
  std::vector<uint64_t> vc(nsqrt + 1, 0);
  uint64_t ii = 2;
  for (uint64_t p : primes) {
    for (; ii < p; ++ii) vc[ii] = vc[ii - 1];
    vc[ii] = vc[ii - 1] + 1;
    ++ii;
  }
  for (; ii <= nsqrt; ++ii) vc[ii] = vc[ii - 1];

  std::function<uint64_t(uint64_t, unsigned)> Count = [&](
      uint64_t k, unsigned i) -> uint64_t {
    if (i == 0) return k - 1;
    uint64_t pi = primes[i - 1];
    if ((k <= nsqrt) && (k < pi * pi)) return vc[k];
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

uint64_t PrimesCount_LucyHedgehogVector(uint64_t n) {
  uint64_t nsqrt = USqrt(n), nsqrti = n / (nsqrt + 1);
  std::vector<uint64_t> vs(nsqrt + 1), vl(nsqrti);
  for (uint64_t i = 1; i <= nsqrt; ++i) vs[i] = i - 1;
  for (uint64_t j = 0; j < nsqrti; ++j) vl[j] = n / (j + 1) - 1;
  for (uint64_t p = 2; p <= nsqrt; ++p) {
    if (vs[p] != vs[p - 1]) {
      uint64_t vp = vs[p - 1], p2 = p * p;
      for (uint64_t j = 0; j < nsqrti; ++j) {
        if (n < (j + 1) * p2) break;
        uint64_t l = (j + 1) * p;
        vl[j] -= ((l <= nsqrti) ? vl[l - 1] : vs[n / l]) - vp;
      }
      for (uint64_t i = nsqrt; i >= p2; --i) {
        vs[i] -= vs[i / p] - vp;
      }
    }
  }
  return vl[0];
}

uint64_t PrimesCount_PCQ(uint64_t n) {
  factorization::PrimesCountQuotients pcq(n);
  return pcq.GetQ(1);
}
