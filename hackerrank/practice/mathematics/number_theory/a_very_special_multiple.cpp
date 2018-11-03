// https://www.hackerrank.com/challenges/a-very-special-multiple

#include "common/factorization/factorization.h"
#include "common/factorization/utils/eulers_totient.h"
#include "common/factorization/utils/divisors.h"
#include "common/modular/modular_arithmetic.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

using TModularA = ModularArithmetic<false, false, uint64_t>;

int main_a_very_special_multiple()
{
    Factorization ff;
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t x;
        cin >> x;
        uint64_t p2 = 0, p5 = 0;
        for (; (x % 2) == 0; x /= 2) ++p2;
        for (; (x % 5) == 0; x /= 5) ++p5;
        uint64_t b = max<uint64_t>(p5, max<uint64_t>(p2, 2) - 2), a;
        x *= 9;
        uint64_t phi = EulersTotient(x, ff.Factorize(x));
        vector<uint64_t> vd = GetDivisors(ff.Factorize(phi));
        sort(vd.begin(), vd.end());
        for (uint64_t d : vd)
        {
            if (TModularA::PowU(10, d, x) == 1)
            {
                a = d;
                break;
            }
        }
        cout << 2 * a + b << endl;
	}
	return 0;
}
