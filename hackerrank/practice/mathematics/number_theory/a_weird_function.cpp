// https://www.hackerrank.com/challenges/a-weird-function

#include "common/numeric/primes_list.h"
#include "common/stl/base.h"

int main_a_weird_function()
{
    PrimesList pl(2000000);
    vector<uint64_t> vi, vs(1, 0);
    for (uint64_t j = 2, i = 1; i <= 1000000000000ull; ++j)
    {
        vi.push_back(i);
        i = (j * (j + 1)) / 2;
    }
    for (uint64_t i : vi)
        vs.push_back(vs.back() + pl.EulerPhi(i));
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
