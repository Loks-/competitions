// https://www.hackerrank.com/challenges/cube-summation

#include "common/binary_indexed_tree/bit_3d.h"
#include "common/stl_base.h"

#include <string>

int main_cube_summation()
{
	unsigned T;
	cin >> T;
	BIT_3D<int64_t> bit(0, 0, 0);
	for (unsigned it = 0; it < T; ++it)
	{
		unsigned N, M;
		cin >> N >> M;
		++N;
		bit.Reset(N, N, N);
		for (unsigned im = 0; im < M; ++im)
		{
			string s;
			cin >> s;
			if (s == "UPDATE")
			{
				unsigned i1, i2, i3;
				int64_t v;
				cin >> i1 >> i2 >> i3 >> v;
				v -= bit.Get(i1, i2, i3);
				bit.Add(i1, i2, i3, v);
			}
			else if (s == "QUERY")
			{
				unsigned i1, i2, i3, j1, j2, j3;
				cin >> i1 >> i2 >> i3 >> j1 >> j2 >> j3;
				cout << bit.Sum(i1, i2, i3, j1 + 1, j2 + 1, j3 + 1) << endl;
			}
		}
	}
	return 0;
}
