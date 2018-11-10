// https://www.hackerrank.com/challenges/rirb

#include "common/stl/base.h"

#include <iomanip>

int main_random_integers_random_bits()
{
    auto SolveB = [](uint64_t n)
    {
        uint64_t k = 0, s = 0;
        for (; (n >> k); ++k);
        for (; k > 0; --k)
        {
            uint64_t p2 = 1ull << k;
            if (n < p2) continue;
            s += (p2 / 2) * k;
            n -= p2;
            s += n + 1;
        }
        s += n;
        return s;
    };

    auto SolveA = [&](uint64_t n)
    {
        if (n <= 1)
            return 1.0 * n;
        double s = 0;
        for (uint64_t k = 0; ; ++k)
        {
            uint64_t v1 = (1ull << k), v2 = v1 * 2;
            if (n >= v2)
                s += v1 * (1.0 + k * 0.5) / (k + 1);
            else
            {
                s += (SolveB(n) - SolveB(v1 - 1)) / (k + 1.0);
                break;
            }
        }
        return s;
    };

    auto SolveA2 = [&](uint64_t A, uint64_t B) { return (SolveA(B) - SolveA(A - 1)) / (B - A + 1); };
    auto SolveB2 = [&](uint64_t A, uint64_t B) { return double(SolveB(B) - SolveB(A - 1)) / (B - A + 1); };

    cout << setprecision(5) << fixed;
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t a, b;
		cin >> a >> b;
        cout << SolveA2(a, b) << " " << SolveB2(a, b) << endl;
	}
	return 0;
}
