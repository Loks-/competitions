// https://www.hackerrank.com/challenges/fibonacci-gcd

#include "common/base.h"
#include "common/numeric/fibonacci_modular_matrix.h"
#include "common/numeric/modular.h"
#include "common/numeric/utils/gcd.h"

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
