// https://www.hackerrank.com/challenges/polynomial-division

#include "common/binary_indexed_tree/bit.h"
#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TBIT = BIT<TModular>;

int main_polynomial_division()
{
	unsigned n, a, b, q, c;
	cin >> n >> a >> b >> q;
	TModular x = -TModular(b) / TModular(a);
	vector<TModular> vx(n), vc(n);
	vx[0] = 1;
	for (unsigned i = 1; i < n; ++i)
		vx[i] = vx[i - 1] * x;
	TBIT bit(n);
	for (unsigned i = 0; i < n; ++i)
	{
		cin >> c;
		vc[i] = c;
		bit.Add(i, vx[i] * vc[i]);
	}
	for (unsigned it = 0; it < q; ++it)
	{
		unsigned t, l, r;
		cin >> t >> l >> r;
		if (t == 1)
		{
			TModular cnew = r;
			bit.Add(l, vx[l] * (cnew - vc[l]));
			vc[l] = cnew;
		}
		else if (t == 2)
		{
			if (x.Get() != 0)
				cout << ((bit.Sum(l, r + 1).Get() == 0) ? "Yes" : "No") << endl;
			else
				cout << ((vc[l] == 0) ? "Yes" : "No") << endl;
		}
	}
	return 0;
}
