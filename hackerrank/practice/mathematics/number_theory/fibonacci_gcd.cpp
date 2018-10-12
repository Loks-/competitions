// https://www.hackerrank.com/challenges/fibonacci-gcd

#include "common/modular/fibonacci_modular_matrix.h"
#include "common/modular/modular.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

using TFibonacci = FibonacciModularMatrix<Modular<1000000007>>;

int main_fibonacci_gcd()
{
	unsigned N;
	cin >> N;
	uint64_t a, b;
	cin >> a;
	for (unsigned i = 1; i < N; ++i)
	{
		cin >> b;
		a = GCD(b, a);
	}
	cout << TFibonacci::GetU(a).Get() <<endl;
	return 0;
}
