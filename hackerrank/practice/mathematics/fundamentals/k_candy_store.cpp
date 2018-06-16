// https://www.hackerrank.com/challenges/k-candy-store

#include "common/base.h"
#include "common/numeric/modular.h"
#include "common/numeric/binomial_coefficient_modular.h"

using TModular = TModular_C32<1000000000>;
using TBinomialCoefficient = BinomialCoefficientModular<TModular>;

int main_k_candy_store()
{
	TBinomialCoefficient bc;
	unsigned T, N, K;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> K;
		cout << bc(N + K - 1, K).Get() << endl;
	}
	return 0;
}
