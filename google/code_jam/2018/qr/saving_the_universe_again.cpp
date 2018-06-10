#include "../../../../common/base.h"

int main_saving_the_universe_again()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		uint64_t d, dc = 0, cc = 1, one = 1;
		string s;
		cin >> d >> s;
		unsigned nc = 0, ns = 0;
		for (char c : s)
		{
			if (c == 'C')
			{
				++nc;
				cc *= 2;
			}
			else if (c == 'S')
			{
				++ns;
				dc += cc;
			}
		}
		if (dc <= d)
			cout << "Case #" << it + 1 << ": " << 0 << endl;
		else if (d < ns)
			cout << "Case #" << it + 1 << ": " << "IMPOSSIBLE" << endl;
		else
		{
			unsigned k = 0;
			for (; d < dc; ++k)
			{
				unsigned csf = 0;
				unsigned lasti = 0, lastc = 0;
				for (unsigned i = 0; i < s.size() - 1; ++i)
				{
					if ((s[i] == 'C') && (s[i + 1] == 'S'))
					{
						lasti = i;
						lastc = csf;
					}
					if (s[i] == 'C')
						csf += 1;
				}
				dc -= (one << lastc);
				swap(s[lasti], s[lasti + 1]);
			}
			cout << "Case #" << it + 1 << ": " << k << endl;
		}
	}
	return 0;
}
