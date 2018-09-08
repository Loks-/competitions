// https://www.hackerrank.com/challenges/dynamic-array

#include "common/stl/base.h"

int main_dynamic_array()
{
	unsigned n, m, l = 0;
	cin >> n >> m;
	vector<vector<unsigned>> v(n);
	for (unsigned i = 0; i < m; ++i)
	{
		unsigned t, x, y;
		cin >> t >> x >> y;
		if (t == 1)
			v[(x ^ l) % n].push_back(y);
		else if (t == 2)
		{
			auto& vr = v[(x ^ l) % n];
			l = vr[y % (vr.size())];
			cout << l << endl;
		}
	}
	return 0;
}
