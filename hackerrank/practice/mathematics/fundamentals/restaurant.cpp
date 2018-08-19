// https://www.hackerrank.com/challenges/restaurant

#include "common/numeric/utils/gcd.h"
#include "common/stl_base.h"

int main_restaurant()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t l, b;
		cin >> l >> b;
		uint64_t m = GCD(l, b);
		cout << l * b / m / m << endl;
	}
	return 0;
}
