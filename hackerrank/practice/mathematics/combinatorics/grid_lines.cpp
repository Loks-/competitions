// https://www.hackerrank.com/challenges/grid-lines

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_grid_lines()
{
    TFactorial f;
    
    auto F = [&](unsigned n, unsigned m, unsigned k)
    {
        TModular s = f.BinomialCoefficient(m, k) * n + f.BinomialCoefficient(n, k) * m;
        for (unsigned dx = 1; dx * (k - 1) < n; ++dx)
        {
            for (unsigned dy = 1; dy * (k - 1) < m; ++dy)
            {
                if (GCD(dx, dy) != 1) continue;
                for (unsigned l = k - 1; (dx * l < n) && (dy * l < m); ++l)
                {
                    s += f.BinomialCoefficient(l - 1, k - 2) * (n - dx * l) * (m - dy * l) * 2;
                }
            }
        }
        return s;
    };

    unsigned n, m, k;
    cin >> n >> m >> k;
    cout << F(n + 1, m + 1, k) - F(n - 1, m - 1, k) << endl;
	return 0;
}
