// https://www.hackerrank.com/challenges/superpowers

#include "common/factorization/utils/eulers_totient.h"
#include "common/factorization/utils/factorize.h"
#include "common/modular/modular_arithmetic.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/stl/base.h"

using TModularA = TModularArithmetic_C32U;

int main_superpowers_of_2()
{
    uint64_t a, b, c = 1, d, r, one = 1;
    cin >> a >> b;
    if (b <= 2)
        r = 0;
    else if (a < 61)
        r = TModularA::PowUSafe(2, (one << a), b);
    else
    {
        for (; (b & 1) == 0; b /= 2) c *= 2;        
        d = EulersTotient(b, Factorize(b));
        uint64_t aa = TModularA::PowUSafe(2, a, d);
        r = MergeRemainders<TModularA>(c, 0, b, TModularA::PowUSafe(2, aa, b));
    }
    cout << r << endl;
	return 0;
}
