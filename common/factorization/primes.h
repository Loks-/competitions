#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

namespace factorization {
// Segmented Sieve of Eratosthenes
// https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
// TODO:
//   Compact primes representation.
class PrimesGenerator {
 protected:
  std::vector<uint64_t> vprimes;
  std::vector<bool> vmask;
  uint64_t block_size, half_block_size;
  uint64_t current_block;
  const uint64_t default_block_size = 262144;

 protected:
  void ResetBlockSize(uint64_t new_block_size) {
    assert(new_block_size > 0);
    vprimes.clear();
    vprimes.push_back(2);
    block_size = new_block_size + (new_block_size & 1);
    half_block_size = block_size / 2;
    vmask.clear();
    vmask.resize(half_block_size, false);
  }

 public:
  PrimesGenerator() : block_size(0) {}
  PrimesGenerator(uint64_t _block_size) { FirstBlock(_block_size); }

  const std::vector<uint64_t>& GetPrimes() const { return vprimes; }

  void FirstBlock(uint64_t new_block_size) {
    ResetBlockSize(new_block_size);
    for (uint64_t hi = 1; hi < half_block_size; ++hi) {
      if (!vmask[hi]) {
        uint64_t i = 2 * hi + 1;
        vprimes.push_back(i);
        for (uint64_t hj = 2 * hi * (hi + 1); hj < half_block_size; hj += i)
          vmask[hj] = true;
      }
    }
    current_block = 1;
  }

  void NextBlock() {
    uint64_t b = current_block * block_size, e = b + block_size;
    std::fill(vmask.begin(), vmask.end(), false);
    for (unsigned ip = 1;; ++ip) {
      uint64_t p = vprimes[ip];
      if (p * p >= e) break;
      uint64_t k = ((b - 1) / p + 1) | 1;
      if (k < p) k = p;
      for (uint64_t hj = (p * k - b) / 2; hj < half_block_size; hj += p)
        vmask[hj] = true;
    }
    for (uint64_t hj = 0; hj < half_block_size; ++hj) {
      if (!vmask[hj]) vprimes.push_back(2 * hj + 1 + b);
    }
    ++current_block;
  }

  std::vector<uint64_t> GeneratePrimes(uint64_t maxn) {
    if (block_size == 0) FirstBlock(default_block_size);
    for (; current_block * block_size < maxn;) NextBlock();
    std::vector<uint64_t> vp = vprimes;
    for (; !vp.empty() && (vp.back() > maxn);) vp.pop_back();
    return vp;
  }
};
}  // namespace factorization

static std::vector<uint64_t> GeneratePrimes(uint64_t maxn) {
  return factorization::PrimesGenerator().GeneratePrimes(maxn);
}
