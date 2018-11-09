// https://www.hackerrank.com/challenges/ncr

#include "common/modular/proxy/binomial_coefficient.h"
#include "common/stl/base.h"

int main_ncr()
{
    PModularBinomialCoefficient bcm(142857);
	unsigned T, n, r;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> n >> r;
        cout << bcm.BinomialCoefficient(n, r) << endl;
	}
	return 0;
}
