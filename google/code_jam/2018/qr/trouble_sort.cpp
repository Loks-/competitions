#include "../../../../common/base.h"

int main_trouble_sort()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		unsigned l, x;
		cin >> l;
		vector<unsigned> v0, v1;
		for (unsigned i = 0; i < l; ++i)
		{
			cin >> x;
			if (i & 1)
				v1.push_back(x);
			else
				v0.push_back(x);
		}
		sort(v0.begin(), v0.end());
		sort(v1.begin(), v1.end());
		unsigned w = l;
		for (unsigned i = 0; i < l - 1; ++i)
		{
			if (i & 1)
			{
				if (v1[i / 2] > v0[i / 2 + 1])
				{
					w = i;
					break;
				}
			}
			else
			{
				if (v0[i / 2] > v1[i / 2])
				{
					w = i;
					break;
				}
			}
		}
		cout << "Case #" << it + 1 << ": " << ((w == l) ? "OK" : to_string(w)) << endl;
	}
	return 0;
}
