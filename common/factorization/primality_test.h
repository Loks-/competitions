#pragma once

#include "primes_list.h"
#include "../base.h"
#include "../modular/arithmetic.h"
#include <vector>

class MillerRabinPrimalityTest {
 public:
  using TModularA = ModularArithmetic<false, false, uint64_t>;

  enum Primality {
    COMPOSITE,
    PROBABLY_PRIME,
  };

  // n - 1 == d * 2^s
  struct Factorization {
    uint64_t n;
    uint64_t s;
    uint64_t d;
  };

  static Factorization Factor(uint64_t n) {
    Factorization f;
    f.n = n;
    f.d = n - 1;
    f.s = 0;
    for (; ((~f.d) & 1); f.d >>= 1) ++f.s;
    return f;
  }

  static const std::vector<uint64_t>& SelectWitnesses(uint64_t n) {
    // Note: http://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
    // lists minimal sets of witnesses that are sufficient for deterministic
    // testing of primes for certain ranges of inputs. We've selected only
    // a few of those ranges; it may be worthwhile to add more ranges here.
    static const std::vector<uint64_t> w2k = {2};
    static const std::vector<uint64_t> w25M = {2, 3, 5};
    static const std::vector<uint64_t> w3T = {2, 3, 5, 7, 11, 13};
    static const std::vector<uint64_t> w341T = {2, 3, 5, 7, 11, 13, 17};
    static const std::vector<uint64_t> w3_1016 = {2,  3,  5,  7, 11,
                                                  13, 17, 19, 23};
    static const std::vector<uint64_t> wmax = {2,  3,  5,  7,  11, 13,
                                               17, 19, 23, 29, 31, 37};

    if (n < 2047ull)
      return w2k;
    else if (n < 25326001ull)
      return w25M;
    else if (n < 3474749660383ull)
      return w3T;
    else if (n < 341550071728321ull)
      return w341T;
    else if (n < 3825123056546413051ull)
      return w3_1016;
    else
      return wmax;
  }

  static Primality CheckMillerRabinWitness(const Factorization& f,
                                           uint64_t witness) {
    uint64_t x = TModularA::PowU(witness, f.d, f.n);
    if ((x == 1) || (x == (f.n - 1))) return PROBABLY_PRIME;
    for (uint64_t i = 0; i < f.s - 1; ++i) {
      x = TModularA::Mult(x, x, f.n);
      if (x == 1) return COMPOSITE;
      if (x == f.n - 1) return PROBABLY_PRIME;
    }
    return COMPOSITE;
  }

  static Primality RunMillerRabinTest(uint64_t n) {
    if ((~n) & 1) return COMPOSITE;
    Factorization f = Factor(n);
    const std::vector<uint64_t>& witnesses = SelectWitnesses(n);
    for (size_t i = 0; i < witnesses.size(); ++i) {
      uint64_t witness = witnesses[i];
      if (CheckMillerRabinWitness(f, witness) == COMPOSITE) return COMPOSITE;
    }
    return PROBABLY_PRIME;
  }

 private:
  uint64_t N, M;
  std::vector<uint8_t> primes;
  std::vector<uint8_t> table;

 public:
  MillerRabinPrimalityTest(unsigned maxprime = 13) {
    N = maxprime, M = 1;
    PrimesList primes_list(maxprime);
    const std::vector<uint64_t>& pl_primes = primes_list.GetPrimes();
    primes.resize(maxprime + 1, 0);
    for (uint64_t p : pl_primes) {
      M *= p;
      primes[p] = 1;
    }
    table.resize(M, 1);
    for (uint64_t p : pl_primes) {
      for (uint64_t j = 0; j < M; j += p) table[j] = 0;
    }
  }

  bool IsPrime(uint64_t n) const {
    if (n <= N) return (primes[n] != 0);
    if (!table[n % M]) return false;
    return (RunMillerRabinTest(n) == PROBABLY_PRIME);
  }
};
