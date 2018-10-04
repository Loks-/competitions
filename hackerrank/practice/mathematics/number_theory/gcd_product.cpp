// https://www.hackerrank.com/challenges/gcd-product

#include "common/numeric/modular.h"
#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_product()
{
    uint64_t n, m;
    cin >> n >> m;
    if (n < m)
        swap(n, m);
    TModular r = 1;
    PrimesList pl(m);
    for (uint64_t p : pl.vprimes)
    {
        TModular mp(p);
        for (uint64_t pp = p; pp <= m; pp *= p)
        {
            r *= mp.PowU((m / pp) * (n / pp));
        }
    }
    cout << r.Get() << endl;
	return 0;
}
