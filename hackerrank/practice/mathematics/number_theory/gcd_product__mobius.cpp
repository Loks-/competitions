// https://www.hackerrank.com/challenges/gcd-product

#include "common/numeric/modular.h"
#include "common/numeric/primes_list_extended/mobius.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_product__mobius()
{
    uint64_t n, m;
    cin >> n >> m;
    if (n < m)
        swap(n, m);
    TModular r = 1;
    TPLEMobius primes_list(m / 2);
    primes_list.MobiusPrecalc();
    uint64_t lni = 0, lmi = 0, power = 0;
    TModular base = 1;
    for (uint64_t i = 2; i <= m; ++i)
    {
        uint64_t ni = n / i, mi = m / i;
        if ((ni == lni) && (mi == lmi))
        {
            base *= TModular(i);
            continue;
        }
        r *= (power == 1) ? base : base.PowU(power);
        base = i;
        power = 0;
        lni = ni; lmi = mi;
        for (uint64_t j = 1; j <= mi; ++j)
            power += uint64_t(primes_list.Mobius(j)) * ((ni / j) * (mi / j));
        power %= 1000000006;
    }
    r *= base.PowS(power);
    cout << r.Get() << endl;
	return 0;
}
