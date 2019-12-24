#include "tester/prime_generation.h"

#include "common/base.h"
#include "common/factorization/primality_test.h"
#include "common/factorization/table/primes.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

std::vector<uint64_t> GetPrimes_TrialDivision(uint64_t maxn) {
  std::vector<uint64_t> vp;
  if (maxn >= 2) vp.push_back(2);
  for (uint64_t i = 3; i <= maxn; i += 2) {
    bool prime = true;
    for (uint64_t p : vp) {
      if (p * p > i) break;
      if ((i % p) == 0) {
        prime = false;
        break;
      }
    }
    if (prime) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_PrimalityTest(uint64_t maxn) {
  std::vector<uint64_t> vp;
  if (maxn >= 2) vp.push_back(2);
  for (uint64_t i = 3; i <= maxn; i += 2) {
    if (IsPrime(i)) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_PrimesTable(uint64_t maxn) {
  return PrimesTable(maxn).GetPrimes();
}

std::vector<uint64_t> GetPrimes_EratosthenesBit(uint64_t maxn) {
  std::vector<uint64_t> vp;
  std::vector<bool> vb(maxn + 1, true);
  uint64_t sqrtn = USqrt(maxn), i = 2;
  for (; i <= sqrtn; ++i) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += i) {
        vb[j] = false;
      }
    }
  }
  for (; i <= maxn; ++i) {
    if (vb[i]) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesByte(uint64_t maxn) {
  std::vector<uint64_t> vp;
  std::vector<uint8_t> vb(maxn + 1, 1);
  for (uint64_t i = 2; i <= maxn; ++i) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += i) {
        vb[j] = 0;
      }
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesInt(uint64_t maxn) {
  std::vector<uint64_t> vp;
  std::vector<int> vb(maxn + 1, 1);
  for (uint64_t i = 2; i <= maxn; ++i) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += i) {
        vb[j] = 0;
      }
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesOdd(uint64_t maxn) {
  if (maxn < 2) return {};
  std::vector<uint64_t> vp{2};
  std::vector<bool> vb(maxn + 1, true);
  uint64_t sqrtn = USqrt(maxn), i = 3;
  for (; i <= sqrtn; i += 2) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += 2 * i) {
        vb[j] = false;
      }
    }
  }
  for (; i <= maxn; i += 2) {
    if (vb[i]) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesOddMemoryReduced(uint64_t maxn) {
  if (maxn < 2) return {};
  std::vector<uint64_t> vp{2};
  uint64_t hn = (maxn - 1) / 2;
  std::vector<bool> vb(hn + 1, true);
  uint64_t sqrtn = USqrt(maxn), hs = (sqrtn - 1) / 2, hi = 1;
  for (; hi <= hs; ++hi) {
    if (vb[hi]) {
      uint64_t i = 2 * hi + 1;
      vp.push_back(i);
      for (uint64_t hj = 2 * hi * (hi + 1); hj <= hn; hj += i) {
        vb[hj] = false;
      }
    }
  }
  for (; hi <= hn; ++hi) {
    if (vb[hi]) vp.push_back(2 * hi + 1);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesSegmented(uint64_t maxn) {
  if (maxn < 2) return {};
  std::vector<uint64_t> vp{2};
  uint64_t block_size = 2 * (USqrt(maxn / 2) + 1);
  uint64_t blocks = (maxn - 1) / block_size + 1;
  uint64_t lbs = block_size / 2;
  std::vector<bool> vb(lbs, true);
  for (uint64_t hi = 1; hi < lbs; ++hi) {
    if (vb[hi]) {
      uint64_t i = 2 * hi + 1;
      vp.push_back(i);
      for (uint64_t hj = 2 * hi * (hi + 1); hj < lbs; hj += i) {
        vb[hj] = false;
      }
    }
  }
  for (uint64_t ib = 1; ib < blocks; ++ib) {
    uint64_t b = ib * block_size, e = b + block_size;
    std::fill(vb.begin(), vb.end(), true);
    for (unsigned ip = 1;; ++ip) {
      uint64_t p = vp[ip];
      if (p * p >= e) break;
      uint64_t k = ((b - 1) / p + 1) | 1;
      if (k < p) k = p;
      for (uint64_t hj = (p * k - b) / 2; hj < lbs; hj += p) {
        vb[hj] = false;
      }
    }
    for (uint64_t hj = 0; hj < lbs; ++hj) {
      if (vb[hj]) vp.push_back(2 * hj + 1 + b);
    }
  }
  while (vp.back() > maxn) vp.pop_back();
  return vp;
}

std::vector<uint64_t> GetPrimes_AtkinBit(uint64_t maxn) {
  std::vector<uint64_t> vp{2, 3, 5};
  if (maxn < 7) {
    while (!vp.empty() && (vp.back() > maxn)) vp.pop_back();
    return vp;
  }
  std::vector<bool> vb(maxn + 1, false);
  for (uint64_t x = 1;; ++x) {
    uint64_t nx = 4 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 1;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 1;; x += 2) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 2;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 2;; ++x) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn + x * x) break;
    for (int64_t y = x - 1;; y -= 2) {
      uint64_t ny = nx - y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
      if (y < 3) break;
    }
  }
  for (uint64_t i = 7; i <= maxn; i += 2) {
    if (vb[i] && (i % 3) && (i % 5)) {
      vp.push_back(i);
      uint64_t x = i * i;
      for (uint64_t j = x; j <= maxn; j += 2 * x) vb[j] = false;
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_AtkinByte(uint64_t maxn) {
  std::vector<uint64_t> vp{2, 3, 5};
  if (maxn < 7) {
    while (!vp.empty() && (vp.back() > maxn)) vp.pop_back();
    return vp;
  }
  std::vector<uint8_t> vb(maxn + 1, 0);
  for (uint64_t x = 1;; ++x) {
    uint64_t nx = 4 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 1;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 1;; x += 2) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 2;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 2;; ++x) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn + x * x) break;
    for (int64_t y = x - 1;; y -= 2) {
      uint64_t ny = nx - y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
      if (y < 3) break;
    }
  }
  for (uint64_t i = 7; i <= maxn; i += 2) {
    if (vb[i] && (i % 3) && (i % 5)) {
      vp.push_back(i);
      uint64_t x = i * i;
      for (uint64_t j = x; j <= maxn; j += 2 * x) vb[j] = 0;
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_AtkinInt(uint64_t maxn) {
  std::vector<uint64_t> vp{2, 3, 5};
  if (maxn < 7) {
    while (!vp.empty() && (vp.back() > maxn)) vp.pop_back();
    return vp;
  }
  std::vector<int> vb(maxn + 1, 0);
  for (uint64_t x = 1;; ++x) {
    uint64_t nx = 4 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 1;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 1;; x += 2) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn) break;
    for (int64_t y = 2;; y += 2) {
      uint64_t ny = nx + y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
    }
  }
  for (uint64_t x = 2;; ++x) {
    uint64_t nx = 3 * x * x;
    if (nx > maxn + x * x) break;
    for (int64_t y = x - 1;; y -= 2) {
      uint64_t ny = nx - y * y;
      if (ny > maxn) break;
      vb[ny] = !vb[ny];
      if (y < 3) break;
    }
  }
  for (uint64_t i = 7; i <= maxn; i += 2) {
    if (vb[i] && (i % 3) && (i % 5)) {
      vp.push_back(i);
      uint64_t x = i * i;
      for (uint64_t j = x; j <= maxn; j += 2 * x) vb[j] = 0;
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_AtkinMemoryReduced(uint64_t maxn) {
  std::vector<uint64_t> vp{2, 3, 5};
  if (maxn < 7) {
    while (!vp.empty() && (vp.back() > maxn)) vp.pop_back();
    return vp;
  }
  uint64_t hn = (maxn - 1) / 2;
  std::vector<bool> vb(hn + 1, false);
  for (uint64_t x = 1;; ++x) {
    uint64_t hi = 2 * x * x;
    if (hi > hn) break;
    for (int64_t y = 0; hi <= hn; hi += 4 * ++y) vb[hi] = !vb[hi];
  }
  for (uint64_t x = 1;; x += 2) {
    uint64_t hi = 3 * x * x / 2 + 2;
    if (hi > hn) break;
    for (int64_t y = 2; hi <= hn; hi += (y += 4)) vb[hi] = !vb[hi];
  }
  for (uint64_t x = 2;; ++x) {
    uint64_t hi = x * (x + 1) - 1;
    if (hi > hn) break;
    for (int64_t y = 2 * x; (hi <= hn) && (y > 2); hi += (y -= 4))
      vb[hi] = !vb[hi];
  }
  // It's possible to speed up this loop with wheel, but it's take
  // only 20-25% time.
  for (uint64_t hi = 3; hi <= hn; ++hi) {
    uint64_t i = 2 * hi + 1;
    if (vb[hi] && (i % 3) && (i % 5)) {
      vp.push_back(i);
      uint64_t x = i * i;
      for (uint64_t hj = x / 2; hj <= hn; hj += x) vb[hj] = false;
    }
  }
  return vp;
}
