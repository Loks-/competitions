#include "common/vector/utils.h"
#include "common/stl_base.h"

int main_transmutation()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned M;
		cin >> M;
		vector<vector<uint64_t>> receipts(M, vector<uint64_t>(M, 0));
		for (unsigned i = 0; i < M; ++i)
		{
			unsigned x, y;
			cin >> x >> y;
			receipts[i][x - 1] += 1;
			receipts[i][y - 1] += 1;
		}
		vector<uint64_t> available = ReadVector<uint64_t>(M);
		uint64_t res = 0;
		vector<bool> visited(M, false);
		vector<uint64_t> required(M, 0);
		required[0] = 1;
		for (bool stop = false; !stop;)
		{
			uint64_t possible = numeric_limits<uint64_t>::max();
			for (unsigned i = 0; i < M; ++i)
			{
				if (required[i] == 0)
					continue;
				possible = min(possible, available[i] / required[i]);
			}
			res += possible;
			for (unsigned i = 0; i < M; ++i)
			{
				available[i] -= possible * required[i];
			}
			for (unsigned i = 0; !stop && (i < M); ++i)
			{
				if (available[i] >= required[i])
					continue;
				visited[i] = true;
				for (unsigned j = 0; j < M; ++j)
				{
					uint64_t w = receipts[i][j];
					if (w == 0) continue;
					if (visited[j])
					{
						stop = true;
						break;
					}
					for (unsigned k = 0; k < M; ++k)
					{
						if (!visited[k])
							receipts[k][j] += w * receipts[k][i];
					}
					required[j] += w * required[i];
					available[j] += w * available[i];
				}
				for (unsigned k = 0; k < M; ++k)
				{
					if (!visited[k])
						receipts[k][i] = 0;
				}
				required[i] = available[i] = 0;
				break;
			}
		}
		cout << "Case #" << it << ": " << res << endl;
	}
	return 0;
}
