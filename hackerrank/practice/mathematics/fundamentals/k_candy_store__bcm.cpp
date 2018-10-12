// https://www.hackerrank.com/challenges/k-candy-store

#include "common/modular/binomial_coefficient_modular.h"
#include "common/stl/base.h"

int main_k_candy_store__bcm()
{
	BinomialCoefficientModular bcm(1000000000);
	unsigned T, N, K;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> K;
		cout << bcm.BinomialCoefficient(N + K - 1, K) << endl;
	}
	return 0;
}
