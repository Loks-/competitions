// https://www.hackerrank.com/challenges/arthur-and-coprimes

#include "common/numeric/factorization/coprime.h"
#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_coprime_conundrum()
{
    PrimesList primes_list(100000);
    unsigned n;
    cin >> n;
    uint64_t total = 0;
    for (unsigned p = 2; ; ++p)
    {
        unsigned maxq = n / p;
        if (maxq <= p) break;
        total += CoprimeNumers(primes_list.Factorize(p), maxq - p);
    }
    cout << total << endl;
	return 0;
}
