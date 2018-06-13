#include "common/base.h"

namespace {
	static double pi4 = atan(1.0);
	static double pi2 = 2.0 * pi4;
	static double sq2 = sqrt(2.0);
	static double sq3 = sqrt(3.0);
	static double beta0 = atan(sq2);

	static double Evaluate(double a)
	{
		double c1v = 0.5 * sin(a);
		double v1v = sq3 * 0.5 * sin(a + beta0);
		return (c1v + v1v) * sq2;
	}

	static void Print(double a, unsigned test_index)
	{
		cout << "Case #" << test_index << ":" << endl;
		double v1 = 0.5 * sin(a);
		double y1 = 0.5 * cos(a);
		printf("%.16lf %.16lf %.16lf\n", v1 / sq2, y1, v1 / sq2);
		double u = sq2 / 4.0;
		double y2 = u * sin(a);
		double v2 = -u * cos(a);
		printf("%.16lf %.16lf %.16lf\n", (v2 + u) / sq2, y2, (v2 - u) / sq2);
		printf("%.16lf %.16lf %.16lf\n", (v2 - u) / sq2, y2, (v2 + u) / sq2);
	}
}

int main_cubic_ufo()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
		double A;
		cin >> A;
		double a0 = 0., s0 = Evaluate(a0), a1 = beta0, s1 = Evaluate(a1);
		while (s1 - s0 > 5e-7)
		{
			double am = (a0 + a1) / 2.0, sm = Evaluate(am);
			if (sm < A)
			{
				a0 = am; s0 = sm;
			}
			else
			{
				a1 = am; s1 = sm;
			}
		}
		Print((a0 + a1) / 2.0, it + 1);
	}
	return 0;
}
