// https://www.hackerrank.com/challenges/most-distant

#include "common/stl_base.h"

#include <cmath>

int main_most_distant()
{
	unsigned N;
	double x, xl = 1e10, xr = -1e10, y, yl = 1e10, yh = -1e10;
	cin >> N;
	for (unsigned it = 0; it < N; ++it)
	{
		cin >> x >> y;
		xl = min(x, xl);
		xr = max(x, xr);
		yl = min(y, yl);
		yh = max(y, yh);
	}
	double max_distance = max(xr - xl, yh - yl);
	max_distance = max(max_distance, sqrt(max(xl*xl, xr*xr) + max(yl*yl, yh*yh)));
	printf("%.6lf", max_distance);
	return 0;
}
