// https://www.hackerrank.com/challenges/subsequence-weighting

#include "common/binary_indexed_tree/bit_sparse.h"
#include "common/proxy/max.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TProxy = ProxyMax<uint64_t>;
using TBIT = BITSparse<TProxy>;

int main_subsequence_weighting__bit_sparse()
{
	uint64_t max_size = (1u << 31);
	unsigned T, N;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N;
		vector<uint64_t> va = ReadVector<uint64_t>(N), vw = ReadVector<uint64_t>(N);
		TBIT bit(max_size);
		for (unsigned i = 0; i < N; ++i)
		{
			TProxy pv = bit.Sum(va[i]);
			pv.Get() += vw[i];
			bit.Add(va[i], pv);
		}
		cout << bit.Sum(max_size).Get() << endl;
	}
	return 0;
}
