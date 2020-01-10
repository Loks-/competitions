#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

namespace factorization {
class PrimesCountQuotients {
 public:
  uint64_t n, nsqrt, nsqrt2;
  std::vector<uint64_t> vcr, vcq;

  PrimesCountQuotients(uint64_t _n) { Init(_n); }

  void Init(uint64_t _n) {
    n = _n;
    nsqrt = USqrt(n);
    nsqrt2 = n / (nsqrt + 1);
    vcr.resize(nsqrt + 1);
    vcq.resize(nsqrt2 + 1);
    for (uint64_t i = 1; i <= nsqrt; ++i) vcr[i] = i - 1;
    for (uint64_t j = 0; j < nsqrt2; ++j) vcq[j] = n / (j + 1) - 1;
    for (uint64_t p = 2; p <= nsqrt; ++p) {
      if (vcr[p] != vcr[p - 1]) {
        uint64_t vp = vcr[p - 1], p2 = p * p;
        for (uint64_t j = 0; j < nsqrt2; ++j) {
          if (n < (j + 1) * p2) break;
          uint64_t l = (j + 1) * p;
          vcq[j] -= ((l <= nsqrt2) ? vcq[l - 1] : vcr[n / l]) - vp;
        }
        for (uint64_t i = nsqrt; i >= p2; --i) {
          vcr[i] -= vcr[i / p] - vp;
        }
      }
    }
    vcq[nsqrt] = vcr[nsqrt];
  }

  uint64_t GetQ(uint64_t q) const {
    return (q <= nsqrt2) ? vcq[q - 1] : vcr[n / q];
  }

  uint64_t GetR(uint64_t r) const {
    if (r <= nsqrt) return vcr[r];
    uint64_t q = n / r;
    assert((n / q) == r);
    return vcq[q - 1];
  }
};
}  // namespace factorization
