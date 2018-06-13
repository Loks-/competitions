#include "common/base.h"

int main_steed_2_cruise_control()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 1; it <= T; ++it)
	{
		unsigned D, N;
		cin >> D >> N;
		double t = 0;
		for (unsigned i = 0; i < N; ++i)
		{
			unsigned k, s;
			cin >> k >> s;
			t = max(t, double(D - k) / s);
		}
		cout << "Case #" << it << ": ";
		printf("%.6lf", D / t);
		cout << endl;
	}
	return 0;
}
