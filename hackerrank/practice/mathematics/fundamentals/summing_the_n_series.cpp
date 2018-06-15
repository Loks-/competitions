// https://www.hackerrank.com/challenges/summing-the-n-series

#include "common/base.h"
#include "common/numeric/modular.h"

using TModular = Modular<>;

int main_summing_the_n_series()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t n;
		cin >> n;
		cout << (TModular(n)).PowU(2).Get() << endl;
	}
	return 0;
}
