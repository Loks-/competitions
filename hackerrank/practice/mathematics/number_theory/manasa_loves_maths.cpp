// https://www.hackerrank.com/challenges/manasa-loves-maths

#include "common/stl/base.h"

#include <string>

int main_manasa_loves_maths()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        bool good = false;
        string s;
        cin >> s;
        if (s.size() == 1)
            good = (s == "0") || (s == "8");
        else if (s.size() == 2)
        {
            int n1 = (s[0] - '0') * 10 + (s[1] - '0');
            int n2 = (s[0] - '0') + 10 * (s[1] - '0');
            good = ((n1 % 8) == 0) || ((n2 % 8) == 0);
        }
        else
        {
            for (unsigned i1 = 0; i1 < s.size(); ++i1)
            {
                for (unsigned i2 = 0; i2 < s.size(); ++i2)
                {
                    if (i1 == i2) continue;
                    for (unsigned i3 = 0; i3 < s.size(); ++i3)
                    {
                        if ((i1 == i3) || (i2 == i3))
                            continue;
                        int n = (s[i1] - '0') * 100 + (s[i2] - '0') * 10 + (s[i3] - '0');
                        good = good || ((n % 8) == 0);
                    }
                }
            }
        }
        cout << (good ? "YES" : "NO") << endl;
	}
	return 0;
}
