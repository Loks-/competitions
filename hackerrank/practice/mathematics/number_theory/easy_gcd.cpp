// https://www.hackerrank.com/challenges/easy-gcd-1

#include "common/factorization/utils/factorize.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_easy_gcd()
{
	unsigned n, k, g = 0, t, b = 0;
	cin >> n >> k;
    for (unsigned i = 0; i < n; ++i)
    {
        cin >> t;
        g = GCD(t, g);
    }
    TFactorization f = Factorize(g);
    for (auto p : f)
        b = max(b, k - (k % unsigned(p.first)));
    cout << b << endl;
	return 0;
}
