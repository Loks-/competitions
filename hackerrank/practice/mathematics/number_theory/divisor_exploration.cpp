// https://www.hackerrank.com/challenges/divisor-exploration

#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_divisor_exploration()
{
    TModular half = TModular(2).Inverse();
    vector<TModular> v(1, 1);
    for (unsigned i = 0; i < 200000; ++i)
        v.push_back(v.back() * TModular(i + 3) * TModular(i + 2) * half);
	unsigned T, m, a;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> m >> a;
        cout << (v[a + m] / v[a]).Get() << endl;
	}
	return 0;
}
