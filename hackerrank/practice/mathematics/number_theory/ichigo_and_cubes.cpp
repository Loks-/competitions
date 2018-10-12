// https://www.hackerrank.com/challenges/ichigo-and-cubes

#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_ichigo_and_cubes()
{
    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t p, q, r;
        cin >> p >> q >> r;
        TModular mp(p), mq(q), mr(r);
        cout << ((mp * mq + mq * mr + mr * mp - 1) / 2).Get() << endl;
    }
	return 0;
}
