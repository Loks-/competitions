// https://www.hackerrank.com/challenges/ajob-subsequence

#include "common/modular/utils/binomial_coefficient_modular_prime.h"
#include "common/stl/base.h"

int main_ajob_subsequence()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, K, P;
        cin >> N >> K >> P;
        cout << BinomialCoefficientModularPrime(N + 1, K + 1, P) << endl;
	}
	return 0;
}
