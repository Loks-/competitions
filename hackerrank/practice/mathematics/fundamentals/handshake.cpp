// https://www.hackerrank.com/challenges/handshake

#include "common/stl_base.h"

int main_handshake()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t n;
		cin >> n;
		cout << (n * (n - 1)) / 2 << endl;
	}
	return 0;
}
