// https://www.hackerrank.com/challenges/divisor-exploration-3

#include "common/modular/factorial_modular.h"
#include "common/modular/modular.h"
#include "common/factorization/primes_list.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = FactorialModular<TModular, true>;

int main_divisor_exploration_3()
{
    TFactorial f;
    PrimesList primes_list(100000);
    const vector<uint64_t>& primes = primes_list.GetPrimes();
	unsigned T, m, a, d;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        TModular r = 1, one = 1;
        cin >> m >> a >> d;
        for (unsigned i = 0; i < m; ++i)
        {
            TModular s = 0;
            s += f.BinomialCoefficient(a + i + d, d - 1);
            TModular p = primes[i], p1 = p - one, pp = p1;
            for (unsigned j = 1; j <= a + i + 1; ++j)
            {
                s += f.BinomialCoefficient(a + i + d - j, d - 1) * pp;
                pp *= p;
            }
            r *= s;
        }
        cout << r.Get() << endl;
	}
	return 0;
}
