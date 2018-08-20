// https://www.hackerrank.com/challenges/castle-on-the-grid

#include "common/graph/base_graph.h"
#include "common/stl_base.h"

#include <string>

int main_castle_on_the_grid()
{
	unsigned N, N2;
	cin >> N;
	N2 = N + 2;
	vector<vector<bool>> vg(N2, vector<bool>(N2, false));
	for (unsigned i = 0; i < N; ++i)
	{
		string s;
		cin >> s;
		assert(s.size() == N);
		for (unsigned j = 0; j < N; ++j)
		{
			if (s[j] == '.')
				vg[i + 1][j + 1] = true;
		}
	}
	vector<pair<int, int>> vd{ {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
	BaseUndirectedGraph g(N2 * N2);
	for (int i = 1; i <= int(N); ++i)
	{
		for (int j = 1; j <= int(N); ++j)
		{
			if (!vg[i][j])
				continue;
			for (auto d : vd)
			{
				for (int k = 1; ; ++k)
				{
					int i2 = i + k * d.first, j2 = j + k * d.second;
					if (vg[i2][j2])
						g.AddEdge(i * N2 + j, i2 * N2 + j2);
					else
						break;
				}
			}
		}
	}
	{
		unsigned i1, i2, j1, j2;
		cin >> i1 >> j1 >> i2 >> j2;
		auto vd = g.GetDistances((i1 + 1) * N2 + (j1 + 1));
		cout << vd[(i2 + 1) * N2 + (j2 + 1)] << endl;
	}
	return 0;
}
