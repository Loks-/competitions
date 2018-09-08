#include "common/stl/base.h"

int main_rounding_error()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned N, L, x, s = 0, sp = 0;
		cin >> N >> L;
		vector<unsigned> vr;
		unsigned r1 = (100 % N);
		for (unsigned i = 0; i < L; ++i)
		{
			cin >> x;
			s += x;

			unsigned q = (100 * x) / N, r = (100 * x) % N;
			sp += q;
			if (2 * r < N)
			{
				if (r1 > 0)
				{
					unsigned t = (N + 1) / 2 - r;
					vr.push_back((t - 1) / r1 + 1);
				}
			}
			else
				++sp;
		}
		sp += (100 / N) * (N - s);
		s = N - s;
		if (r1 > 0)
		{
			unsigned t = ((N + 1) / 2 - 1) / r1 + 1;
			sort(vr.begin(), vr.end());
			for (unsigned i = 0; i < vr.size(); ++i)
			{
				if (vr[i] >= t)
					break;
				if (vr[i] > s)
					break;
				sp += 1; s -= vr[i];
			}
			sp += s / t;
		}
		cout << "Case #" << it << ": " << sp << endl;
	}
	return 0;
}
