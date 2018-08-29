#include "common/stl_base.h"

int main_bit_party()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		uint64_t R, B, C, ss = 0, sp = 0;
		cin >> R >> B >> C;
		vector<uint64_t> vM(C), vS(C), vP(C), vC(C);
		for (unsigned i = 0; i < C; ++i)
		{
			cin >> vM[i] >> vS[i] >> vP[i];
			ss = std::max(ss, vS[i]);
			sp = std::max(sp, vP[i]);
		}
		uint64_t t1 = 0, t2 = ss * B + sp;
		while (t2 - t1 > 1)
		{
			uint64_t tm = (t1 + t2) / 2;
			for (unsigned i = 0; i < C; ++i)
			{
				vC[i] = (vP[i] >= tm) ? 0 : ((tm - vP[i]) / vS[i]);
				vC[i] = min(vC[i], vM[i]);
			}
			if (R < C)
			{
				sort(vC.begin(), vC.end());
				reverse(vC.begin(), vC.end());
			}
			uint64_t total = 0;
			for (unsigned i = 0; i < R; ++i)
				total += vC[i];
			if (total < B)
				t1 = tm;
			else
				t2 = tm;
		}
		cout << "Case #" << it << ": " << t2 << endl;
	}
	return 0;
}
