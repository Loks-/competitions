// https://www.hackerrank.com/challenges/the-white-lotus-and-caterpillar-game

#include "common/stl/base.h"

#include <iomanip>

int main_the_white_lotus_and_caterpillar_game()
{
    cout << setprecision(7) << fixed;
	uint64_t n, m, s = 0;
	cin >> n >> m;
	n -= 1;
	for (uint64_t i = 0; i < m; ++i)
	{
		for (uint64_t j = 0; j < m; ++j)
		{
			if (i + 1 < j)
				s += std::max(j, n);
			else if (j + 1 < i)
				s += std::max(m - j - 1, n);
			else
				s += n;
		}
	}
    cout << (1.0 * s) / (m * m) << endl;
	return 0;
}
