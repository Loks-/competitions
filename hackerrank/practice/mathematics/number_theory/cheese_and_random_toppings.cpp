// https://www.hackerrank.com/challenges/cheese-and-random-toppings

#include "common/factorization/primes_list.h"
#include "common/modular/utils/binomial_coefficient_modular_prime.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/stl/base.h"

int main_cheese_and_random_toppings()
{
	unsigned T, N, R, M;
    PrimesList primes_list(50);
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N >> R >> M;
        uint64_t r = 0, m = 1;
        TFactorization vf = primes_list.Factorize(M);
        for (auto pf : vf)
        {
            uint64_t rf = BinomialCoefficientModularPrime(N, R, unsigned(pf.first));
            r = MergeRemainders<TModularArithmetic_P32U>(m, r, pf.first, rf);
            m *= pf.first;
        }
        cout << r << endl;
	}
	return 0;
}
