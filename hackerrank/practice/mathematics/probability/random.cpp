// https://www.hackerrank.com/challenges/random

#include "common/stl/base.h"

#include <cmath>
#include <iomanip>

int main_random()
{
	cout << setprecision(6) << fixed;
	unsigned n, a, b;
	cin >> n >> a >> b;
	vector<double> v(n), vt(n);
	double s = 0, m;
	for (unsigned i = 0; i < n; ++i)
	{
		cin >> v[i];
		s += v[i];
	}
	m = s / n;
	double w0 = pow((n - 3.0) / (n - 1.0), a);
	for (unsigned i = 0; i < n; ++i)
		v[i] = w0 * v[i] + (1 - w0) * m;
	for (unsigned ib = 0; ib < b; ++ib)
	{
		vt.swap(v);
		for (unsigned i = 0; i < n; ++i)
		{
			v[i] = (i * (i - 1) / 2.0 + (n - i - 1) * (n - i - 2) / 2.0 - 1.0) * vt[i];
			for (unsigned j = 0; j < n; ++j)
				v[i] += (min(min(i, j) + 1, n - max(i, j))) * vt[j];
			v[i] /= (n * (n - 1)) / 2.0;
		}
	}
	s = 0;
	for (unsigned i = 0; i < n; ++i)
		s += v[i] * ((i + 1) * (n - i) - 1);
	s /= (n * (n - 1)) / 2.0;
    cout << s << endl;
	return 0;
}
