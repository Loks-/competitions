// https://www.hackerrank.com/challenges/megaprime-numbers

#include "common/binary_search.h"
#include "common/numeric/primality_test.h"
#include "common/stl/base.h"

int main_megaprime_numbers()
{
    const vector<uint64_t> valid_digits {2, 3, 5, 7};
    auto to_uint = [valid_digits](unsigned u)
    {
        uint64_t t = 0, t10 = 1;
        for (; u; u /= 4)
        {
            if (u < 5)
                --u;
            t += t10 * valid_digits[u % 4];
            t10 *= 10;
        }
        return t;
    };
    uint64_t f, l;
    cin >> f >> l;
    unsigned fb = LowerBoundF(0u, 2000000000u, f, to_uint);
    unsigned lb = UpperBoundF(0u, 2000000000u, l, to_uint);
    unsigned total = 0;
    MillerRabinPrimalityTest pt;
    for (; fb < lb; ++fb)
    {
        if (pt.IsPrime(to_uint(fb)))
            ++total;
    }
    cout << total << endl;
	return 0;
}
