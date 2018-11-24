// https://www.hackerrank.com/challenges/permutation-problem

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_permutation_problem()
{
    const uint64_t M = TModular::GetMod();
    TFactorial f;
    unsigned maxn = 1001;
    vector<uint64_t> v(maxn * maxn);
    v[0] = (TModular(9)/TModular(10)).Get();
    for (unsigned d = 0; d < 10; ++d)
    {
        for (unsigned ij = maxn; ij--; )
        {
            for (unsigned j = 1; 2 * j < ij; ++j)
            {
                uint64_t fji = f.GetI(j).Get();
                unsigned i = ij - j;
                for (unsigned k = j; k < i; ++k)
                    v[ij * maxn + k] += (v[i * maxn + k] * fji) % M;
            }
            uint64_t t = 0;
            for (unsigned k = (ij + 1) / 2; k <= ij; ++k)
            {
                t += (k < ij) ? v[k * (maxn + 1)] * f.GetI(ij - k).Get() : 0ull;
                t += ((k > 0) && (2 * k != ij)) ? v[(ij - k) * (maxn + 1)] * f.GetI(k).Get() : 0ull;
                t %= M;
                v[ij * maxn + k] += t;
            }
            for (unsigned k = 0; k <= ij; ++k)
                v[ij * maxn + k] %= M;
        }
    }

    unsigned T, n, k;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        cout << (v[n * maxn + min(n, k)] * f.Get(n).Get()) % M << endl;
    }
	return 0;
}
