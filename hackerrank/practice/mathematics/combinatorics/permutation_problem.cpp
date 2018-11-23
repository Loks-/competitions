// https://www.hackerrank.com/challenges/permutation-problem

#include "common/modular/static/factorial.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TFactorial = ModularFactorial<TModular>;

int main_permutation_problem()
{
    TFactorial f;
    unsigned T, n, k;
    cin >> T;
    TModular m = TModular(9) / TModular(10);
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n >> k;
        vector<TModular> vcurrent(n + 1), vnext(n + 1);
        vcurrent[0] = 1;
        for (unsigned i = 0; i < 10; ++i)
        {
            fill(vnext.begin(), vnext.end(), 0);
            for (unsigned j = 0; j <= n; ++j)
            {
                for (unsigned l = 0; l <= min(j, k); ++l)
                    vnext[j] += vcurrent[j - l] * f.GetI(l);
            }
            vcurrent.swap(vnext);
        }
        cout << m * vcurrent.back() * f.Get(n) << endl;
    }
	return 0;
}
