// https://www.hackerrank.com/challenges/assignment

#include "common/local.h"
#include "common/numeric/log_double_factorial.h"
#include "common/numeric/log_double_io.h"
#include "common/statistics/stat_m1.h"
#include "common/stl/base.h"

#include <iomanip>
#include <unordered_map>
#include <utility>

int main_assignment_problem()
{
    LogDoubleFactorial f;
    unsigned maxm = local_run ? 65 : 251, maxm2 = maxm * maxm;
    vector<double> v(maxm * maxm * maxm); // layers max_length m
    for (unsigned l = 1; l < maxm; ++l)
        v[1 * maxm2 + l * maxm + l] = 1.0;
    for (unsigned m = 1; m < maxm; ++m)
    {
        unsigned k_end = min(m + 1, maxm - m);
        for (unsigned l = 1; l <= m; ++l)
        {
            unsigned ml_end = m - l + 2;
            for (unsigned ml = (m - 1) / l + 1; ml < ml_end; ++ml)
            {
                if (l > 1 && 2 * ml > m)
                    v[l * maxm2 + ml * maxm + m] = f.BinomialCoefficient(m - ml - 1, l - 2).Get() * l;
                double c = v[l * maxm2 + ml * maxm + m];
                for (unsigned k = 1; k < k_end; ++k)
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
            for (unsigned ml = 1; ml + l - 1 <= M; ++ml)
                s.AddSample(double(ml), b * v[l* maxm2 + ml *maxm + M]);
        }
        cout << s.Mean() << endl;
	}
	return 0;
}
