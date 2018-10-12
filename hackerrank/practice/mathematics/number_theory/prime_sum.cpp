// https://www.hackerrank.com/challenges/prime-sum

#include "common/factorization/primality_test.h"
#include "common/stl/base.h"

int main_prime_sum()
{
    MillerRabinPrimalityTest pt;
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, K;
        cin >> N >> K;
        bool ok;
        if (K == 1)
            ok = pt.IsPrime(N);
        else if ((N & 1) && (K == 2))
            ok = (N > 2) && pt.IsPrime(N - 2);
        else
            ok = (N >= 2 * K);
        cout << (ok ? "Yes" : "No") << endl;
	}
	return 0;
}
