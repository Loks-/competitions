// https://www.hackerrank.com/challenges/div-and-span

#include "common/modular/static/catalan.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TCatalan = ModularCatalan<TModular>;

int main_div_and_span()
{
    TCatalan c;
    unsigned N = 200002, M = 201;
    vector<TModular> vd(N * M);
    vd[0] = 1;
    for (unsigned i = 1; i < N; ++i)
    {
        vd[i * M] = 1;
        for (unsigned j = 1; j < M; ++j)
        {
            TModular s = 0;
            for (unsigned k = 0; k <= j; ++k)
                s += c(j - k) * vd[(i-1) * M + k];
            vd[i * M + j] = s;
        }
    }

    unsigned T, X, Y;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> X >> Y;
        cout << c(X) * vd[(2*X + 1) * M + Y] * c.GetF(X) * c.GetF(Y) << endl;
    }
	return 0;
}
