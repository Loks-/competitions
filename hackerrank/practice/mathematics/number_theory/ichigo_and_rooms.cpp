// https://www.hackerrank.com/challenges/ichigo-and-rooms

#include "common/factorization/utils/factorize.h"
#include "common/modular/utils/multiplicative_order.h"
#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"

int main_ichigo_and_rooms()
{
   	unsigned T, N;
    int A;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        cin >> N >> A;
        N = 2 * N + 1;
        if (A < 0)
            A += N;
        unsigned NGCD = N / GCD(N, unsigned(A));
        cout << N - 1 - GetMultiplicativeOrder(NGCD, 2) << endl;
    }
	return 0;
}
