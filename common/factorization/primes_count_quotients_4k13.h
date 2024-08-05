#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"

#include <vector>

namespace factorization {
// Similar to PrimesCountQuotients but count primes 4k+1 and 4k+3
// independently
class PrimesCountQuotients4k13 {
 public:
  uint64_t n, nsqrt, nsqrt2;
  std::vector<uint64_t> vcr1, vcr3, vcq1, vcq3;

  explicit PrimesCountQuotients4k13(uint64_t _n) { Init(_n); }

  void Init(uint64_t _n) {
    n = _n;
    nsqrt = USqrt(n);
    nsqrt2 = n / (nsqrt + 1);
    vcr1.resize(nsqrt + 1);
    vcr3.resize(nsqrt + 1);
    vcq1.resize(nsqrt2 + 1);
    vcq3.resize(nsqrt2 + 1);
    for (uint64_t i = 1; i <= nsqrt; ++i) {
      vcr1[i] = (i - 1) / 4;
      vcr3[i] = (i + 1) / 4;
    }
    for (uint64_t j = 0; j < nsqrt2; ++j) {
      uint64_t i = n / (j + 1);
      vcq1[j] = (i - 1) / 4;
      vcq3[j] = (i + 1) / 4;
    }
    for (uint64_t p = 3; p <= nsqrt; p += 2) {
      if (vcr1[p] != vcr1[p - 1]) {
        uint64_t vp1 = vcr1[p - 1], vp3 = vcr3[p - 1], p2 = p * p;
        for (uint64_t j = 0; j < nsqrt2; ++j) {
          if (n < (j + 1) * p2) break;
          uint64_t l = (j + 1) * p;
          vcq1[j] -= ((l <= nsqrt2) ? vcq1[l - 1] : vcr1[n / l]) - vp1;
          vcq3[j] -= ((l <= nsqrt2) ? vcq3[l - 1] : vcr3[n / l]) - vp3;
        }
        for (uint64_t i = nsqrt; i >= p2; --i) {
          vcr1[i] -= vcr1[i / p] - vp1;
          vcr3[i] -= vcr3[i / p] - vp3;
        }
      } else if (vcr3[p] != vcr3[p - 1]) {
        uint64_t vp1 = vcr1[p - 1], vp3 = vcr3[p - 1], p2 = p * p;
        for (uint64_t j = 0; j < nsqrt2; ++j) {
          if (n < (j + 1) * p2) break;
          uint64_t l = (j + 1) * p;
          vcq3[j] -= ((l <= nsqrt2) ? vcq1[l - 1] : vcr1[n / l]) - vp1;
          vcq1[j] -= ((l <= nsqrt2) ? vcq3[l - 1] : vcr3[n / l]) - vp3;
        }
        for (uint64_t i = nsqrt; i >= p2; --i) {
          vcr3[i] -= vcr1[i / p] - vp1;
          vcr1[i] -= vcr3[i / p] - vp3;
        }
      }
    }
    vcq1[nsqrt2] = vcr1[nsqrt];
    vcq3[nsqrt2] = vcr3[nsqrt];
  }

  uint64_t GetQ1(uint64_t q) const {
    return (q <= nsqrt2) ? vcq1[q - 1] : vcr1[n / q];
  }

  uint64_t GetQ3(uint64_t q) const {
    return (q <= nsqrt2) ? vcq3[q - 1] : vcr3[n / q];
  }

  uint64_t GetR1(uint64_t r) const {
    if (r <= nsqrt) return vcr1[r];
    uint64_t q = n / r;
    assert((n / q) == r);
    return vcq1[q - 1];
  }

  uint64_t GetR3(uint64_t r) const {
    if (r <= nsqrt) return vcr3[r];
    uint64_t q = n / r;
    assert((n / q) == r);
    return vcq3[q - 1];
  }
};
}  // namespace factorization
