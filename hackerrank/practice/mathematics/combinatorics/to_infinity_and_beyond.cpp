// https://www.hackerrank.com/challenges/to-infinity-and-beyond

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_to_infinity_and_beyond()
{
    TFactorial f;
	unsigned T;
	cin >> T;
	uint64_t x, y, z;
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> x >> y >> z;
		unsigned mk = unsigned(min(x, y));
		TModular s(0), v = f.GetI(mk);
		for (unsigned l = 1; l <= mk; ++l)
			v *= TModular(x + y + 1 - l);
		TModular a(1), b(1), c(1);
		v -= a * b;
		s += v;
		for (unsigned l = 1; l < mk; ++l)
		{
			a *= TModular(x + 1 - l);
			b *= TModular(y + 1 - l);
			c *= TModular(z - l);
			v -= a*b*f.GetI(l)*f.GetI(l);
			s += v*c*f.GetI(l);
		}
		cout << s << endl;
	}
	return 0;
}
