// https://www.hackerrank.com/challenges/minion-of-the-year

#include "common/factorization/primes_list.h"
#include "common/modular/modular_arithmetic.h"
#include "common/modular/discrete_log_sqrt_map.h"
#include "common/numeric/utils/gcd.h"
#include "common/numeric/utils/lcm.h"
#include "common/stl/base.h"

using TDiscreteLog = DiscreteLogSqrtMap<>;

int main_minion_of_the_year()
{
    PrimesList primes_list(10000);
	unsigned T, N, P;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        cin >> P >> N;
        uint64_t P1 = P - 1;
        TDiscreteLog dl(P, primes_list.Factorize(P1), 500000);
        for (unsigned iN = 0; iN < N; ++iN)
        {
            uint64_t A, B, C, D;
            cin >> A >> B >> C >> D;
            C %= P; D %= P;
            if (C * D == 0)
            {
                if (C + D > 0)
                    cout << "wala" << endl;
                else
                    cout << A + B << endl;
                continue;
            }
            uint64_t c = dl.Log(C), d = dl.Log(D);
            uint64_t gc = GCD(c, P1), gd = GCD(d, P1), gcd = LCM(gc, gd);
            c *= (gcd / gc); c %= P1; A *= (gcd / gc);
            d *= (gcd / gd); d %= P1; B *= (gcd / gd);
            uint64_t m = P1 / gcd;
            c /= gcd; d /= gcd;
            if (m == 1)
            {
                cout << A + B << endl;
                continue;
            }
            uint64_t c1 = TModularArithmetic_C32U::Div(c, d, m);
            uint64_t d1 = TModularArithmetic_C32U::Div(d, c, m);
            if (A < B)
            {
                swap(A, B);
                swap(c1, d1);
            }
            uint64_t best = A + B * c1;
            for (uint64_t i = 2; i * A + B < best; ++i)
                best = min(best, A * i + B * ((c1 * i) % m));
            cout << best << endl;
        }
	}
	return 0;
}
