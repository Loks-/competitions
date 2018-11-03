// https://www.hackerrank.com/challenges/equations

#include "common/factorization/primes_list.h"
#include "common/modular/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<1000007>;

int main_equations()
{
	uint64_t n;
	cin >> n;
    TModular r = 1;
    PrimesList primes_list(n);
    for (uint64_t p : primes_list.GetPrimes())
    {
        if (p > n) break;
        uint64_t c = 0;
        for (uint64_t t = n/p; t; t /= p)
            c += t;
        r *= TModular(2 * c + 1);
    }
    cout << r << endl;
	return 0;
}
