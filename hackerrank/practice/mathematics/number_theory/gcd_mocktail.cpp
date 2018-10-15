// https://www.hackerrank.com/challenges/gcd-mocktail

#include "common/modular/modular.h"
#include "common/stl/base.h"

using TModular = Modular<30000001>;

int main_gcd_mocktail()
{
    unsigned T, N, D, L, Q;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> N >> D >> Q;
        vector<TModular> v(N + 1);
        for (unsigned k = 1; k <= N; ++k)
            v[k] = TModular(N / k).PowU(D);
        for (unsigned k = N / 2; k; --k)
        {
            for (unsigned l = 2 * k; l <= N; l += k)
                v[k] -= v[l];
        }
        for (unsigned iQ = 0; iQ < Q; ++iQ)
        {
            cin >> L;
            TModular r = 0;
            for (unsigned k = 1; k <= N; ++k)
                r += v[k] * TModular(k).PowU(L);
            cout << r.Get() << endl;
        }
    }
	return 0;
}
