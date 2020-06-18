#include "tester/primes_count.h"

#include "common/assert_exception.h"
#include "common/binary_indexed_tree/bit.h"
#include "common/factorization/primes_count_quotients.h"
#include "common/factorization/primes_generator.h"
#include "common/factorization/primes_range.h"
#include "common/factorization/table/mobius.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/usqrt.h"
#include "common/template.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

uint64_t PrimesCount_Table(uint64_t n) {
  const std::vector<uint64_t> table{0,
                                    4,
                                    25,
                                    168,
                                    1229,
                                    9592,
                                    78498,
                                    664579,
                                    5761455,
                                    50847534,
                                    455052511,
                                    4118054813ull,
                                    37607912018ull,
                                    346065536839ull,
                                    3204941750802ull,
                                    29844570422669ull,
                                    279238341033925ull,
                                    2623557157654233ull,
                                    24739954287740860ull};
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
  for (auto x : factorization::PrimesRange(n)) {
    FakeUse(x);
    ++c;
  }
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

uint64_t PrimesCount_MeisselLehmer1(uint64_t n) {
  if (n < 2) return 0;
  uint64_t ncbrt = UCbrt(n), nsqrt = USqrt(n), ncbrt2 = n / ncbrt;
  auto primes = GeneratePrimes(ncbrt2);
  uint64_t k =
      std::upper_bound(primes.begin(), primes.end(), ncbrt) - primes.begin();
  uint64_t l =
      std::upper_bound(primes.begin(), primes.end(), nsqrt) - primes.begin();

  std::function<uint64_t(uint64_t, unsigned)> F = [&](uint64_t k,
                                                      unsigned i) -> uint64_t {
    if (k < primes[i]) return 1;
    uint64_t s = k - (k / 2);
    for (unsigned j = 1; (j < i) && (primes[j] <= k); ++j) {
      s -= F(k / primes[j], j);
    }
    return s;
  };

  uint64_t s = (k ? F(n, k) : n) + (l * (l - 1)) / 2 - ((k - 1) * (k - 2)) / 2;
  for (uint64_t i = k; i < l; ++i) {
    s -= std::upper_bound(primes.begin(), primes.end(), n / primes[i]) -
         primes.begin();
  }
  return s;
}

uint64_t PrimesCount_MeisselLehmerB(uint64_t n, unsigned b) {
  if (n < 2) return 0;
  uint64_t ncbrt = UCbrt(n), nsqrt = USqrt(n), ncbrt2 = n / ncbrt;
  auto primes = GeneratePrimes(ncbrt2);
  uint64_t k =
      std::upper_bound(primes.begin(), primes.end(), ncbrt) - primes.begin();
  uint64_t l =
      std::upper_bound(primes.begin(), primes.end(), nsqrt) - primes.begin();

  b = std::min(b, unsigned(k));
  uint64_t m = 1;
  for (unsigned i = 0; i < b; ++i) m *= primes[i];
  std::vector<std::vector<uint64_t>> vc(b + 1, std::vector<uint64_t>(m + 1, 0));
  for (uint64_t j = 1; j <= m; ++j) {
    vc[0][j] = j;
  }
  for (unsigned i = 1; i <= b; ++i) {
    uint64_t p = primes[i - 1];
    for (uint64_t j = 1; j <= m; ++j) {
      vc[i][j] = vc[i][j - 1] + ((j % p) && (vc[i - 1][j - 1] != vc[i - 1][j]));
    }
  }

  std::function<uint64_t(uint64_t, unsigned)> F = [&](uint64_t k,
                                                      unsigned i) -> uint64_t {
    if (k < primes[i]) return 1;
    if (i <= b) return (k / m) * vc[i].back() + vc[i][k % m];
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

// Based on Lagarias, Miller and Odlyzko paper
// https://www.ams.org/journals/mcom/1985-44-170/S0025-5718-1985-0777285-5/S0025-5718-1985-0777285-5.pdf
// Simplified version with O(n^(2/3)) memory.
uint64_t PrimesCount_ExtendedMeisselLehmer(uint64_t n) {
  if (n < 2) return 0;
  uint64_t ncbrt = UCbrt(n), nsqrt = USqrt(n), ncbrt2 = n / ncbrt;
  factorization::table::Mobius mobius(ncbrt);
  auto primes = GeneratePrimes(ncbrt2);
  uint64_t k =
      std::upper_bound(primes.begin(), primes.end(), ncbrt) - primes.begin();
  uint64_t l =
      std::upper_bound(primes.begin(), primes.end(), nsqrt) - primes.begin();

  int64_t s = (l * (l - 1)) / 2 - ((k - 1) * (k - 2)) / 2;
  for (uint64_t i = k; i < l; ++i) {
    s -= std::upper_bound(primes.begin(), primes.end(), n / primes[i]) -
         primes.begin();
  }
  for (uint64_t i = 1; i <= ncbrt; ++i) s += mobius(i) * (n / i);

  std::vector<unsigned> vs(ncbrt2 + 1, 1);
  BIT<int64_t> bit(ncbrt2 + 1);
  for (uint64_t b = 0; b < k; ++b) {
    uint64_t p = primes[b], np = n / p;
    for (uint64_t ns = ncbrt / p + 1; ns <= ncbrt; ++ns) {
      if (vs[ns] && (ns % p))
        s -= mobius(ns) * (np / ns - bit.Sum(np / ns + 1));
    }
    vs[p] = 0;
    bit.Add(p);
    for (uint64_t m = p * p; m <= ncbrt2; m += p) {
      if (vs[m]) {
        vs[m] = 0;
        bit.Add(m);
      }
    }
  }
  return s;
}

uint64_t PrimesCount_LucyHedgehogRecursive(uint64_t n) {
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

  std::function<uint64_t(uint64_t, unsigned)> Count =
      [&](uint64_t k, unsigned i) -> uint64_t {
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
