// https://www.hackerrank.com/challenges/chandrima-and-xor

#include "common/vector/read.h"
#include "common/stl/base.h"

int main_chandrima_and_xor()
{
    uint64_t M = 1000000007u;
	unsigned n;
	cin >> n;
    vector<uint64_t> va = ReadVector<uint64_t>(n);
    uint64_t m = *max_element(va.begin(), va.end()), r = 0;
    vector<uint64_t> vfi {1, 2}, v2p {1, 2};
    for (unsigned i = 0; vfi.back() < m; ++i)
    {
        vfi.push_back(vfi[i] + vfi[i + 1]);
        v2p.push_back((2 * v2p[i + 1]) % M);
    }
    for (unsigned i = vfi.size(); i--; )
    {
        unsigned c = 0;
        for (unsigned j = 0; j < n; ++j)
        {
            if (va[j] >= vfi[i])
            {
                ++c;
                va[j] -= vfi[i];
            }
        }
        if (c & 1)
            r += v2p[i];
    }
    cout << r % M << endl;
	return 0;
}
