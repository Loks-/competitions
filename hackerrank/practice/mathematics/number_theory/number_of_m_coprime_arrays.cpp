// https://www.hackerrank.com/challenges/number-of-m-coprime-arrays

#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/vector.h"
#include "common/numeric/modular.h"
#include "common/numeric/primes_list_extended/primes_powers.h"
#include "common/stl/base.h"

#include <numeric>

using TModular = Modular<>;
using TVector = Vector<TModular>;
using TMatrix = Matrix<TModular>;

int main_number_of_m_coprime_arrays()
{
    PLEPrimesPowers primes_list(1000000);
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, M;
        cin >> N >> M;
        vector<unsigned> fp = primes_list.GetPrimesPowers(M);
        cout << fp.size() << "\t" << M << endl;

        unsigned p2fp = (1u << fp.size());
        vector<uint64_t> vc(p2fp, 1);
        for (unsigned i = 0; i < p2fp; ++i)
        {
            for (unsigned j = 0; j < fp.size(); ++j)
            {
                if (i & (1u << j))
                    vc[i] *= fp[j];
            }
        }

        TVector v(p2fp, 0); v(0) = 1;
        TMatrix m(p2fp, p2fp);
        for (unsigned i = 0; i < p2fp; ++i)
        {
            TModular im(vc[i]);
            for (unsigned j = 0; j < p2fp; ++j)
            {
                if ((i & j) == 0)
                    m(i, j) = im;
            }
        }
        TVector vf = m.PowU(N) * v;
        cout << accumulate(vf.begin(), vf.end(), TModular(0)).Get() << endl;
	}
	return 0;
}
