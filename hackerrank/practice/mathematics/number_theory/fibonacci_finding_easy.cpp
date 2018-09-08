// https://www.hackerrank.com/challenges/fibonacci-finding-easy

#include "common/numeric/fibonacci_modular_matrix.h"
#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TFibonacci = FibonacciModularMatrix<Modular<1000000007>>;

int main_fibonacci_finding_easy()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t A, B, N;
		cin >> A >> B >> N;
		cout << TFibonacci::GetU(N, A, B).Get() << endl;
	}
	return 0;
}
