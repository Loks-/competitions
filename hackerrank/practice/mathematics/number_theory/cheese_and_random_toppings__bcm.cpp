// https://www.hackerrank.com/challenges/cheese-and-random-toppings

#include "common/numeric/binomial_coefficient_modular.h"
#include "common/stl/base.h"

int main_cheese_and_random_toppings__bcm()
{
	unsigned T, N, R, M;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N >> R >> M;
        BinomialCoefficientModular bcm(M);
        cout << bcm.BinomialCoefficient(N, R) << endl;
	}
	return 0;
}
