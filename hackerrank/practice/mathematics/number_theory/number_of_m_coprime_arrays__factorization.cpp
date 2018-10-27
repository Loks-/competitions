// https://www.hackerrank.com/challenges/number-of-m-coprime-arrays

#include "common/factorization/factorization.h"
#include "common/linear_algebra/matrix_static_size.h"
#include "common/modular/modular.h"
#include "common/stl/base.h"

#include <numeric>

using TModular = Modular<>;
using TMatrix = TSquareMatrixStaticSize<TModular, 2>;

int main_number_of_m_coprime_arrays__factorization()
{
    Factorization ff;
    TMatrix m;
    m(0, 0) = m(0, 1) = 1;

	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, M;
        cin >> N >> M;
        TFactorization fp = ff.Factorize(M);
        TModular r = 1;
        for (auto q : fp)
        {
            m(1, 0) = q.second;
            r *= m.PowU(N + 1)(0, 0);
        }
        cout << r.Get() << endl;
	}
	return 0;
}
