// https://www.hackerrank.com/challenges/a-very-special-multiple

#include "common/numeric/factorization/eulers_totient.h"
#include "common/numeric/factorization/divisors.h"
#include "common/numeric/modular_arithmetic.h"
#include "common/numeric/primes_list.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

using TModular = ModularArithmetic<false, false, uint64_t>;

int main_a_very_special_multiple()
{
    PrimesList primes_list(100000);
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
        uint64_t phi = EulersTotient(x, primes_list.Factorize(x));
        vector<uint64_t> vd = GetDivisors(primes_list.Factorize(phi));
        sort(vd.begin(), vd.end());
        for (uint64_t d : vd)
        {
            if (TModular::PowU(10, d, x) == 1)
            {
                a = d;
                break;
            }
        }
        cout << 2 * a + b << endl;
	}
	return 0;
}
