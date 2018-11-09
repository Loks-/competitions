#pragma once

#include "modular_proxy.h"
#include "../../numeric/complex.h"

template <bool is_modular_prime = true>
class PModularComplex
{
public:
	using TValue = int64_t;
	using TComplex = Complex<TValue>;
	using TModularProxy = ModularProxy<is_modular_prime, true, TValue>;

protected:
	TModularProxy proxy;

public:
	PModularComplex(TValue mod = 1000000007) : proxy(mod) {}
	PModularComplex(const TModularProxy& _proxy) : proxy(_proxy) {}

	TModularProxy GetProxy() const { return proxy; }
	void SetProxy(const TModularProxy& _proxy) { proxy = _proxy; }
	TValue GetMod() const { return proxy.GetMod(); }
	void SetMod(TValue mod) { proxy.SetMod(mod); }

	TComplex Add(const TComplex& l, const TComplex& r) const { TComplex t = l + r; return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im));	}
	TComplex Sub(const TComplex& l, const TComplex& r) const { TComplex t = l - r; return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im)); }
	TComplex Minus(const TComplex& l) const { TComplex t = -l; return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im)); }
	TComplex Mult(const TComplex& l, const TComplex& r) const { TComplex t = l * r; return TComplex(proxy.ApplyT(t.re), proxy.ApplyT(t.im)); }

	TComplex Div(const TComplex& l, const TComplex& r) const
	{
		TComplex t = Mult(l, r.Conjugate());
		TValue x = proxy.ApplyT(r.AbsSquared());
		return TComplex(proxy.Div(t.re, x), proxy.Div(t.im, x));
	}

	TComplex Inverse(const TComplex& l) const
	{
		TValue x = proxy.ApplyT(l.AbsSquared());
		return TComplex(proxy.Div(l.re, x), proxy.Div(proxy.Minus(l.im), x));
	}

	TComplex PowU(TComplex x, uint64_t pow) const
	{
		TComplex ans = TComplex(1, 0);
		for (; pow; pow >>= 1)
		{
			if (pow & 1)
				ans = Mult(ans, x);
			x = Mult(x, x);
		}
		return ans;
	}

	TComplex PowS(const TComplex& x, int64_t pow) { return (pow < 0) ? PowU(Inverse(x), -pow) : PowU(x, pow); }
};
