// https://www.hackerrank.com/challenges/gcd-product

#include "common/numeric/modular.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_gcd_product__counting()
{
    uint64_t n, m;
    cin >> n >> m;
    if (n < m)
        swap(n, m);
    vector<uint64_t> vc(m + 1);
    for (uint64_t i = 1; i <= m; ++i)
        vc[i] = (n / i) * (m / i);
    TModular r = 1;
    for (uint64_t i = m; i > 1; --i)
    {
        for (uint64_t j = 2 * i; j <= m; j += i)
            vc[i] -= vc[j];
        r *= TModular(i).PowU(vc[i]);
    }
    cout << r.Get() << endl;
	return 0;
}
