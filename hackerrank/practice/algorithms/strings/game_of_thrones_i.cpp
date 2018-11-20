// https://www.hackerrank.com/challenges/game-of-thrones

#include "common/stl/base.h"

#include <string>

int main_game_of_thrones_i()
{
    string s;
    cin >> s;
    sort(s.begin(), s.end());
    unsigned n = unsigned(s.size());
    s.push_back(' ');
    unsigned ls = 0, so = 0;
    for (unsigned i = 1; i <= n; ++i)
    {
        if (s[i] == s[i-1]) continue;
        so += (i - ls) & 1;
        ls = i;
    }
    cout << ((so > 1) ? "NO" : "YES") << endl;
	return 0;
}
