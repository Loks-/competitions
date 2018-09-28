// https://www.hackerrank.com/challenges/dance-class

#include "common/numeric/utils/usqrt.h"
#include "common/stl/base.h"

int main_dancing_in_pairs()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t n;
        cin >> n;
        uint64_t sn = USqrt(n);
        cout << ((sn & 1) ? "odd" : "even") << endl;
	}
	return 0;
}
