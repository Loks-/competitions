// https://www.hackerrank.com/challenges/closest-number

#include "common/numeric/utils/pow.h"
#include "common/stl_base.h"

int main_closets_number()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        int64_t a, b, x, r;
        cin >> a >> b >> x;
        if ((a == 1) || (b == 0))
            r = (x > 1) ? 0 : 1;
        else if (b < 0)
            r = 0;
        else
        {
            int64_t apb = PowU(a, uint64_t(b));
            int64_t xl = apb - (apb % x), xr = xl + x;
            r = (xl + xr >= 2 * apb) ? xl : xr;
        }
        cout << r << endl;
	}
	return 0;
}
