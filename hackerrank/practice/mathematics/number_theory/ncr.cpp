// https://www.hackerrank.com/challenges/ncr

#include "common/numeric/utils/binomial_coefficient_extended.h"
#include "common/stl/base.h"

int main_ncr()
{
    BinomialCoefficientExtended bce(142857);
	unsigned T, n, r;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> n >> r;
        cout << bce.BinomialCoefficient(n, r) << endl;
	}
	return 0;
}
