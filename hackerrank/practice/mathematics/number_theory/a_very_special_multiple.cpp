// https://www.hackerrank.com/challenges/a-very-special-multiple

#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

int main_a_very_special_multiple()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t x;
        cin >> x;
        uint64_t xt = x, p2 = 0, p5 = 0;
        for (; (xt % 2) == 0; xt /= 2) ++p2;
        for (; (xt % 5) == 0; xt /= 5) ++p5;
        uint64_t b = max<uint64_t>(p5, max<uint64_t>(p2, 2) - 2), a = 1;
        for (uint64_t t = (4 * PowU<uint64_t>(10, b)) % x, ts = t; ts; ++a)
        {
            t = (t * 10) % x;
            ts = (ts + t) % x;
        }
        cout << 2 * a + b << endl;
	}
	return 0;
}
