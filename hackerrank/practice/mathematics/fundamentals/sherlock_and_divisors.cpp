// https://www.hackerrank.com/challenges/sherlock-and-divisors

#include "common/base.h"
#include "common/numeric/primes_list.h"

int main_sherlock_and_divisors()
{
	unsigned T, N;
	PrimesList primes(100000);
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N;
		cout << ((N & 1) ? 0 : primes.GetDivisorsCount(N / 2)) << endl;
	}
	return 0;
}
