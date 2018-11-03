// https://www.hackerrank.com/challenges/hackerrank-number

#include "common/numeric/long_unsigned.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_hackerrank_number()
{
    uint64_t a, b;
    cin >> a >> b;
    // L
    unordered_set<uint64_t> usL;
    for (uint64_t ia = 1; ia <= a; ++ia)
        for (uint64_t ib = 1; ib <= b; ++ib)
            usL.insert(ia * ib);
    uint64_t cL = usL.size(), sL = 0;
    for (uint64_t u : usL)
        sL += u;
    // cout << cL << "\t" << sL << endl;
    // R
    uint64_t maxaxb = 0;
    for (unsigned i = 30; i--; )
    {
        uint64_t bit = (1ull << i);
        if ((a | b) & bit)
        {
            maxaxb |= bit;
            if (a & b & bit)
            {
                maxaxb += (bit - 1);
                break;
            }
        }
    }
    vector<bool> vR(maxaxb + 1, false);
    for (uint64_t ia = 1; ia <= a; ++ia)
        for (uint64_t ib = 1; ib <= b; ++ib)
            vR[ia^ib] = true;
    uint64_t cR = 0, sR = 0;
    for (uint64_t i = 0; i <= maxaxb; ++i)
    {
        if (vR[i])
        {
            cR += 1;
            sR += i;
        }
    }
    // cout << cR << "\t" << sR << endl;
    // Final
    uint64_t lb = 1, pb = 10;
    for (; b >= pb; pb *= 10) ++lb;
    uint64_t r = sL * cR * pb * 10 + cL * sR;
    cout << r << endl;
	return 0;
}
