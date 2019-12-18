#pragma once

#include "common/base.h"
#include "common/numeric/utils/usqrt.h"
#include <vector>

namespace factorization {
// Segmented Sieve of Eratosthenes
// https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
class PrimesGenerator {
 protected:
  const uint64_t default_block_size = 262144;
  uint64_t block_size, half_block_size, current_block;
  std::vector<bool> vmask;
  std::vector<uint64_t> first_block_primes, current_block_primes;
  uint64_t current_prime_block_index, current_prime_global_index;

 protected:
  void SetBlockSize(uint64_t new_block_size) {
    assert(new_block_size > 0);
    block_size = new_block_size + (new_block_size & 1);
    half_block_size = block_size / 2;
    current_block = 0;
    first_block_primes.clear();
    current_block_primes.clear();
    current_block_primes.push_back(2);
    current_prime_block_index = 0;
    current_prime_global_index = 0;
  }

  void FirstBlock() {
    vmask.clear();
    vmask.resize(half_block_size, false);
    for (uint64_t hi = 1; hi < half_block_size; ++hi) {
      if (!vmask[hi]) {
        uint64_t i = 2 * hi + 1;
        first_block_primes.push_back(i);
        for (uint64_t hj = 2 * hi * (hi + 1); hj < half_block_size; hj += i)
          vmask[hj] = true;
      }
    }
    current_block_primes = first_block_primes;
    current_block = 1;
  }

  void NextBlock() {
    assert(current_block < block_size);
    uint64_t b = current_block * block_size, e = b + block_size;
    std::fill(vmask.begin(), vmask.end(), false);
    for (uint64_t p : first_block_primes) {
      if (p * p >= e) break;
      uint64_t k = ((b - 1) / p + 1) | 1;
      if (k < p) k = p;
      for (uint64_t hj = (p * k - b) / 2; hj < half_block_size; hj += p)
        vmask[hj] = true;
    }
    current_block_primes.clear();
    for (uint64_t hj = 0; hj < half_block_size; ++hj) {
      if (!vmask[hj]) current_block_primes.push_back(2 * hj + 1 + b);
    }
    ++current_block;
  }

 public:
  PrimesGenerator() { SetBlockSize(default_block_size); }
  PrimesGenerator(uint64_t _block_size) { SetBlockSize(_block_size); }

  uint64_t Get() const {
    assert(current_prime_block_index < current_block_primes.size());
    return current_block_primes[current_prime_block_index];
  }

  uint64_t GetPrimeIndex() const { return current_prime_global_index; }

  void Next() {
    if (++current_prime_block_index == current_block_primes.size()) {
      if (current_block == 0)
        FirstBlock();
      else
        NextBlock();
      current_prime_block_index = 0;
    }
    ++current_prime_global_index;
  }

  uint64_t GetNext() {
    Next();
    return Get();
  }
};
}  // namespace factorization

inline std::vector<uint64_t> GeneratePrimes(uint64_t maxn) {
  factorization::PrimesGenerator pg;
  std::vector<uint64_t> vprimes;
  for (uint64_t p = pg.Get(); p <= maxn; p = pg.GetNext()) vprimes.push_back(p);
  return vprimes;
}
