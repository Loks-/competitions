// https://www.hackerrank.com/challenges/sherlock-and-moving-tiles

#include "common/stl_base.h"

#include <cmath>
#include <iomanip>

int main_sherlock_and_moving_tiles()
{
	cout << setprecision(4) << fixed;
	int64_t l, s1, s2, ds, Q, q;
	cin >> l >> s1 >> s2 >> Q;
	ds = abs(s1 - s2);
	double ds2 = sqrt((double)(2.0));
	for (unsigned i = 0; i < Q; ++i)
	{
		cin >> q;
		double x = sqrt(double(q));
		double t = (l - x) / ds * ds2;
		cout << t << endl;
	}
	return 0;
}
