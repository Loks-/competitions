// https://www.hackerrank.com/challenges/polita-sets

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_beautiful_sets()
{
    TFactorial f;
    unsigned T, n, k;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        TModular s = 0;
        for (unsigned l = 1; l <= k / 2; ++l)
            s += f.BinomialCoefficient(n - k + 1, l) * f.BinomialCoefficient(k - l - 1, l - 1);
        cout << s << endl;
    }
	return 0;
}
