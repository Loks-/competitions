// https://www.hackerrank.com/challenges/special-multiple

#include "common/stl/base.h"

int main_special_multiple()
{
	int T, N;
	cin >> T;
	for (; T; --T)
	{
		cin >> N;
		unsigned p2 = 0, p3 = 0, p5 = 0;
		for (; (N % 2) == 0; N /= 2, ++p2);
		for (; ((N % 3) == 0) && (p3 < 2); N /= 3, ++p3);
		for (; (N % 5) == 0; N /= 5, ++p5);
		unsigned z = max(p2, p5);
		cout << "9";
		if (N > 1)
		{
			vector<int> vl(N, -1), vf(N, -1);
			vl[1] = 0; vf[1] = 0;
			for (int c = 10 % N, p = 1; vl[0] == -1; c = (c * 10) % N, ++p)
			{
				for (int j = 0; j < N; ++j)
				{
					if ((vl[j] != -1) && (vl[j] != p))
					{
						int k = (j + c) % N;
						if (vl[k] == -1)
						{
							vl[k] = p;
							vf[k] = j;
						}
					}
				}
			}
			for (int p = vl[0], i = 0; vf[i];)
			{
				i = vf[i];
				for (--p; p > vl[i]; --p) cout << "0";
				cout << "9";
			}
		}
		for (; z; --z)
			cout << "0";
		cout << endl;
	}
	return 0;
}
