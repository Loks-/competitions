#include "tester/prime_generation.h"

#include "common/base.h"
#include "common/factorization/primality_test.h"
#include "common/factorization/primes_list.h"
#include <vector>

std::vector<uint64_t> GetPrimes_PrimalityTest(uint64_t maxn) {
  std::vector<uint64_t> vp;
  if (maxn >= 2) vp.push_back(2);
  for (uint64_t i = 3; i <= maxn; i += 2) {
    if (IsPrime(i)) vp.push_back(i);
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_PrimesList(uint64_t maxn) {
  return factorization::PrimesList(maxn).GetPrimes();
}

std::vector<uint64_t> GetPrimes_EratosthenesBit(uint64_t maxn) {
  std::vector<uint64_t> vp;
  std::vector<bool> vb(maxn + 1, true);
  for (uint64_t i = 2; i <= maxn; ++i) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += i) {
        vb[j] = false;
      }
    }
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
  for (uint64_t i = 3; i <= maxn; i += 2) {
    if (vb[i]) {
      vp.push_back(i);
      for (uint64_t j = i * i; j <= maxn; j += 2 * i) {
        vb[j] = false;
      }
    }
  }
  return vp;
}

std::vector<uint64_t> GetPrimes_EratosthenesOddMemoryReduced(uint64_t maxn) {
  if (maxn < 2) return {};
  std::vector<uint64_t> vp{2};
  uint64_t hn = (maxn - 1) / 2;
  std::vector<bool> vb(hn + 1, true);
  for (uint64_t hi = 1; hi <= hn; ++hi) {
    if (vb[hi]) {
      uint64_t i = 2 * hi + 1;
      vp.push_back(i);
      for (uint64_t hj = 2 * hi * (hi + 1); hj <= hn; hj += i) {
        vb[hj] = false;
      }
    }
  }
  return vp;
}
