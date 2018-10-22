// https://www.hackerrank.com/challenges/order-of-prime-in-factorial

#include "common/stl/base.h"

int main_order_of_prime_in_factorial()
{
	unsigned T;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        uint64_t p, l, s;
        cin >> p >> l;
        s = (l / p / p) * p;
        uint64_t l1 = s * p, l2 = l - l1 + 1, l1p = 0;
        for (uint64_t t = l1 / p; t; t /= p)
            l1p += t;
        l1p %= p;
        uint64_t r = (p - l1p) % p;
        if (r == 0)
            s += min(p, l2);
        else
           s += min(max(r * p, l2) - r*p, p);
        cout << s - 1 << endl;
	}
	return 0;
}
