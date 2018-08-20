// https://www.hackerrank.com/challenges/down-to-zero-ii

#include "common/stl_base.h"

int main_down_to_zero_ii()
{
	unsigned MN = 1000000 + 1;
	vector<unsigned> v(MN, MN);
	v[0] = 0;
	for (unsigned i = 0; i < MN - 1; ++i)
	{
		v[i + 1] = min(v[i + 1], v[i] + 1);
		for (unsigned j = 2; j <= i; ++j)
		{
			if (i*j >= MN) break;
			v[i*j] = min(v[i*j], v[i] + 1);
		}
	}
	unsigned Q, N;
	cin >> Q;
	for (; Q; --Q)
	{
		cin >> N;
		cout << v[N] << endl;
	}
	return 0;
}
