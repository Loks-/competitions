// https://www.hackerrank.com/challenges/summing-the-n-series

#include "common/modular/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_summing_the_n_series()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		TModular n;
		cin >> n;
		cout << n.PowU(2) << endl;
	}
	return 0;
}
