// https://www.hackerrank.com/challenges/a-weird-function

#include "common/numeric/primes_list_extended/eulers_totient.h"
#include "common/stl/base.h"

int main_a_weird_function()
{
    TPLEEulersTotient primes_list(1500000);
    primes_list.EulersTotientPrecalc();
    vector<uint64_t> vi, vs(1, 0);
    uint64_t phi1 = 1, phi2 = 1;
    for (uint64_t j = 2;; ++j)
    {
        uint64_t i = (j * (j - 1)) / 2;
        if (i > 1000000000000ull)
            break;
        vi.push_back(i);
        phi1 = phi2;
        phi2 = primes_list.EulersTotient(j & 1 ? j : j / 2);
        vs.push_back(vs.back() + phi1 * phi2);
    }
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t l, r;
        cin >> l >> r;
        size_t i1 = lower_bound(vi.begin(), vi.end(), l) - vi.begin();
        size_t i2 = upper_bound(vi.begin(), vi.end(), r) - vi.begin();
        cout << vs[i2] - vs[i1] << endl;
	}
	return 0;
}
