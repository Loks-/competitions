// https://www.hackerrank.com/challenges/leonardo-and-prime

#include "common/numeric/primes_list.h"
#include "common/stl_base.h"

int main_leonardos_prime_factors()
{
	PrimesList primes(100);
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t n, k = 0;
		cin >> n;
		for (; primes.vprimes[k] <= n; ++k)
			n /= primes.vprimes[k];
		cout << k << endl;
	}
	return 0;
}
