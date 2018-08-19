// https://www.hackerrank.com/challenges/jim-and-the-jokes

#include "common/stl_base.h"

#include <unordered_map>

int main_jim_and_the_jokes()
{
	unordered_map<unsigned, uint64_t> um;
	unsigned n, m, d, x;
	cin >> n;
	for (; n; --n)
	{
		cin >> m >> d;
		unsigned d1 = (d % 10), d2 = (d / 10);
		if ((d1 >= m) || (d2 >= m))
			continue;
		x = d1 + d2 * m;
		um[x] += 1;
	}
	uint64_t r = 0;
	for (auto p : um)
	{
		r += (p.second * (p.second - 1)) / 2;
	}
	cout << r << endl;
	return 0;
}
