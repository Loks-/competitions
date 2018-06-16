// https://www.hackerrank.com/challenges/halloween-party

#include "common/base.h"

int main_halloween_party()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t K;
		cin >> K;
		cout << (K / 2) * ((K + 1) / 2) << endl;
	}
	return 0;
}
