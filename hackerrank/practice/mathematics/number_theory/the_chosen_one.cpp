// https://www.hackerrank.com/challenges/the-chosen-one

#include "common/numeric/utils/gcd.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

int main_the_chosen_one()
{
	unsigned n;
	cin >> n;
	vector<uint64_t> v = ReadVector<uint64_t>(n);
	if (n == 1)
	{
		cout << v[0] + 1 << endl;
		return 0;
	}
	vector<uint64_t> vl(n), vr(n);
	vl[0] = vr[n - 1] = 0;
	for (unsigned i = 1; i < n; ++i)
	{
		vl[i] = GCD(vl[i - 1], v[i - 1]);
		vr[n - 1 - i] = GCD(vr[n - i], v[n - i]);
	}
	for (unsigned i = 0; i < n; ++i)
	{
		uint64_t gcd = GCD(vl[i], vr[i]);
		if ((v[i] % gcd) != 0)
		{
			cout << gcd << endl;
			return 0;
		}
	}
	return 0;
}
