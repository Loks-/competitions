// https://www.hackerrank.com/challenges/bus-station

#include "common/base.h"
#include "common/numeric/primes_list.h"

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
	PrimesList primes(100000);
	vector<uint64_t> vd = primes.GetDivisors(s);
	sort(vd.begin(), vd.end());
	for (uint64_t d : vd)
	{
		if (d < m) continue;
		bool good = true;
		for (uint64_t t = d; t < s; t += d)
		{
			if (us.find(t) == us.end())
			{
				good = false;
				break;
			}
		}
		if (good)
			cout << d << " ";
	}
	cout << endl;
	return 0;
}
