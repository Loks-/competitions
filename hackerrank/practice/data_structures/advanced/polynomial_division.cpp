// https://www.hackerrank.com/challenges/polynomial-division

#include "common/binary_indexed_tree/bit.h"
#include "common/modular/static/modular.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TBIT = BIT<TModular>;

int main_polynomial_division()
{
	unsigned n, a, b, q, c;
	cin >> n >> a >> b >> q;
	TModular x = -TModular(b) / TModular(a);
	if (x.Get())
	{
		vector<TModular> vx(n);
		vx[0] = 1;
		for (unsigned i = 1; i < n; ++i)
			vx[i] = vx[i - 1] * x;
		TBIT bit(n);
		for (unsigned i = 0; i < n; ++i)
		{
			cin >> c;
			bit.Add(i, vx[i] * c);
		}
		for (unsigned it = 0; it < q; ++it)
		{
			unsigned t, l, r;
			cin >> t >> l >> r;
			if (t == 1)
				bit.Add(l, vx[l] * TModular(r) - bit.Get(l));
			else if (t == 2)
				cout << ((bit.Sum(l, r + 1) == 0) ? "Yes" : "No") << endl;
		}
	}
	else
	{
		vector<unsigned> vc = ReadVector<unsigned>(n);
		for (unsigned it = 0; it < q; ++it)
		{
			unsigned t, l, r;
			cin >> t >> l >> r;
			if (t == 1)
				vc[l] = r;
			else if (t == 2)
				cout << ((vc[l] == 0) ? "Yes" : "No") << endl;
		}
	}
	return 0;
}
