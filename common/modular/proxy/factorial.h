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
    unsigned k = vf.size();
    if (k > n) return;
    vf.resize(n + 1);
    for (unsigned i = k; i <= n; ++i)
      vf[i] = (coprime_only && (GCD<uint64_t>(proxy.GetMod(), i) > 1))
                  ? vf[i - 1]
                  : proxy.Mult(proxy.ApplyU(i), vf[i - 1]);
    if (keep_inverted) {
      // Assume vf[i] coprime to mod.
      vfi.resize(n + 1);
      vfi[n] = proxy.Inverse(vf[n]);
      for (unsigned i = n; i > k; --i)
        vfi[i - 1] = (coprime_only && (GCD<uint64_t>(proxy.GetMod(), i) > 1))
                         ? vfi[i]
                         : proxy.Mult(proxy.ApplyU(i), vfi[i]);
    }
    // for (; vf.size() <= n;) {
    //   vf.push_back(
    //       (coprime_only && (GCD<uint64_t>(proxy.GetMod(), vf.size()) > 1))
    //           ? vf.back()
    //           : proxy.Mult(proxy.ApplyU(vf.size()), vf.back()));
    //   if (keep_inverted) vfi.push_back(proxy.Inverse(vf.back()));
    // }
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
