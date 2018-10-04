// https://www.hackerrank.com/challenges/breaking-sticks

#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_breaking_sticks()
{
    PrimesList primes_list(1100000);
	unsigned n;
	cin >> n;
    uint64_t sum = n, a;
    for (unsigned i = 0; i < n; ++i)
    {
        cin >> a;
        auto vf = primes_list.Factorize(a);
        reverse(vf.begin(), vf.end());
        uint64_t t = 1;
        for (auto p : vf)
        {
            for (unsigned j = 0; j < p.second; ++j)
                sum += (t *= p.first);
        }
    }
    cout << sum << endl;
	return 0;
}
