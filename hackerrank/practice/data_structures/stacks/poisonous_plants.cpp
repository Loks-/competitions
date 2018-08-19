// https://www.hackerrank.com/challenges/poisonous-plants

#include "common/stl_base.h"

#include <stack>

int main_poisonous_plants()
{
	unsigned N, p, l = 0;
	cin >> N;
	stack<pair<unsigned, unsigned>> s;
	for (unsigned in = 0; in < N; ++in)
	{
		cin >> p;
		unsigned d = 0;
		for (; !s.empty() && s.top().first >= p; s.pop())
			d = max(d, s.top().second);
		if (s.empty())
			d = 0;
		else
		{
			d += 1;
			l = max(l, d);
		}
		s.push({ p, d });
	}
	cout << l << endl;
	return 0;
}
