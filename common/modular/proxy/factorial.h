#pragma once

#include "common/base.h"
#include "common/numeric/utils/gcd.h"

#include <vector>

namespace modular {
namespace proxy {
template <class TProxy, bool keep_inverted = true, bool coprime_only = false>
class Factorial {
 public:
  using TValue = typename TProxy::TValue;

 protected:
  TProxy proxy;
  std::vector<TValue> vf, vfi;

 public:
  explicit Factorial(TValue mod) : proxy(mod) {
    vf.push_back(1);
    if (keep_inverted) vfi.push_back(1);
  }
  const TProxy& GetProxy() const { return proxy; }

  void Adjust(unsigned n) {
    for (; vf.size() <= n;) {
      vf.push_back(
          (coprime_only && (GCD<uint64_t>(proxy.GetMod(), vf.size()) > 1))
              ? vf.back()
              : proxy.Mult(proxy.ApplyU(vf.size()), vf.back()));
      if (keep_inverted) vfi.push_back(proxy.Inverse(vf.back()));
    }
  }

  TValue Get(unsigned n) {
    Adjust(n);
    return vf[n];
  }

  TValue GetI(unsigned n) {
    Adjust(n);
    return (keep_inverted ? vfi[n] : proxy.Inverse(vf[n]));
  }

  TValue operator()(unsigned n) { return Get(n); }

  TValue BinomialCoefficient(unsigned n, unsigned k) {
    if (k > n) return 0;
    Adjust(n);
    return (keep_inverted ? proxy.Mult(proxy.Mult(vf[n], vfi[k]), vfi[n - k])
                          : proxy.Div(vf[n], proxy.Mult(vf[k], vf[n - k])));
  }
};
}  // namespace proxy
}  // namespace modular
