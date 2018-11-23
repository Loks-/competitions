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
    vector<uint64_t> vcurrent(maxn * maxn), vnext(maxn * maxn);
    vcurrent[0] = vnext[0] = (TModular(9)/TModular(10)).Get();
    for (unsigned d = 0; d < 10; ++d)
    {
        for (unsigned i = 0; i < maxn; ++i)
        {
            unsigned jmax = min(i, maxn - i);
            for (unsigned j = 1; j < jmax; ++j)
            {
                uint64_t fji = f.GetI(j).Get();
                // for (unsigned k = j; k < i; ++k)
                //     vnext[(i + j) * maxn + k] += (vcurrent[i * maxn + k] * fji) % M;
                for (uint64_t *pn = &(vnext[(i + j) * maxn + j]), *pn_end = pn + (i - j), *pc = &(vcurrent[i * maxn + j]); pn != pn_end; )
                    *pn++ += (*pc++ * fji) % M;
                uint64_t s = (vcurrent[i * (maxn + 1)] * fji) % M;
                // for (unsigned k = i; k <= i + j; ++k)
                //     vnext[(i + j) * maxn + k] += s;
                for (uint64_t *pn = &(vnext[(i + j) * maxn + i]), *pn_end = pn + j + 1; pn != pn_end; )
                    *pn++ += s;
            }
            for (unsigned j = max(i, 1u); i + j < maxn; ++j)
            {
                uint64_t fji = f.GetI(j).Get();
                uint64_t s = (vcurrent[i * (maxn + 1)] * fji) % M;
                // for (unsigned k = j; k <= i + j; ++k)
                //     vnext[(i + j) * maxn + k] += s;
                for (uint64_t *pn = &(vnext[(i + j) * maxn + j]), *pn_end = pn + i + 1; pn != pn_end; )
                    *pn++ += s;
            }
        }
        for (unsigned i = 0; i < maxn; ++i)
        {
            for (unsigned k = 0; k <= i; ++k)
                vcurrent[i * maxn + k] = (vnext[i * maxn + k] %= M);
        }
    }

    unsigned T, n, k;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        cout << (vcurrent[n * maxn + min(n, k)] * f.Get(n).Get()) % M << endl;
    }
	return 0;
}
