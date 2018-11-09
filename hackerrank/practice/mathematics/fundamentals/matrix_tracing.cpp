// https://www.hackerrank.com/challenges/matrix-tracing

#include "common/modular/static/modular_io.h"
#include "common/modular/static/factorial.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular, true>;

int main_matrix_tracing()
{
	TFactorial f;
	unsigned T, N, M;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> M;
		cout << f.BinomialCoefficient(N + M - 2, N - 1) << endl;
	}
	return 0;
}
