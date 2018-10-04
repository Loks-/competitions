// https://www.hackerrank.com/challenges/equations

#include "common/numeric/modular.h"
#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

using TModular = Modular<1000007>;

int main_equations()
{
    PrimesList primes_list(1100000);
	uint64_t n;
	cin >> n;
    TModular r = 1;
    for (uint64_t p : primes_list.GetPrimes())
    {
        if (p > n) break;
        unsigned c = 0;
        for (uint64_t t = n/p; t; t /= p)
            c += t;
        r *= TModular(2 * c + 1);
    }
    cout << r.Get() << endl;
	return 0;
}
