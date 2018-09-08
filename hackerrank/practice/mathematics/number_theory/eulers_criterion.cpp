// https://www.hackerrank.com/challenges/eulers-criterion

#include "common/numeric/utils/legendre_symbol.h"
#include "common/stl/base.h"

int main_eulers_criterion()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t A, M;
		cin >> A >> M;
		cout << ((LegendreSymbol(A, M) >= 0) ? "YES" : "NO") << endl;
	}
	return 0;
}
