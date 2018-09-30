#pragma once

#include <vector>

template <class TModularProxy, bool keep_inverted = true>
class FactorialModularProxy
{
public:
    using TValue = typename TModularProxy::TValue;

protected:
    TModularProxy proxy;
	std::vector<TValue> vf, vfi;

public:
	FactorialModularProxy(TValue mod) : proxy(mod) { vf.push_back(1); if (keep_inverted) vfi.push_back(1); }
    const TModularProxy& GetProxy() const { return proxy; }

	void Adjust(unsigned n)
	{
		for (; vf.size() <= n; )
		{
			vf.push_back(proxy.Mult(proxy.ApplyU(vf.size()), vf.back()));
			if (keep_inverted)
				vfi.push_back(proxy.Inverse(vf.back()));
		}
	}

	TValue Get(unsigned n) { Adjust(n); return vf[n];	}
	TValue GetI(unsigned n) { Adjust(n); return (keep_inverted ? vfi[n] : proxy.Inverse(vf[n])); }
	TValue operator()(unsigned n) { return Get(n); }

	TValue BinomialCoefficient(unsigned n, unsigned k)
	{
		if (k > n) return 0;
		Adjust(n);
		return (keep_inverted ? proxy.Mult(proxy.Mult(vf[n], vfi[k]), vfi[n - k]) : proxy.Div(vf[n], proxy.Mult(vf[k], vf[n - k])));
	}
};
