// https://www.hackerrank.com/challenges/sherlock-and-gcd

#include "common/base.h"
#include "common/numeric/utils.h"

int main_sherlock_and_gcd()
{
	unsigned T, N;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N;
        uint64_t a, b;
        cin >> a;
        for (unsigned i = 1; i < N; ++i)
        {
            cin >> b;
            a = GCD(b, a);
        }
        cout << ((a == 1) ? "YES" : "NO") <<endl;
    }
	return 0;
}
