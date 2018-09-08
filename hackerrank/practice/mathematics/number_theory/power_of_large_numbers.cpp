// https://www.hackerrank.com/challenges/power-of-large-numbers

#include "common/numeric/modular_arithmetic.h"
#include "common/stl/base.h"

#include <string>

int main_power_of_large_numbers()
{
	const uint64_t M1 = 1000000007, M2 = M1 - 1;
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		string s1, s2;
		cin >> s1 >> s2;
		uint64_t v1 = 0, v2 = 0;
		for (char c : s1)
			v1 = (v1 * 10 + (c - '0')) % M1;
		for (char c : s2)
			v2 = (v2 * 10 + (c - '0')) % M2;
		cout << TModularArithmetic_P32U::PowU(v1, v2, M1) << endl;
	}
	return 0;
}
