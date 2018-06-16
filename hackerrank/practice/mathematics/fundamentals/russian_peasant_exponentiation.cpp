// https://www.hackerrank.com/challenges/russian-peasant-exponentiation

#include "common/base.h"
#include "common/numeric/complex_modular_proxy.h"

using TComplexProxy = ComplexModularProxy<false>;
using TComplex = TComplexProxy::TComplex;

int main_russian_peasant_exponentiation()
{
	TComplexProxy p;
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		int64_t a, b, k, m;
		cin >> a >> b >> k >> m;
		p.SetMod(m);
		TComplex f = p.PowU(TComplex(a, b), k);
		cout << f.re << " " << f.im << endl;
	}
	return 0;
}
