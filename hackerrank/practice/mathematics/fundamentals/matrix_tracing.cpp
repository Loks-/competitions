// https://www.hackerrank.com/challenges/matrix-tracing

#include "common/numeric/modular.h"
#include "common/numeric/factorial_modular.h"
#include "common/stl_base.h"

using TModular = Modular<>;
using TFactorial = FactorialModular<TModular, true>;

int main_matrix_tracing()
{
	TFactorial f;
	unsigned T, N, M;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> M;
		cout << f.BinomialCoefficient(N + M - 2, N - 1).Get() << endl;
	}
	return 0;
}
