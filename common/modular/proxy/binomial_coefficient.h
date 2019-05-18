#pragma once

#include "common/base.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/proxy/factorial.h"
#include "common/modular/proxy/modular_proxy.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/utils/pow.h"
#include <tuple>
#include <vector>

// Calculation Binomial coefficients by any modular.
// Based on Andrew Granville paper.
class PModularBinomialCoefficient {
 public:
  using TPrimeFactorialProxy =
      PModularFactorial<TModularProxy_P32U, true, false>;
  using TPrimePowerFactorialProxy =
      PModularFactorial<TModularProxy_C32U, true, true>;

  static uint64_t GetPPower(uint64_t n, uint64_t p) {
    uint64_t s = 0;
    for (n /= p; n; n /= p) s += n;
    return s;
  }

  static uint64_t GetPPower(uint64_t n, uint64_t k, uint64_t p) {
    assert(n >= k);
    return GetPPower(n, p) - GetPPower(k, p) - GetPPower(n - k, p);
  }

  static uint64_t GetPPower(uint64_t n, uint64_t k, uint64_t l, uint64_t p) {
    assert(n >= k);
    return GetPPower(n, p) - GetPPower(k, p) - GetPPower(l, p);
  }

  static uint64_t BinomialCoefficient(uint64_t n, uint64_t k,
                                      TPrimeFactorialProxy& fp) {
    const auto& mp = fp.GetProxy();
    uint64_t p = mp.GetMod();
    uint64_t ppower = GetPPower(n, k, p);
    uint64_t r = ((ppower & 1) ? p - 1 : 1);
    uint64_t l = n - k;
    for (; n;) {
      r = mp.Mult(r, mp.Mult(fp.Get(unsigned(n % p)),
                             mp.Mult(fp.GetI(unsigned(k % p)),
                                     fp.GetI(unsigned(l % p)))));
      n /= p;
      k /= p;
      l /= p;
    }
    return r;
  }

  static uint64_t BinomialCoefficient(uint64_t n, uint64_t k, uint64_t p,
                                      unsigned q,
                                      TPrimePowerFactorialProxy& fp) {
    const auto& mp = fp.GetProxy();
    uint64_t pq = mp.GetMod();
    uint64_t ppower_adj =
        GetPPower((n * p) / pq, (k * p) / pq, ((n - k) * p) / pq, p);
    uint64_t r = (((p == 2) && (q >= 3)) ? 1 : ((ppower_adj & 1) ? pq - 1 : 1));
    uint64_t l = n - k;
    for (; n;) {
      r = mp.Mult(r, mp.Mult(fp.Get(unsigned(n % pq)),
                             mp.Mult(fp.GetI(unsigned(k % pq)),
                                     fp.GetI(unsigned(l % pq)))));
      n /= p;
      k /= p;
      l /= p;
    }
    return r;
  }

 protected:
  std::vector<TPrimeFactorialProxy> factorial_proxies_prime;
  std::vector<std::tuple<uint64_t, unsigned, TPrimePowerFactorialProxy>>
      factorial_proxies_composite;

 public:
  PModularBinomialCoefficient(uint64_t n) {
    TFactorization nf(FactorizeBase(n));
    for (auto fp : nf) {
      if (fp.power == 1)
        factorial_proxies_prime.push_back(TPrimeFactorialProxy(fp.prime));
      else
        factorial_proxies_composite.push_back(std::make_tuple(
            fp.prime, fp.power,
            TPrimePowerFactorialProxy(PowU(fp.prime, fp.power))));
    }
  }

  uint64_t BinomialCoefficient(uint64_t n, uint64_t k) {
    if (n < k) return 0;
    uint64_t m = 1, mr = 0;
    for (auto& fp : factorial_proxies_prime) {
      uint64_t p = fp.GetProxy().GetMod();
      uint64_t pr =
          ((GetPPower(n, k, p) > 0) ? 0 : BinomialCoefficient(n, k, fp));
      mr = MergeRemainders<TModularArithmetic_P32U>(m, mr, p, pr);
      m *= p;
    }
    for (auto& t : factorial_proxies_composite) {
      uint64_t p = std::get<0>(t);
      unsigned q = std::get<1>(t);
      TPrimePowerFactorialProxy& fp = std::get<2>(t);
      const auto& mp = fp.GetProxy();
      uint64_t pq = mp.GetMod();
      uint64_t ppower = GetPPower(n, k, p);
      uint64_t pqr = BinomialCoefficient(n, k, p, q, fp);
      if (ppower) pqr = mp.Mult(pqr, mp.PowU(p, ppower));
      mr = MergeRemainders<TModularArithmetic_C32U>(m, mr, pq, pqr);
      m *= pq;
    }
    return mr;
  }
};
