// https://www.hackerrank.com/challenges/minimal-distance-to-pi

#include "common/numeric/continued_fraction.h"
#include "common/numeric/fraction.h"
#include "common/stl/base.h"

int main_minimal_distance_to_pi()
{
    int64_t l, r, bn = 0, bd = 1;
    cin >> l >> r;
    l -= 1;
    // Based on https://oeis.org/A001203 .
    vector<int64_t> pi_seq { 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2, 1, 1, 15, 3, 13, 1, 4, 2, 6, 6, 99, 1, 2, 2, 6, 3, 5, 1, 1, 6, 8, 1, 7, 1, 2, 3, 7, 1, 2, 1, 1, 12, 1, 1, 1, 3, 1, 1, 8, 1, 1, 2, 1, 6, 1, 1, 5, 2, 2, 3, 1, 2, 4, 4, 16, 1, 161, 45, 1, 22, 1, 2, 2, 1, 4, 1, 2, 24, 1, 2, 1, 3, 1, 2, 1 };
    ContinuedFraction cf(pi_seq);
    for (TIFraction f = cf.GetNextFraction(); f.GetD() <= r; f = cf.GetNextFraction())
    {
        int64_t x1 = l / f.GetD(), x2 = r / f.GetD();
        if (x2 > x1)
        {
            bn = (x1 + 1) * f.GetN();
            bd = (x1 + 1) * f.GetD();
        }
    }
    cout << bn << "/" << bd << endl;
	return 0;
}
