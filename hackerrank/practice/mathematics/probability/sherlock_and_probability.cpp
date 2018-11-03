// https://www.hackerrank.com/challenges/sherlock-and-probability

#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

#include <string>

int main_sherlock_and_probability()
{
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
		uint64_t K, N;
		cin >> N >> K;
		string s;
		cin >> s;
		uint64_t S1 = 0, S2 = N * N;
		vector<unsigned> v(1, 0);
		for (char c : s)
			v.push_back(v.back() + ((c == '1') ? 1 : 0));
		for (uint64_t i = 0; i < N; ++i)
		{
			if (s[i] == '0') continue;
			uint64_t j1 = max(i, K) - K;
			uint64_t j2 = min(i + K + 1, N);
			S1 += v[j2] - v[j1];
		}
		uint64_t k = GCD(S1, S2);
		S1 /= k; S2 /= k;
		cout << S1 << "/" << S2 << endl;
	}
	return 0;
}
