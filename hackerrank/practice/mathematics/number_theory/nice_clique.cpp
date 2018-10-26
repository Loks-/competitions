// https://www.hackerrank.com/challenges/niceclique

#include "common/factorization/primes_list.h"
#include "common/factorization/utils/divisors_sum.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_nice_clique()
{
	unsigned n;
	cin >> n;
	vector<uint64_t> v = ReadVector<uint64_t>(n);
	uint64_t m = *max_element(v.begin(), v.end());
	uint64_t msqrt = USqrt(m) + 1;
	PrimesList primes_list(msqrt);
	vector<unsigned> m1(2), m2(2);
	for (uint64_t x : v)
	{
		auto f = primes_list.Factorize(x);
		m1[f.size() & 1] += 1;
		m2[GetDivisorsSum(f) & 1] += 1;
	}
	cout << max(max(m1[0], m1[1]), max(m2[0], m2[1])) << endl;
	return 0;
}
