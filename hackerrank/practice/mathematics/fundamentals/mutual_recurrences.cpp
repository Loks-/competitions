// https://www.hackerrank.com/challenges/mutual-recurrences

#include "common/numeric/modular.h"
#include "common/matrix/matrix_static_size.h"
#include "common/stl_base.h"

using TModular = TModular_C32<1000000000>;
using TMatrix = TSquareMatrixStaticSize<TModular, 22>;

int main_mutual_recurrences()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		unsigned a, b, c, d, e, f, g, h;
		uint64_t n;
		cin >> a >> b >> c >> d >> e >> f >> g >> h >> n;
		TMatrix trans;
		trans(0, 0) = d;
		trans(1, 0) = d;
		trans(1, 1) = d;
		trans(2, 2) = h;
		trans(3, 2) = h;
		trans(3, 3) = h;
		// x:  4-12
		// y: 13-21
		for (unsigned i = 0; i < 8; ++i)
		{
			trans(i + 4, i + 5) = 1;
			trans(i + 13, i + 14) = 1;
		}
		trans(12, 1) = 1;
		trans(12, 13 - a) += 1;
		trans(12, 22 - b) += 1;
		trans(12, 22 - c) += 1;
		trans(21, 3) = 1;
		trans(21, 22 - e) += 1;
		trans(21, 13 - f) += 1;
		trans(21, 13 - g) += 1;
		TMatrix tf = trans.PowU(n + 1);
		array<TModular, 22> vf;
		vf.fill(TModular(1));
		vf[1] = vf[3] = 0;
		TModular x(0), y(0);
		for (unsigned i = 0; i < 22; ++i)
		{
			x += tf(12, i) * vf[i];
			y += tf(21, i) * vf[i];
		}
		cout << x.Get() << " " << y.Get() << endl;
	}
	return 0;
}
