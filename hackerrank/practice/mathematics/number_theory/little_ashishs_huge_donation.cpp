// https://www.hackerrank.com/challenges/little-chammys-huge-donation

#include "common/binary_search.h"
#include "common/stl/base.h"

int main_little_ashishs_huge_donation()
{
    auto f = [](uint64_t x) { return (x * (x + 1) * (2 * x + 1)) / 6; };
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N;
        cin >> N;
        uint64_t i = UpperBoundF(0, 200000000, N, f);
        cout << i - 1 << endl;
	}
	return 0;
}
