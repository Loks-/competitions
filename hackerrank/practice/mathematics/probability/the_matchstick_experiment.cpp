// https://www.hackerrank.com/challenges/matchstick-experiment

#include "common/stl/base.h"

#include <cmath>
#include <iomanip>

int main_the_matchstick_experiment()
{
	double p;
	auto P = [&](unsigned remove, unsigned keep) { return pow(p, remove) * pow(1.0 - p, keep); };
	auto Solve11 = [&]() { return 1.0; };

	auto Solve1X = [&](uint64_t n)
	{
		double sum = 0;
		// 1 - angle
		sum += 2 * P(0, 1);
		// 1 - mid
		sum += (n - 2) * P(0, 2);
		if (n == 2)
		{
			sum += P(1, 0);
		}
		else
		{
			// 2 - angle
			sum += 2 * P(1, 1);
			// 2 - mid
			sum += (n - 3) * P(1, 2);
			if (n == 3)
			{
				sum += P(2, 0);
			}
			else
			{
				// 3 - angle
				sum += 2 * P(2, 1);
				// 3 - mid
				sum += (n - 4) * P(2, 2);
			}
		}
		return sum;
	};

	auto SolveXX2 = [&](uint64_t n, uint64_t m)
	{
		double sum = 0;
		if (m == 2)
		{
			// angle
			sum += 2 * P(1, 2);
			// mid
			sum += (n - 2) * P(1, 4);
		}
		else
		{
			// angle
			sum += 4 * P(1, 3);
			// border m
			sum += 2 * (m - 3) * P(1, 4);
			// border n
			sum += 2 * (n - 2) * P(1, 5);
			// mid
			sum += (n - 2) * (m - 3) * P(1, 6);
		}
		return sum;
	};

	auto SolveXX3 = [&](uint64_t n, uint64_t m)
	{
		if (m == 2)
			return 0.;
		double sum = 0;
		if (m == 3)
		{
			// angle
			sum += 2 * P(2, 3);
			// mid
			sum += (n - 2) * P(2, 6);
		}
		else
		{
			// angle
			sum += 4 * P(2, 4);
			// border m
			sum += 2 * (m - 4) * P(2, 5);
			// border n
			sum += 2 * (n - 2) * P(2, 7);
			// mid
			sum += (n - 2) * (m - 4) * P(2, 8);
		}
		return sum;
	};

	auto SolveXXA = [&](uint64_t n, uint64_t m)
	{
		double sum = 0;
		if (n > m) swap(n, m);
		if (n == 2)
		{
			if (m == 2)
			{
				// ff
				sum += P(2, 2);
			}
			else
			{
				// ff
				sum += P(2, 3);
				// fm
				sum += (m - 3) * P(2, 5);
				// fb
				sum += P(2, 4);
			}
		}
		else
		{
			// ff
			sum += P(2, 4);
			// fm + mf
			sum += ((m - 3) + (n - 3)) * P(2, 6);
			// fb + bf
			sum += 2 * P(2, 5);
			// mm
			sum += (m - 3) * (n - 3) * P(2, 8);
			// mb + bm
			sum += ((m - 3) + (n - 3)) * P(2, 7);
			// bb
			sum += P(2, 6);
		}
		return sum;
	};

	auto SolveXX = [&](uint64_t n, uint64_t m)
	{
		double sum = 0.;
		// 1 - angle
		sum += 4 * P(0, 2);
		// 1 - border
		sum += 2 * ((n - 2) + (m - 2)) * P(0, 3);
		// 1 - mid
		sum += (n - 2) * (m - 2) * P(0, 4);
		// 2 - horizontal
		sum += SolveXX2(n, m);
		// 2 - vertical
		sum += SolveXX2(m, n);
		// 3 - horizontal
		sum += SolveXX3(n, m);
		// 3 - vertical
		sum += SolveXX3(m, n);
		// 3 - angle
		sum += 4 * SolveXXA(n, m);
		return sum;
	};

	cout << setprecision(10) << fixed;
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
		uint64_t n, m;
		cin >> n >> m >> p;
		double sum = (n == 1) ? ((m == 1) ? Solve11() : Solve1X(m)) : ((m == 1) ? Solve1X(n) : SolveXX(n, m));
		cout << sum / (n * m) << endl;
	}
	return 0;
}
