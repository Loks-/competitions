// https://www.hackerrank.com/challenges/hyperrectangle-gcd

#include "common/numeric/modular.h"
#include "common/numeric/primes_list_extended/eulers_totient.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_hyperrectangle_gcd__totient()
{
    TPLEEulersTotient primes_list(100000);
    primes_list.EulersTotientPrecalc();
    vector<TModular> v(100001);
    unsigned T, K;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> K;
        vector<unsigned> vnk = ReadVector<unsigned>(K);
        unsigned n0 = *min_element(vnk.begin(), vnk.end());
        for (unsigned i = 1; i <= n0; ++i)
        {
            v[i] = 1;
            for (unsigned nk : vnk)
                v[i] *= TModular(nk / i);
        }
        TModular s = 0;
        for (unsigned i = n0; i; --i)
            s += v[i] * TModular(primes_list.EulersTotient(i));
        cout << s.Get() << endl;
    }
	return 0;
}
