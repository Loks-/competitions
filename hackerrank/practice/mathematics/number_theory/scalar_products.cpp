// https://www.hackerrank.com/challenges/scalar-products

#include "common/stl_base.h"

#include <unordered_set>

int main_scalar_products()
{
	uint64_t C, M, n;
	cin >> C >> M >> n;
	C %= M;
	vector<uint64_t> s(2*n + 3);
	vector<uint64_t> s2(2 * n + 3);
	s[0] = 0;
	s[1] = C;
	s2[0] = 0;
	s2[1] = (C*C) % M;
	for (size_t i = 2; i < s.size(); ++i)
	{
		s[i] = (s[i - 1] + s[i - 2]) % M;
		s2[i] = (s[i] * s[i]) % M;
	}
	unordered_set<uint64_t> us;
	for (int kl = 3; kl < 2 * n; ++kl)
	{
		us.insert((s2[kl + 1] + s2[kl]) % M);
	}
	cout << us.size() << endl;
	return 0;
}
