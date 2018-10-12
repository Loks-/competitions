// https://www.hackerrank.com/challenges/bus-station

#include "common/factorization/utils/divisors.h"
#include "common/factorization/utils/factorize.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_bus_station()
{
	unsigned N;
	uint64_t x, s = 0, m = 0;
	cin >> N;
	unordered_set<uint64_t> us;
	for (unsigned it = 0; it < N; ++it)
	{
		cin >> x;
		s += x;
		m = max(m, x);
		us.insert(s);
	}
	vector<uint64_t> vd = GetDivisors(Factorize(s));
	sort(vd.begin(), vd.end());
	for (uint64_t d : vd)
	{
		if (d < m) continue;
		bool good = true;
		for (uint64_t t = d; good && (t < s); t += d)
		{
			if (us.find(t) == us.end())
				good = false;
		}
		if (good)
			cout << d << " ";
	}
	cout << endl;
	return 0;
}
