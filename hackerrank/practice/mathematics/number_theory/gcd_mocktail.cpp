// https://www.hackerrank.com/challenges/gcd-mocktail

#include "common/modular/modular.h"
// #include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

using TModular = Modular<30000001>;

int main_gcd_mocktail()
{
    unsigned T, N, D, L, Q;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> N >> D >> Q;
        // unsigned sN = USqrt<unsigned>(N);
        vector<unsigned> vk;
        vector<TModular> vc;
        // O(T * (N + 2 * sqrt(N) log D) ) 
        for (unsigned k = 1; k <= N + 1; ++k)
        {
            if ((k > 1) && (N / k == N / (k-1)))
                continue;
            vk.push_back(k);
            vc.push_back(TModular(N / k).PowU(D));
        }
        // O(T * N)
        for (unsigned i = vk.size(); i--; )
        {
            unsigned k = vk[i], m = 1, mnext;
            for (unsigned j = i + 1; j + 1 < vk.size(); ++j)
            {
                mnext = (vk[j + 1] - 1) / k;
                vc[i] -= TModular(mnext - m) * vc[j];
                m = mnext;
            }
        }
        // O(T * Q * N * log L)
        for (unsigned iQ = 0; iQ < Q; ++iQ)
        {
            cin >> L;
            TModular r = 0;
            for (unsigned i = 0, k = 1; k <= N; ++k)
            {
                if (vk[i + 1] <= k)
                    ++i;
                r += vc[i] * TModular(k).PowU(L);
            }
            cout << r.Get() << endl;
        }
    }
	return 0;
}
