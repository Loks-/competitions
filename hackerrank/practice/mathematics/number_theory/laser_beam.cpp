// https://www.hackerrank.com/challenges/laser-beam

#include "common/factorization/primes_list.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

#include <unordered_map>

using TModular = Modular<>;

int main_laser_beam()
{
    PrimesList primes_list(60000);
    uint64_t N0 = 10;
    vector<TModular> vf(N0), vg(N0);
    for (uint64_t i = 1; i < N0; ++i)
        vf[i] = 24 * i * i + 2;
    for (uint64_t i = 1; 2 * i < N0; ++i)
    {
        for (uint64_t j = 2 * i; j < N0; j += i)
            vf[j] -= vf[i];
        vg[i] = vg[i - 1] + vf[i];
    }

    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t N, M, D;
        cin >> N >> M >> D;
        uint64_t NM = N / M, maxk = 0;
        unordered_map<uint64_t, TModular> mg, mgt;
        if (D == 1)
        {
            mg[NM] = 1;
            maxk = NM;
        }
        else
        {
            uint64_t l = min(USqrt(N), NM);
            for (uint64_t k = 1;; ++k)
            {
                uint64_t kd = k * D, i1 = N / kd, i0 = N / (kd + 1);
                i0 = min(i0, NM), i1 = min(i1, NM);
                if (i1 < N0) break;
                if (i0 == i1) continue;
                maxk = max(maxk, i1);
                mg[i1] += 1;
                mg[i0] -= 1;
                l = min(l, i0);
            }
            for (uint64_t i = 1; i <= l; ++i)
            {
                if (((N / i) % D) == 0)
                {
                    mg[i - 1] -= 1;
                    mg[i] += 1;
                    maxk = max(maxk, i);
                }
            }
        }
        cout << maxk << endl;
        // cout << r + Solve() << endl;
    }
    return 0;
}
