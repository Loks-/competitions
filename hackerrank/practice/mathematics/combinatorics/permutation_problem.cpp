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
    unsigned maxn = 1001;
    vector<uint64_t> vcurrent(maxn * maxn), vnext(maxn * maxn);
    vcurrent[0] = 1;
    for (unsigned d = 0; d < 10; ++d)
    {
        fill(vnext.begin(), vnext.end(), 0);
        for (unsigned i = 0; i < maxn; ++i)
        {
            unsigned jmax = min(i, maxn - i);
            for (unsigned j = 0; j < jmax; ++j)
            {
                uint64_t fji = f.GetI(j).Get();
                for (unsigned k = j; k < i; ++k)
                    vnext[(i + j) * maxn + k] += (vcurrent[i * maxn + k] * fji) % M;
                uint64_t s = (vcurrent[i * (maxn + 1)] * fji) % M;
                for (unsigned k = i; k <= i + j; ++k)
                    vnext[(i + j) * maxn + k] += s;
            }
            for (unsigned j = i; i + j < maxn; ++j)
            {
                uint64_t fji = f.GetI(j).Get();
                uint64_t s = (vcurrent[i * (maxn + 1)] * fji) % M;
                for (unsigned k = j; k <= i + j; ++k)
                    vnext[(i + j) * maxn + k] += s;
            }
        }
        for (unsigned i = 0; i < maxn; ++i)
        {
            for (unsigned k = 0; k <= i; ++k)
                vnext[i * maxn + k] %= M;
        }
        vcurrent.swap(vnext);
    }

    unsigned T, n, k;
    cin >> T;
    TModular m = TModular(9) / TModular(10);
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        cout << m * vcurrent[n * maxn + min(n, k)] * f.Get(n) << endl;
    }
	return 0;
}
