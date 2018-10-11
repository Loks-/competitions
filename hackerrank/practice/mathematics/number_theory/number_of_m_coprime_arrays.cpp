// https://www.hackerrank.com/challenges/number-of-m-coprime-arrays

#include "common/linear_algebra/matrix_static_size.h"
#include "common/numeric/modular.h"
#include "common/numeric/primes_list_extended/primes_powers.h"
#include "common/stl/base.h"

#include <numeric>

using TModular = Modular<>;
using TMatrix = TSquareMatrixStaticSize<TModular, 2>;

int main_number_of_m_coprime_arrays()
{
    PLEPrimesPowers primes_list(1000000);
    TMatrix m;
    m(0, 0) = m(0, 1) = 1;

	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, M;
        cin >> N >> M;
        vector<unsigned> fp = primes_list.GetPrimesPowers(M);
        TModular r = 1;
        for (unsigned q : fp)
        {
            m(1, 0) = q;
            r *= m.PowU(N + 1)(0, 0);
        }
        cout << r.Get() << endl;
	}
	return 0;
}
