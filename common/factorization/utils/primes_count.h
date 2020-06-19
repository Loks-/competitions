#pragma once

#include "common/base.h"
#include "common/binary_indexed_tree/bit.h"
#include "common/factorization/table/mobius.h"
#include "common/factorization/table/pi.h"
#include "common/numeric/utils/pow.h"
#include "common/numeric/utils/ucbrt.h"
#include "common/numeric/utils/ulog2.h"
#include "common/numeric/utils/usqrt.h"

#include <algorithm>
#include <vector>

// Based on Deleglise and Rivat paper
// https://www.ams.org/journals/mcom/1996-65-213/S0025-5718-96-00674-6/S0025-5718-96-00674-6.pdf
// Simplified version with O(n^(2/3)) memory.
inline uint64_t PrimesCount(uint64_t n) {
  if (n < 2) return 0;
  uint64_t n2 = USqrt(n), n3 = UCbrt(n), n4 = USqrt(n2), nl = ULog2(n),
           yr = n3 * PowU(nl, 3), y = std::max(n3, std::min(yr, n2)),
           ny = n / y, ny2 = USqrt(n / y);
  factorization::table::Mobius mobius(y);
  factorization::table::Pi pi(ny);
  auto primes = pi.GetPrimes();
  uint64_t py = pi(y), pn2 = pi(n2), pn3 = pi(n3), pn4 = pi(n4), pny2 = pi(ny2);

  int64_t s = ((pn2 - py + 1) * (pn2 + py - 2)) / 2;  // P2
  for (uint64_t i = py; i < pn2; ++i) s -= pi(n / primes[i]);
  for (uint64_t i = 1; i <= y; ++i) s += mobius(i) * (n / i);  // S0
  s += ((py - pn3) * (py - pn3 - 1)) / 2;                      // S1
  s += py * (pn3 - pny2);                                      // S2 U
  for (uint64_t i = pny2; i < pn3; ++i) s -= pi(n / primes[i] / primes[i]);
  for (uint64_t i = pn4; i < pn3; ++i)  // S2 V1
    s -= (i - 1ll) *
         (std::min<int64_t>(pi(n / primes[i] / primes[i]), py) - i - 1);
  for (uint64_t i = pn4; i < pn3; ++i) {  // S2 V2
    for (uint64_t j = i + 1,
                  je = std::min<uint64_t>(pi(n / primes[i] / primes[i]), py);
         j < je; ++j)
      s += pi(n / primes[i] / primes[j]);
  }

  // S3
  uint64_t vs_size = std::max(y, ny) + 1;
  std::vector<unsigned> vs(vs_size, 1);
  BIT<int64_t> bit(vs_size);
  for (uint64_t b = 0; b < pn4; ++b) {
    uint64_t p = primes[b], np = n / p;
    for (uint64_t ns = y / p + 1; ns <= y; ++ns) {
      if (vs[ns] && (ns % p))
        s -= mobius(ns) * (np / ns - bit.Sum(np / ns + 1));
    }
    vs[p] = 0;
    bit.Add(p);
    for (uint64_t m = p * p; m <= ny; m += p) {
      if (vs[m]) {
        vs[m] = 0;
        bit.Add(m);
      }
    }
  }
  return s;
}
