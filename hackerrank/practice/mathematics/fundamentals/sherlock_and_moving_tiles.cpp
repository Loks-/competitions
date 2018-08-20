// https://www.hackerrank.com/challenges/sherlock-and-moving-tiles

#include "common/stl_base.h"

#include <cmath>

int main_sherlock_and_moving_tiles()
{
	int64_t l, s1, s2, ds, Q, q;
	cin >> l >> s1 >> s2 >> Q;
	ds = abs(s1 - s2);
	double ds2 = sqrt((double)(2.0));
	for (unsigned i = 0; i < Q; ++i)
	{
		cin >> q;
		double x = sqrt(double(q));
		double t = (l - x) / ds * ds2;
		printf("%.6lf\n", t);
	}
	return 0;
}
