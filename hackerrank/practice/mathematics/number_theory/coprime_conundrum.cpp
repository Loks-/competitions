// https://www.hackerrank.com/challenges/arthur-and-coprimes

#include "common/numeric/primes_list.h"
#include "common/numeric/utils/coprime.h"
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
        total += CoprimeNumers(p, maxq - p, pl);
    }
    cout << total << endl;
	return 0;
}
