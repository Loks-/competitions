// https://www.hackerrank.com/challenges/picking-cards

#include "common/modular/static/modular_io.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModular = Modular<>;

int main_picking_cards()
{
    unsigned T, n;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        cin >> n;
        vector<unsigned> v = ReadVector<unsigned>(n);
        sort(v.begin(), v.end());
        v.push_back(n);
        TModular r = 1;
        for (unsigned i = 0, j = 0; i < n; ++i)
        {
            for (; v[j] <= i;) ++j;
            if (j <= i)
                r = 0;
            else
                r *= j - i;
        }
        cout << r << endl;
    }
	return 0;
}
