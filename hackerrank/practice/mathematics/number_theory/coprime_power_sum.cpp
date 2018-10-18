// https://www.hackerrank.com/challenges/coprime-power-sum

#include "common/modular/modular.h"
#include "common/polynomial/base_newton_polynomial.h"
#include "common/polynomial/utils/sum_of_powers.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <functional>

using TModular = Modular<>;
using TPolynom = BaseNewtonPolynomial<TModular>;

int main_coprime_power_sum()
{
    unsigned T, N, K, maxn = 50, pk_cache_size = 10000;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t M;
        cin >> N >> K >> M;
        TPolynom p = GetSumOfPowers<TModular>(K);
        vector<uint64_t> vs = ReadVector<uint64_t>(N);
        sort(vs.begin(), vs.end());
        vs.push_back(M + 1);
        vector<TModular> vspk;
        for (uint64_t s : vs)
            vspk.push_back(TModular(s).PowU(K));
        vector<TModular> cache_pk(pk_cache_size);
        for (unsigned i = 1; i < pk_cache_size; ++i)
            cache_pk[i] = cache_pk[i-1] + TModular(i).PowU(K);

        std::function<TModular(uint64_t, unsigned)> SolveR = [&](uint64_t rM, unsigned i) -> TModular
        {
            return (vs[i] > rM) ? (rM < pk_cache_size ? cache_pk[rM] : p.Apply(TModular(rM))) : SolveR(rM, i + 1) - SolveR(rM / vs[i], i + 1) * vspk[i];
        };

        cout << ((vs[0] == 1) ? uint64_t(0) : SolveR(M, 0).Get()) << endl;
    }
	return 0;
}
