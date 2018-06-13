// https://www.hackerrank.com/challenges/crush

#include "common/base.h"

int main_array_manipulation()
{
	unsigned n, m, a, b;
	int64_t k;
	cin >> n >> m;
	vector<int64_t> v(n + 1, 0);
	for (unsigned i = 0; i < m; ++i)
	{
		cin >> a >> b >> k;
		v[a - 1] += k;
		v[b] -= k;
	}
	int64_t current = 0, best = 0;
	for (unsigned i = 0; i < n; ++i)
	{
		current += v[i];
		best = max(best, current);
	}
	cout << best << endl;
	return 0;
}
