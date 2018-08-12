// https://www.hackerrank.com/challenges/littlepandapower

#include "common/base.h"
#include "common/numeric/modular_arithmetic.h"

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
