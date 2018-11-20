// https://www.hackerrank.com/challenges/number-list

#include "common/vector/read.h"
#include "common/stl/base.h"

int main_number_list()
{
    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t n, k;
        cin >> n >> k;
        vector<uint64_t> va = ReadVector<uint64_t>(n);
        uint64_t lg = -1ull, s = 0;
        for (uint64_t i = 0; i < n; ++i)
        {
            if (va[i] <= k) continue;
            uint64_t l = i - lg;
            s += l * (l - 1);
            lg = i;
        }
        s += (n - lg) * (n - lg - 1);
        cout << (n * (n + 1) - s) / 2 << endl;
    }
	return 0;
}
