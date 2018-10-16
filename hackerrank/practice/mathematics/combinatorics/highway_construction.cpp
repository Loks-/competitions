// https://www.hackerrank.com/challenges/highway-construction

#include "common/modular/modular.h"
#include "common/polynomial/utils/sum_of_powers.h"
#include "common/stl/base.h"

using TModular = Modular<1000000009>;

int main_highway_construction()
{
    unsigned T, K;
    cin >> T;
    for (unsigned i = 0; i < T; ++i)
    {
        uint64_t N;
        cin >> N >> K;
        cout << ((N <= 2) ? uint64_t(0) : (GetSumOfPowers<TModular>(K)(N-1) - TModular(1)).Get()) << endl;
    }
    return 0;
}
