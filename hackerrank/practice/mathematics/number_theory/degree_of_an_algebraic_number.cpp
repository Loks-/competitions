// https://www.hackerrank.com/challenges/degree-of-an-algebraic-number

#include "common/factorization/factorization.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/field/utils/rank.h"
#include "common/modular/modular.h"
#include "common/numeric/utils/pow.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

#include <set>

using TModular = Modular<2>;
using TMatrix = Matrix<TModular>;

int main_degree_of_an_algebraic_number()
{
    Factorization ff;
	unsigned T, N;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        set<unsigned> ps;
        cin >> N;
        vector<unsigned> va = ReadVector<unsigned>(N);
        vector<vector<unsigned>> vap;
        for (unsigned a : va)
        {
            TFactorization f = ff.Factorize(a);
            vector<unsigned> vp;
            for (auto p : f)
            {
                if (p.second & 1)
                {
                    ps.insert(unsigned(p.first));
                    vp.push_back(unsigned(p.first));
                }
            }
            vap.push_back(vp);
        }
        vector<unsigned> pv(ps.begin(), ps.end());
        TMatrix m(N, unsigned(ps.size()));
        for (unsigned i = 0; i < N; ++i)
        {
            for (unsigned p : vap[i])
            {
                unsigned j = unsigned(lower_bound(pv.begin(), pv.end(), p) - pv.begin());
                m(i, j) = 1;
            }
        }
        cout << PowU<uint64_t>(2, FMatrixRank(m)) << endl;
	}
	return 0;
}
