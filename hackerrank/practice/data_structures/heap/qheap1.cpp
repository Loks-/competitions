// https://www.hackerrank.com/challenges/qheap1

#include "common/stl/base.h"

#include <set>

int main_qheap1()
{
	set<int> s;
	int Q, q, x;
	cin >> Q;
	for (; Q; --Q)
	{
		cin >> q;
		if (q == 1)
		{
			cin >> x;
			s.insert(x);
		}
		else if (q == 2)
		{
			cin >> x;
			s.erase(x);
		}
		else if (q == 3)
		{
			cout << *s.begin() << endl;
		}
	}
	return 0;
}
