// https://www.hackerrank.com/challenges/mehta-and-his-laziness

#include "common/base.h"
#include "common/numeric/primes_list.h"
#include "common/numeric/utils/gcd.h"

int main_mehta_and_his_laziness()
{
    PrimesList pl(10000);
	unsigned T, N;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N;
        auto f = pl.Factorize(N);
        if ((f.size() == 0) || (f[0].first != 2) || (f[0].second < 2) || (N == 4))
            cout << 0 << endl;
        else
        {
            bool square = true;
            unsigned p = 1, q = 1;
            for (unsigned i = 0; i < f.size(); ++i)
            {
                square = square && ((f[i].second & 1) == 0);
                p *= (((i > 0) ? 1 : 0) + f[i].second / 2);
                q *= (1 + f[i].second);
            }
            p -= square ? 1 : 0;
            q -= 1;
            unsigned r = GCD(p, q);
            p /= r; q /= r;
            cout << p << "/" << q << endl;
        }
	}
	return 0;
}
