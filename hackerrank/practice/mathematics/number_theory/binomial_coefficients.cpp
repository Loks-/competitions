// https://www.hackerrank.com/challenges/binomial-coefficients

#include "common/numeric/long_unsigned.h"
#include "common/stl/base.h"

int main_binomial_coefficients()
{
	unsigned T, P;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        LongUnsigned n = LongUnsigned::Read(), t = n, r(1u);
        cin >> P;
        for (; !t.Empty(); t /= P)
        {
            unsigned k = t % P;
            r *= (1 + k);
        }
        (n + 1 - r).Write();
	}
	return 0;
}
