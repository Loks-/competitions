// https://www.hackerrank.com/challenges/permutation-problem

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_permutation_problem()
{
    const uint64_t M = TModular::GetMod();
    TFactorial f;
    unsigned maxd = 11, maxn = 1001;
    vector<vector<vector<TModular>>> v(maxd, vector<vector<TModular>>(maxn, vector<TModular>(maxn)));
    v[0][0][0] = TModular(9)/TModular(10);
    for (unsigned d = 1; d < maxd; ++d)
    {
        for (unsigned k = 0; k < maxn; ++k)
            v[d][0][k] = v[0][0][0];
        for (unsigned n = 1; n < maxn; ++n)
        {
            for (unsigned k = 1; k <=n; ++k)
            {
                TModular s = 0, fik = f.GetI(k), fikpe = 1;
                unsigned nek = n;
                for (unsigned e = 0; e <= d; ++e)
                {
                    if (e * k > n) break;
                    s += f.BinomialCoefficient(d, e) * fikpe * v[d - e][nek][min(nek, k - 1)];
                    fikpe *= fik;
                    nek -= k;
                }
                v[d][n][k] = s;
            }
        }
    }

    unsigned T, n, k;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        cout << v[10][n][min(n, k)] * f.Get(n) << endl;
    }
	return 0;
}
