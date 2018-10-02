// https://www.hackerrank.com/challenges/arthur-and-coprimes

#include "common/numeric/primes_list.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_coprime_conundrum()
{
    PrimesList pl(100000);
    unsigned n;
    cin >> n;
    uint64_t total = 0;
    for (unsigned p = 2; ; ++p)
    {
        unsigned maxq = n / p;
        if (maxq <= p) break;
        total += pl.EulerPhi(p) * (maxq / p - 1);
        for (unsigned q = 1, qlast = maxq % p; q <= qlast; ++q)
        {
            if (GCD(p, q) == 1)
                ++total;
        }
    }
    cout << total << endl;
	return 0;
}
