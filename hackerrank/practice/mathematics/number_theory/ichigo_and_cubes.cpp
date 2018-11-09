// https://www.hackerrank.com/challenges/ichigo-and-cubes

#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_ichigo_and_cubes()
{
    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        TModular p, q, r;
        cin >> p >> q >> r;
        cout << (p * q + q * r + r * p - 1) / 2 << endl;
    }
	return 0;
}
