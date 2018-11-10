// https://www.hackerrank.com/challenges/dice-stats

#include "common/stl/base.h"

#include <iomanip>

int main_dice_stats()
{
    struct S
    {
        double p, pS, pSS;
        S() : p(0), pS(0), pSS(0) {}
    };

	vector<double> vp(6), vi(6);
	double exp_approx = 0.;
	for (unsigned i = 0; i < 6; ++i)
	{
		vi[i] = 1.0 + i;
		cin >> vp[i];
		exp_approx += vi[i] * vp[i];
	}
	for (double& di : vi)
		di -= exp_approx;
	unsigned N;
	cin >> N;
	vector<S> vcurrent(6), vnew(6);
	for (unsigned iN = 0; iN < N; ++iN)
	{
		for (unsigned i = 0; i < 6; ++i)
		{
			vnew[i].p = vnew[i].pS = vnew[i].pSS = 0.;
			if (iN == 0)
			{
				vnew[i].p = vp[i];
				vnew[i].pS = vp[i] * vi[i];
				vnew[i].pSS = vp[i] * vi[i] * vi[i];
			}
			else
			{
				for (unsigned j = 0; j < 6; ++j)
				{
					if (j == i) continue;
					double p = vp[i] / (1.0 - vp[j]);
					vnew[i].p += p * vcurrent[j].p;
					vnew[i].pS += p * (vcurrent[j].pS + vcurrent[j].p * vi[i]);
					vnew[i].pSS += p * (vcurrent[j].pSS + 2 * vcurrent[j].pS * vi[i] + vcurrent[j].p * vi[i] * vi[i]);
				}
			}
		}
		vcurrent.swap(vnew);
	}
	S f;
	for (unsigned i = 0; i < 6; ++i)
	{
		f.p += vcurrent[i].p;
		f.pS += vcurrent[i].pS;
		f.pSS += vcurrent[i].pSS;
	}
	f.pS /= f.p;
	f.pSS /= f.p;
    cout << setprecision(6) << fixed << f.pS + exp_approx * N << endl << f.pSS - f.pS * f.pS << endl;
	return 0;
}
