// https://www.hackerrank.com/challenges/parity-party

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<7340033>;
using TFactorial = ModularFactorial<TModular>;

// TODO:
//   FFT solution
int main_parity_party()
{
	TFactorial f;
	unsigned n, a, b, c, s;
	cin >> n >> a >> b >> c;
    s = a + b + c;
	TModular res = 0, one = 1;
    for (unsigned i = 0; i <= a + b; ++i)
    {
        unsigned k0 = (i <= b) ? 0u : i - b;
        unsigned k1 = (a <= i) ? a : i;
        TModular r = 0, m1pk = (k0 & 1) ? -one : one;
        for (unsigned k = k0; k <= k1; ++k)
        {
            r += f.BinomialCoefficient(a, k) * f.BinomialCoefficient(b, i - k) * m1pk;
            m1pk = -m1pk;
        }
        res += r * (TModular(s) - TModular(2 * i)).PowU(n);
    }
	cout << res / TModular(2).PowU(a + b) << endl;
	return 0;
}
