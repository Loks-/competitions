#include "common/stl_base.h"

#include <map>

int main_bathroom_stalls()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		uint64_t N, K;
		cin >> N >> K;
		map<uint64_t, uint64_t> m;
		m[N] = 1;
		uint64_t l, k;
		for (; ;)
		{
			auto it = m.rbegin();
			l = it->first;
			k = it->second;
			if (k >= K)
				break;
			K -= k;
			m.erase(l);
			m[l / 2] += k;
			m[(l - 1) / 2] += k;
		}
		cout << "Case #" << it << ": " << l / 2 << " " << (l - 1) / 2 << endl;
	}
	return 0;
}
