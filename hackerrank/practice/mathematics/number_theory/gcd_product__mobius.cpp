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
    for (uint64_t i = 2; i <= m; ++i)
    {
        uint64_t ni = n / i, mi = m / i;
        int64_t power = 0;
        for (uint64_t j = 1; j <= mi; ++j)
            power += int64_t(primes_list.Mobius(j)) * int64_t((ni / j) * (mi / j));
        r *= TModular(i).PowS(power);
    }
    cout << r.Get() << endl;
	return 0;
}
