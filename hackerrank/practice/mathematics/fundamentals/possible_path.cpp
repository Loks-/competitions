// https://www.hackerrank.com/challenges/possible-path

#include "common/base.h"
#include "common/numeric/utils.h"

int main_possible_path()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t a, b, x, y;
		cin >> a >> b >> x >> y;
		cout << (NumbericUtils::GCD(a, b) == NumbericUtils::GCD(x, y) ? "YES" : "NO") << endl;
	}
	return 0;
}
