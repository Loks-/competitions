// https://www.hackerrank.com/challenges/random-number-generator-1

#include "common/stl/base.h"

#include <iomanip>

int main_random_number_generator2()
{
    cout << setprecision(9) << fixed;
	unsigned maxN = 1000002;
	vector<double> vH(1, 0.);
	for (; vH.size() <= maxN; )
		vH.push_back(vH.back() + 1.0 / vH.size());
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
		size_t N;
		cin >> N;
		size_t l = 0, h = N;
		for (; h - l > 1; )
		{
			size_t m = (l + h) / 2;
			double c = (N - m - 1.0) / 2.0 / (vH[N] - vH[m - 1]);
			double pm = 0.5 - c / m;
			if (pm > 0)
				h = m;
			else
				l = m;
		}
		double c = (N - h - 1.0) / 2.0 / (vH[N] - vH[h - 1]);
		double d = (N + h) * (N - h + 1) / 4.0 - 2 * c * c * (vH[N] - vH[h - 1]);
		d /= (2 * N - 1);
        cout << d << endl;
	}
	return 0;
}
