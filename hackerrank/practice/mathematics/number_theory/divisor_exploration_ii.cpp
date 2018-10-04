// https://www.hackerrank.com/challenges/divisor-exploration-2

#include "common/numeric/modular.h"
#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_divisor_exploration_ii()
{
    PrimesList primes_list(10000000);
	unsigned T, m, a;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        TModular r = 1, one = 1;
        cin >> m >> a;
        for (unsigned i = 0; i < m; ++i)
        {
            TModular p = primes_list.GetPrimes()[i];
            r *= ((p.PowU(a + i + 2) - one) / (p - one) * p - TModular(a + i + 2)) / (p - one);
        }
        cout << r.Get() << endl;
	}
	return 0;
}
