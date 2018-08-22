// https://www.hackerrank.com/challenges/kindergarten-adventures

#include "common/stl_base.h"

int main_kindergarten_adventures_linear()
{
	unsigned n;
	cin >> n;
	vector<int> v(2 * n + 1);
	for (unsigned i = 0; i < n; ++i)
	{
		unsigned t;
		cin >> t;
		if (t < n)
		{
			v[i + 1] += 1;
			v[i + n - t + 1] -= 1;
		}
	}
	for (unsigned i = 1; i <= 2 * n; ++i)
		v[i] += v[i - 1];
	int best = 0; unsigned p = 0;
	for (unsigned i = 0; i < n; ++i)
	{
		int x = v[i] + v[i + n];
		if (best < x)
		{
			best = x;
			p = i;
		}
	}
	cout << p + 1 << endl;
	return 0;
}
