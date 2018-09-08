// https://www.hackerrank.com/challenges/connecting-towns

#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TModular = TModular_C32<1234567>;

int main_connecting_towns()
{
	unsigned T, n, ni;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> n;
		TModular x = 1;
		for (unsigned i = 1; i < n; ++i)
		{
			cin >> ni;
			x *= TModular(ni);
		}
		cout << x.Get() << endl;
	}
	return 0;
}
