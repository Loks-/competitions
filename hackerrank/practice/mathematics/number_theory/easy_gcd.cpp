// https://www.hackerrank.com/challenges/easy-gcd-1

#include "common/numeric/utils/gcd.h"
#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_easy_gcd()
{
	unsigned n, k, g = 0, t, b = 0;
	cin >> n >> k;
    for (unsigned i = 0; i < n; ++i)
    {
        cin >> t;
        g = GCD(t, g);
    }
    PrimesList primes_list(100000);
    auto v = primes_list.Factorize(g);
    for (auto p : v)
        b = max(b, k - (k % unsigned(p.first)));
    cout << b << endl;
	return 0;
}
