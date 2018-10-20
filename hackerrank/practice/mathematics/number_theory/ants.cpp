// https://www.hackerrank.com/challenges/ants

#include "common/vector/read.h"
#include "common/stl/base.h"

int main_ants()
{
    unsigned N, L = 1000, p = 0;
    cin >> N;
    vector<unsigned> vp = ReadVector<unsigned>(N);
    sort(vp.begin(), vp.end());
    for (unsigned i = 0; i < vp.size(); ++i)
    {
        if (vp[i] == i)
            vp.push_back(i + L);
        if ((i > 0) && (vp[i-1] + 1 == vp[i]))
        {
            ++p; ++i;
        }
    }
    cout << 4 * ((N + 1ull) / 2) * (N / 2) * 100000ull + 2 * p << endl;
	return 0;
}
