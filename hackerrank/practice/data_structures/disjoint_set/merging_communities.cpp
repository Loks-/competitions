// https://www.hackerrank.com/challenges/merging-communities

#include "common/disjoint_set.h"
#include "common/stl_base.h"

int main_merging_communities()
{
	unsigned N, Q;
	cin >> N >> Q;
	DisjointSet ds(N + 1);
	for (; Q; --Q)
	{
		char c; unsigned i, j;
		cin >> c;
		if (c == 'Q')
		{
			cin >> i;
			cout << ds.GetSize(i) << endl;
		}
		else if (c == 'M')
		{
			cin >> i >> j;
			ds.Union(i, j);
		}
	}
	return 0;
}
