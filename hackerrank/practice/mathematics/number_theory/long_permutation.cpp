// https://www.hackerrank.com/challenges/long-permutation

#include "common/vector/read.h"
#include "common/stl/base.h"

int main_long_permutation()
{
    unsigned n;
    uint64_t m;
    cin >> n >> m;
    vector<unsigned> p = ReadVector<unsigned>(n);
    uint64_t pos = m, move = m;
    for (; move && (pos < move + n); --move)
    {
        uint64_t rel_pos = pos - move;
        pos = move + p[rel_pos] - 1;
    }
    cout << pos << endl;
	return 0;
}
