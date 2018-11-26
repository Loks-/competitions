// https://www.hackerrank.com/challenges/assignment

#include "common/numeric/log_double.h"
#include "common/statistics/stat_m1.h"
#include "common/stl/base.h"

#include <iomanip>
#include <unordered_map>
#include <utility>

int main_assignment_problem()
{
    unsigned maxm = 65, maxm2 = maxm * maxm; //251;
    vector<LogDouble> v(maxm * maxm * maxm); // layers max_length m
    for (unsigned l = 1; l < maxm; ++l)
        v[1 * maxm2 + l * maxm + l] = 1.0;
    for (unsigned m = 1; m + 1 < maxm; ++m)
    {
        for (unsigned l = 1; l <= m; ++l)
        {
            for (unsigned ml = 1; ml <= m; ++ml)
            {
                LogDouble c = v[l * maxm2 + ml * maxm + m];
                if (c.GetLog() < -0.5) continue; // c == 0
                for (unsigned k = 1; m + k < maxm; ++k)
                    v[(l + 1) * maxm2 + max(k, ml) * maxm + (m + k)] += c;
            }
        }
    }

	cout << setprecision(3) << fixed;
	unsigned T, M, N;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        StatM1<LogDouble> s;
        cin >> M >> N;
        LogDouble b = 1.0;
        for (unsigned l = 1; l <= min(M, N); ++l)
        {
            b *= double(N - l + 1) / double(l);
            for (unsigned ml = 1; ml <= M; ++ml)
            {
                LogDouble w = v[l* maxm2 + ml *maxm + M];
                if (w.GetLog() < -0.5) continue;
                s.AddSample(double(ml), w * b);
            }
        }
        cout << s.Mean().Get() << endl;
	}
	return 0;
}
