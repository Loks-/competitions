// https://www.hackerrank.com/challenges/truck-tour

#include "common/stl_base.h"

int main_truck_tour()
{
	unsigned N;
	cin >> N;
	vector<int64_t> vp(N), vd(N);
	for (unsigned i = 0; i < N; ++i)
		cin >> vp[i] >> vd[i];
	for (unsigned i = 0, j; i < N; ++i)
	{
		int64_t p = 0;
		for (j = i; j < i + N; ++j)
		{
			p += vp[j % N] - vd[j % N];
			if (p < 0)
				break;
		}
		if (j == i + N)
		{
			cout << i << endl;
			return 0;
		}
		else
			i = j;
	}
	assert(false);
	return 0;
}
