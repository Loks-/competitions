#include "common/base.h"

int main_waffle_choppers()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned R, C, H, V, total = 0;
		bool b = true;
		cin >> R >> C >> H >> V;
		vector<vector<unsigned>> v(R, vector<unsigned>(C, 0));
		vector<unsigned> vR(R, 0), vC(C, 0);
		for (unsigned i = 0; i < R; ++i)
		{
			std::string s;
			cin >> s;
			for (unsigned j = 0; j < C; ++j)
			{
				v[i][j] = (s[j] == '@') ? 1 : 0;
				total += v[i][j];
				vR[i] += v[i][j];
				vC[j] += v[i][j];
			}
		}
		if (total != 0)
		{
			b = b && ((total % ((H + 1) * (V + 1))) == 0);
			vector<unsigned> vRs(1, 0), vCs(1, 0);
			if (b)
			{
				unsigned t1 = total / (H + 1);
				unsigned req = t1, sf = 0;
				for (unsigned i = 0; i < R; ++i)
				{
					sf += vR[i];
					if (sf == req)
					{
						vRs.push_back(i + 1);
						req += t1;
					}
					else if (sf > req)
					{
						b = false;
						break;
					}
				}
			}
			if (b)
			{
				unsigned t1 = total / (V + 1);
				unsigned req = t1, sf = 0;
				for (unsigned i = 0; i < C; ++i)
				{
					sf += vC[i];
					if (sf == req)
					{
						vCs.push_back(i + 1);
						req += t1;
					}
					else if (sf > req)
					{
						b = false;
						break;
					}
				}
			}
			b == b && (vRs.size() == H + 2) && (vCs.size() == V + 2);
			if (b)
			{
				unsigned tb = total / ((H + 1) * (V + 1));
				for (unsigned i = 0; i <= H; ++i)
				{
					for (unsigned j = 0; j <= V; ++j)
					{
						unsigned sf = 0;
						for (unsigned i1 = vRs[i]; i1 < vRs[i + 1]; ++i1)
						{
							for (unsigned j1 = vCs[j]; j1 < vCs[j + 1]; ++j1)
							{
								sf += v[i1][j1];
							}
						}
						if (sf != tb)
						{
							b = false;
							break;
						}
					}
				}
			}
		}
		cout << "Case #" << it << ": ";
		cout << (b ? "POSSIBLE" : "IMPOSSIBLE");
		cout << endl;
	}
	return 0;
}
