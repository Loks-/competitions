// https://www.hackerrank.com/challenges/random-number-generator

#include "common/numeric/fraction_io.h"
#include "common/stl/base.h"

int main_random_number_generator()
{
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
		unsigned A, B, C;
		cin >> A >> B >> C;
		unsigned S1 = 0, S2 = 2 * A * B;
		if (A > B) swap(A, B);
		if (C >= A + B)
			S1 = S2;
		else if (C <= A)
			S1 = C * C;
		else
		{
			unsigned d = C - A;
			S1 += 2 * d * A;
			B -= d;
			C -= d;
			if (C <= B)
				S1 += C * C;
			else
			{
				unsigned e = A + B - C;
				S1 += 2 * A * B - e * e;
			}
		}
		cout << TIFraction(S1, S2) << endl;
	}
	return 0;
}
