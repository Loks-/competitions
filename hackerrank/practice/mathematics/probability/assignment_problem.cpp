// https://www.hackerrank.com/challenges/assignment

#include "common/numeric/factorial_double.h"
#include "common/statistics/stat_m2.h"
#include "common/stl/base.h"

#include <iomanip>
#include <random>
#include <unordered_map>

int main_assignment_problem()
{
	cout << setprecision(3) << fixed;
    FactorialDouble f;
    unordered_map<unsigned, unsigned> mcnt;
    minstd_rand random_engine;
	unsigned T, M, N;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        cin >> M >> N;
        uniform_int_distribution<unsigned> uid(0, N - 1);
        StatM2<> s;
        for (unsigned j = 0; (j < 1000) || (s.Variance() > 1e-6 * j); ++j)
        {
            // cout << j << "\t" << s.Variance() << endl;
            mcnt.clear();
            for (unsigned k = 0; k < M; ++k)
                mcnt[uid(random_engine)] += 1;
            unsigned best = 1;
            double wm = 1.0;
            for (auto p : mcnt)
            {
                best = max(best, p.second);
                wm *= f.Get(p.second);
            }
            s.AddSample(best, wm);
        }
        cout << s.Mean() << endl;
	}
	return 0;
}
