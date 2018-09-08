// https://www.hackerrank.com/challenges/equal-stacks

#include "common/vector/read.h"
#include "common/stl/base.h"

#include <unordered_map>

int main_equal_stacks()
{
	vector<unsigned> vn = ReadVector<unsigned>(3);
	unordered_map<unsigned, unsigned> m;
	for (unsigned i = 0; i < 3; ++i)
	{
		vector<unsigned> vt = ReadVector<unsigned>(vn[i]);
		reverse(vt.begin(), vt.end());
		unsigned h = 0;
		for (unsigned ih : vt)
		{
			h += ih;
			m[h] += 1;
		}
	}
	unsigned best = 0;
	for (auto p : m)
	{
		if (p.second == 3)
			best = max(best, p.first);
	}
	cout << best << endl;
	return 0;
}
