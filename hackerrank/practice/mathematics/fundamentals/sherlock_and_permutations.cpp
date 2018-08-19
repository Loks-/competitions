// https://www.hackerrank.com/challenges/sherlock-and-permutations

#include "common/numeric/modular.h"
#include "common/numeric/factorial_modular.h"
#include "common/stl_base.h"

using TModular = Modular<>;
using TFactorial = FactorialModular<TModular, true>;

int main_sherlock_and_permutations()
{
	TFactorial f;
	unsigned T, N, M;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> M;
		cout << f.BinomialCoefficient(N + M - 1, N).Get() << endl;
	}
	return 0;
}
