#pragma once

#include "common/modular/proxy/proxy.h"
#include "common/numeric/complex.h"

namespace modular {
namespace proxy {
template <bool is_modular_prime = true>
class Complex {
 public:
  using TValue = int64_t;
  using TComplex = numeric::Complex<TValue>;
  using TProxy = Proxy<is_modular_prime, true, TValue>;

 protected:
  TProxy proxy;

 public:
  constexpr explicit Complex(TValue mod = 1000000007) : proxy(mod) {}

  constexpr explicit Complex(const TProxy& _proxy) : proxy(_proxy) {}

  constexpr TProxy GetProxy() const { return proxy; }

  constexpr void SetProxy(const TProxy& _proxy) { proxy = _proxy; }

  constexpr TValue GetMod() const { return proxy.GetMod(); }

  constexpr void SetMod(TValue mod) { proxy.SetMod(mod); }

  constexpr TComplex Add(const TComplex& l, const TComplex& r) const {
    const TComplex t = l + r;
    return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));
  }

  constexpr TComplex Sub(const TComplex& l, const TComplex& r) const {
    const TComplex t = l - r;
    return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));
  }

  constexpr TComplex Minus(const TComplex& l) const {
    const TComplex t = -l;
    return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));
  }

  constexpr TComplex Mult(const TComplex& l, const TComplex& r) const {
    const TComplex t = l * r;
    return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));
  }

  constexpr TComplex Sqr(const TComplex& l) const {
    const TComplex t = l * l;
    return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));
  }

  constexpr TComplex Div(const TComplex& l, const TComplex& r) const {
    const TComplex t = Mult(l, r.Conjugate());
    const TValue x = proxy.ApplyT(r.AbsSquared());
    return TComplex(proxy.Div(t.re, x), proxy.Div(t.im, x));
  }

  constexpr TComplex Inverse(const TComplex& l) const {
    const TValue x = proxy.ApplyT(l.AbsSquared());
    return TComplex(proxy.Div(l.re, x), proxy.Div(proxy.Minus(l.im), x));
  }

  constexpr TComplex PowU(TComplex x, uint64_t pow) const {
    TComplex ans = TComplex(1, 0);
    for (; pow; pow >>= 1) {
      if (pow & 1) ans = Mult(ans, x);
      x = Sqr(x);
    }
    return ans;
  }

  constexpr TComplex PowS(const TComplex& x, int64_t pow) {
    return (pow < 0) ? PowU(Inverse(x), -pow) : PowU(x, pow);
  }
};
}  // namespace proxy
}  // namespace modular
