// https://www.hackerrank.com/challenges/sherlock-and-divisors

#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_sherlock_and_divisors()
{
	unsigned T, N;
	PrimesList primes_list(100000);
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N;
		cout << ((N & 1) ? 0 : primes_list.GetDivisorsCount(N / 2)) << endl;
	}
	return 0;
}
