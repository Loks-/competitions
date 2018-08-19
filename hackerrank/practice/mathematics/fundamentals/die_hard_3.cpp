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
		for (; !qp.empty();)
		{
			pair<unsigned, unsigned> p = qp.front();
			qp.pop();
			unsigned x = p.first, y = p.second;
#define CHECK_AND_ADD(u, v) \
            if ((u == c) || (v == c)) {found = true; break; } \
            if (!vb[u][v]) { qp.push(make_pair(u, v)); vb[u][v] = true; }

			CHECK_AND_ADD(x, 0);
			CHECK_AND_ADD(0, y);
			CHECK_AND_ADD(x, b);
			CHECK_AND_ADD(a, y);
			if (x + y < a)
			{
				CHECK_AND_ADD(x + y, 0);
			}
			if (x + y < b)
			{
				CHECK_AND_ADD(0, x + y);
			}
			if (x + y > a)
			{
				CHECK_AND_ADD(a, x + y - a);
			}
			if (x + y > b)
			{
				CHECK_AND_ADD(x + y - b, b);
			}
		}
		cout << (found ? "YES" : "NO") << endl;
	}
	return 0;
}
