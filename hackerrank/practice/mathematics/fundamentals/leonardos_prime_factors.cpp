// https://www.hackerrank.com/challenges/leonardo-and-prime

#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_leonardos_prime_factors()
{
	PrimesList primes_list(100);
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t n, k = 0;
		cin >> n;
		for (; primes_list.GetPrimes()[k] <= n; ++k)
			n /= primes_list.GetPrimes()[k];
		cout << k << endl;
	}
	return 0;
}
