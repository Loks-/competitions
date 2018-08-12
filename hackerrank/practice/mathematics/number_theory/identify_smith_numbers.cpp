// https://www.hackerrank.com/challenges/identify-smith-numbers

#include "common/base.h"
#include "common/numeric/primes_list.h"
#include "common/numeric/utils.h"

int main_identify_smith_numbers()
{
    PrimesList pl(100000);
	uint64_t n;
	cin >> n;
    uint64_t s = 0;
    auto vf = pl.Factorize(n);
    for (auto p : vf)
        s += DigitsSum(p.first) * p.second;
    cout << (s == DigitsSum(n) ? 1 : 0) << endl;
	return 0;
}
