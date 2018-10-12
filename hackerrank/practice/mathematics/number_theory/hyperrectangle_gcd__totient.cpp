// https://www.hackerrank.com/challenges/hyperrectangle-gcd

#include "common/modular/modular.h"
#include "common/factorization/primes_list_extended/eulers_totient.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_hyperrectangle_gcd__totient()
{
    TPLEEulersTotient primes_list(100000);
    primes_list.EulersTotientPrecalc();
    unsigned T, K;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> K;
        vector<unsigned> vnk = ReadVector<unsigned>(K);
        unsigned n0 = *min_element(vnk.begin(), vnk.end());
        TModular s = 0;
        for (unsigned i = 1; i <= n0; ++i)
        {
            TModular t = primes_list.EulersTotient(i);
            for (unsigned nk : vnk)
                t *= TModular(nk / i);
            s += t;
        }
        cout << s.Get() << endl;
    }
	return 0;
}
