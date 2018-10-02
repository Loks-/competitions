// https://www.hackerrank.com/challenges/easy-math

#include "common/numeric/utils/pow.h"
#include "common/stl/base.h"

int main_easy_math()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        unsigned x;
        cin >> x;
        unsigned xt = x, p2 = 0, p5 = 0;
        for (; (xt % 2) == 0; xt /= 2) ++p2;
        for (; (xt % 5) == 0; xt /= 5) ++p5;
        unsigned b = max(p5, max(p2, 2u) - 2u), a = 1;
        for (uint64_t t = (4 * PowU<uint64_t>(10, b)) % x, ts = t; ts; ++a)
        {
            t = (t * 10) % x;
            ts = (ts + t) % x;
        }
        cout << 2 * a + b << endl;
	}
	return 0;
}
