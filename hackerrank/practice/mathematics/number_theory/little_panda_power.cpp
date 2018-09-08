// https://www.hackerrank.com/challenges/littlepandapower

#include "common/numeric/modular_arithmetic.h"
#include "common/stl/base.h"

int main_little_panda_power()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t A, X;
        int64_t B;
		cin >> A >> B >> X;
		cout << TModularArithmetic_C32U::PowSSafe(A, B, X) << endl;
	}
	return 0;
}
