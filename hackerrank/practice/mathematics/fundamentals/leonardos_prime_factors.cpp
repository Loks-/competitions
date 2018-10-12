// https://www.hackerrank.com/challenges/leonardo-and-prime

#include "common/factorization/primes_list.h"
#include "common/stl/base.h"

int main_leonardos_prime_factors()
{
	PrimesList primes_list(100);
	const vector<uint64_t>& primes = primes_list.GetPrimes();
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t n, k = 0;
		cin >> n;
		for (; primes[k] <= n; ++k)
			n /= primes[k];
		cout << k << endl;
	}
	return 0;
}
