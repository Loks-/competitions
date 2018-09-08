// https://www.hackerrank.com/challenges/filling-jars

#include "common/stl/base.h"

int main_filling_jars()
{
	uint64_t N, M, T = 0, a, b, k;
	cin >> N >> M;
	for (uint64_t it = 0; it < M; ++it)
	{
		cin >> a >> b >> k;
		T += (b - a + 1) * k;
	}
	cout << (T / N) << endl;
	return 0;
}
