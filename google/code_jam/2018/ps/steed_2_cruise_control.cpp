#include "common/stl_base.h"

#include <iomanip>

int main_steed_2_cruise_control()
{
	unsigned T;
	cin >> T;
	cout << setprecision(6) << fixed;
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
		cout << "Case #" << it << ": " << D/t << endl;
	}
	return 0;
}
