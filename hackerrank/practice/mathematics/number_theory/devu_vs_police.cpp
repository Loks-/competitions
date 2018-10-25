// https://www.hackerrank.com/challenges/devu-police

#include "common/factorization/primes_list_extended/eulers_totient.h"
#include "common/modular/modular_arithmetic.h"
#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

using TModular = TModularArithmetic_C32U;

int main_devu_vs_police()
{
    TPLEEulersTotient primes_list(10000);
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t n1, k1, n2, k2, n, n3;
        cin >> n1 >> k1 >> n2 >> k2 >> n;
        bool small_power = ((n2 == 0) || (n2 == 1) || (k2 == 0) || ((n2 == 2) && (k2 <= 5)) || ((n2 == 3) && (k2 <= 2)) || ((n2 <= 24) && (k2 == 1)));
        n1 = TModular::PowU(n1, k1, n);
        n3 = primes_list.EulersTotient(n);
        n2 = (small_power ? PowU(n2, k2) : 24 * n3 + TModular::PowU(n2, k2, n3));
        cout << TModular::PowU(n1, n2, n) % n << endl;
	}
	return 0;
}
