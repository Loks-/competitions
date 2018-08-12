// https://www.hackerrank.com/challenges/reverse-game

#include "common/base.h"

int main_reverse_game()
{
	unsigned T, N, K;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N >> K;
		if (K < N / 2)
			cout << 2 * K + 1 << endl;
		else
			cout << 2 * (N - K - 1) << endl;
	}
	return 0;
}
