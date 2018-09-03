// https://www.hackerrank.com/challenges/die-hard-3

#include "common/stl_base.h"

#include <queue>

int main_die_hard_3() 
{
	unsigned T, a, b, c;
	cin >> T;
	for (; T; --T)
	{
		cin >> a >> b >> c;
		vector<vector<bool>> vb(a + 1, vector<bool>(b + 1, false));
		queue<pair<unsigned, unsigned>> qp;
		qp.push(make_pair(0u, 0u));
		vb[0][0] = true;
		bool found = false;

		auto CheckAndAdd = [&](unsigned u, unsigned v) {
			if ((u == c) || (v == c))
				found = true;
			else if (!vb[u][v])
				qp.push(make_pair(u, v));
			vb[u][v] = true;
		};
		for (; !found && !qp.empty(); qp.pop())
		{
			pair<unsigned, unsigned> p = qp.front();
			unsigned x = p.first, y = p.second;
			CheckAndAdd(x, 0);
			CheckAndAdd(0, y);
			CheckAndAdd(x, b);
			CheckAndAdd(a, y);
			if (x + y < a)
				CheckAndAdd(x + y, 0);
			if (x + y < b)
				CheckAndAdd(0, x + y);
			if (x + y > a)
				CheckAndAdd(a, x + y - a);
			if (x + y > b)
				CheckAndAdd(x + y - b, b);
		}
		cout << (found ? "YES" : "NO") << endl;
	}
	return 0;
}
