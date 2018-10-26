// https://www.hackerrank.com/challenges/arthur-and-coprimes

#include "common/factorization/factorization.h"
#include "common/factorization/utils/coprime.h"
#include "common/stl/base.h"

int main_coprime_conundrum()
{
    Factorization ff;
    unsigned n;
    cin >> n;
    uint64_t total = 0;
    for (unsigned p = 2; ; ++p)
    {
        unsigned maxq = n / p;
        if (maxq <= p) break;
        total += CoprimeNumers(ff.Factorize(p), maxq - p);
    }
    cout << total << endl;
	return 0;
}
