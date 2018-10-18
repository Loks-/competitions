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
    unsigned T, N, K;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t M;
        cin >> N >> K >> M;
        TPolynom p = GetSumOfPowers<TModular>(K);
        vector<uint64_t> vs = ReadVector<uint64_t>(N);
        vector<TModular> vspk;
        for (uint64_t s : vs)
            vspk.push_back(TModular(s).PowU(K));

        std::function<TModular(uint64_t, unsigned)> SolveR = [&](uint64_t rM, unsigned i) -> TModular
        {
            if (rM == 0) return 0;
            if (i >= N) return p.Apply(TModular(rM));
            return SolveR(rM, i + 1) - SolveR(rM / vs[i], i + 1) * vspk[i];
        };

        cout << SolveR(M, 0).Get() << endl;
    }
	return 0;
}
