// https://www.hackerrank.com/challenges/sherlock-puzzle

#include "common/stl/base.h"

#include <string>

int main_sherlock_puzzle()
{
    uint64_t k;
    string s;
    cin >> k >> s;
    vector<int> vs(1, 0);
    int smin = 0, smax = 0, maxl = 0;
    for (unsigned i = 0; i < s.size(); ++i)
    {
        vs.push_back(vs.back() + ((s[i] == '1') ? 3 : -2));
        smin = min(smin, vs.back());
        smax = max(smax, vs.back());
    }
    if (vs.back() >= 0)
    {
        cout << k * s.size() << endl;
        return 0;
    }
    int max_excess = smax - (vs.back() - smin);
    int maxk = max_excess / -vs.back();
    maxk = min(maxk, int(k));
    for (int fk = max(0, maxk-1); fk <= maxk; ++fk)
    {
        // ...
    }
    cout << maxl << endl;
	return 0;
}
