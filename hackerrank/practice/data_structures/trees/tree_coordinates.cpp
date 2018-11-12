// https://www.hackerrank.com/challenges/tree-coordinates

#include "common/graph/base_graph/distance.h"
#include "common/graph/base_tree/base_tree.h"
#include "common/graph/base_tree/deep.h"
#include "common/graph/base_tree/lca.h"
#include "common/stl/base.h"

int main_tree_coordinates()
{
	unsigned n, m;
	cin >> n >> m;
	BaseTree t(n);
	t.ReadEdges();
	vector<unsigned> vd = GetDeep(t), vdp1, vdp2;
	LCA lca;
	lca.Build(t);
	vector<unsigned> vp1(m), vp2(m);
	for (unsigned it = 0; it < m; ++it)
	{
		unsigned a, b;
		cin >> a >> b;
		vp1[it] = a - 1;
		vp2[it] = b - 1;
	}

	unsigned bp1 = 0, bp2 = 0;
	for (unsigned i = 1; i < n; ++i)
	{
		if (vd[i] > vd[bp1])
			bp1 = i;
	}
	vdp1 = GetDistancesFromSource(t, bp1);
	for (unsigned i = 1; i < n; ++i)
	{
		if (vdp1[i] > vdp1[bp2])
			bp2 = i;
	}
	vdp2 = GetDistancesFromSource(t, bp2);

	unsigned adj = 2 * vdp1[bp2];
	vector<vector<unsigned>> vl11(5 * adj), vl12(5 * adj), vl21(5 * adj), vl22(5 * adj);
	unsigned m11 = 0, m12 = 0, m21 = 0, m22 = 0;
	for (unsigned i = 0; i < m; ++i)
	{
		unsigned t11 = vdp1[vp1[i]];
		unsigned t12 = vdp1[vp2[i]];
		unsigned t21 = vdp2[vp1[i]];
		unsigned t22 = vdp2[vp2[i]];
		unsigned d11 = t11 + t12; m11 = max(m11, d11); vl11[d11].push_back(i);
		unsigned d12 = t11 + t22; m12 = max(m12, d12); vl12[d12].push_back(i);
		unsigned d21 = t21 + t12; m21 = max(m21, d21); vl21[d21].push_back(i);
		unsigned d22 = t21 + t22; m22 = max(m22, d22); vl22[d22].push_back(i);
	}

	unsigned max_distance = 0;
	for (unsigned s = max(m11 + m22, m12 + m21); s > max_distance + adj; --s)
	{
		if (s <= max_distance + adj) break;
		for (unsigned s1 = 0; s1 <= s; ++s1)
		{
			unsigned s2 = s - s1;
			for (unsigned i1 : vl11[s1])
			{
				for (unsigned i2 : vl22[s2])
				{
					unsigned l = lca.GetDistance(vp1[i1], vp1[i2]) + lca.GetDistance(vp2[i1], vp2[i2]);
					max_distance = max(max_distance, l);
					if (s <= max_distance + adj) break;
				}
				if (s <= max_distance + adj) break;
			}
			if (s <= max_distance + adj) break;
			for (unsigned i1 : vl12[s1])
			{
				for (unsigned i2 : vl21[s2])
				{
					unsigned l = lca.GetDistance(vp1[i1], vp1[i2]) + lca.GetDistance(vp2[i1], vp2[i2]);
					max_distance = max(max_distance, l);
					if (s <= max_distance + adj) break;
				}
				if (s <= max_distance + adj) break;
			}
			if (s <= max_distance + adj) break;
		}
	}
	cout << max_distance << endl;
	return 0;
}
