// https://www.hackerrank.com/challenges/gcd-sequence

#include "common/factorization/primes_list_extended/mobius.h"
#include "common/modular/modular.h"
#include "common/modular/factorial_modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_sequence()
{
    TPLEMobius primes_list(100000);
    primes_list.MobiusPrecalc();
    FactorialModular<TModular> f;

	unsigned T, N, K;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N >> K;
        TModular r = 0;
        for (unsigned i = 1; i <= N; ++i)
        {
            int m = primes_list.Mobius(i);
            if (m == 0) continue;
            r += ((m == 1) ? TModular(1) : -TModular(1)) * f.BinomialCoefficient(K + N / i - 1, K);
        }
        cout << r.Get() << endl;
	}
	return 0;
}
