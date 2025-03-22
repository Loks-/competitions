#pragma once

#include "common/base.h"
#include "common/factorization/utils/factorization_base.h"
#include "common/modular/proxy/factorial.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/modular_proxy.h"
#include "common/numeric/utils/pow.h"

#include <tuple>
#include <vector>

// Calculation Binomial coefficients by any modular.
// Based on Andrew Granville paper.
namespace modular {
namespace proxy {
class BinomialCoefficient {
 public:
  using TPrimeFactorialProxy = Factorial<ModularProxyPrime32, true, false>;
  using TPrimePowerFactorialProxy =
      Factorial<ModularProxyComposite32, true, true>;

  static constexpr uint64_t GetPPower(uint64_t n, uint64_t p) {
    uint64_t s = 0;
    for (n /= p; n; n /= p) s += n;
    return s;
  }

  static constexpr uint64_t GetPPower(uint64_t n, uint64_t k, uint64_t p) {
    assert(n >= k);
    return GetPPower(n, p) - GetPPower(k, p) - GetPPower(n - k, p);
  }

  static constexpr uint64_t GetPPower(uint64_t n, uint64_t k, uint64_t l,
                                      uint64_t p) {
    assert(n >= k);
    return GetPPower(n, p) - GetPPower(k, p) - GetPPower(l, p);
  }

  static constexpr uint64_t Apply(uint64_t n, uint64_t k,
                                  TPrimeFactorialProxy& fp) {
    const auto& mp = fp.GetProxy();
    const uint64_t p = mp.GetMod();
    const uint64_t ppower = GetPPower(n, k, p);
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

  static constexpr uint64_t Apply(uint64_t n, uint64_t k, uint64_t p,
                                  unsigned q, TPrimePowerFactorialProxy& fp) {
    const auto& mp = fp.GetProxy();
    const uint64_t pq = mp.GetMod();
    const uint64_t ppower_adj =
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
  explicit BinomialCoefficient(uint64_t n) {
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

  uint64_t Apply(uint64_t n, uint64_t k) {
    if (n < k) return 0;
    uint64_t m = 1, mr = 0;
    for (auto& fp : factorial_proxies_prime) {
      const uint64_t p = fp.GetProxy().GetMod();
      const uint64_t pr = ((GetPPower(n, k, p) > 0) ? 0 : Apply(n, k, fp));
      mr = MergeRemainders<TArithmetic_P32U>(m, mr, p, pr);
      m *= p;
    }
    for (auto& t : factorial_proxies_composite) {
      const uint64_t p = std::get<0>(t);
      const unsigned q = std::get<1>(t);
      TPrimePowerFactorialProxy& fp = std::get<2>(t);
      const auto& mp = fp.GetProxy();
      const uint64_t pq = mp.GetMod();
      const uint64_t ppower = GetPPower(n, k, p);
      uint64_t pqr = Apply(n, k, p, q, fp);
      if (ppower) pqr = mp.Mult(pqr, mp.PowU(p, ppower));
      mr = MergeRemainders<TArithmetic_C32U>(m, mr, pq, pqr);
      m *= pq;
    }
    return mr;
  }
};
}  // namespace proxy
}  // namespace modular
