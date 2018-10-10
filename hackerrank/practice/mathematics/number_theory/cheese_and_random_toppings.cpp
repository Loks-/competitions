// https://www.hackerrank.com/challenges/cheese-and-random-toppings

#include "common/numeric/primes_list.h"
#include "common/numeric/utils/binomial_coefficient_prime_modular.h"
#include "common/numeric/utils/merge_remainders.h"
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
            uint64_t rf = BinomialCoefficientPrimeModular(N, R, unsigned(pf.first));
            r = MergeRemainders<TModularArithmetic_P32U>(m, r, pf.first, rf);
            m *= pf.first;
        }
        cout << r << endl;
	}
	return 0;
}
