// https://www.hackerrank.com/challenges/possible-path

#include "common/numeric/utils/gcd.h"
#include "common/stl_base.h"

int main_possible_path()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t a, b, x, y;
		cin >> a >> b >> x >> y;
		cout << (GCD(a, b) == GCD(x, y) ? "YES" : "NO") << endl;
	}
	return 0;
}
