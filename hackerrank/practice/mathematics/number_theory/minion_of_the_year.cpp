// https://www.hackerrank.com/challenges/minion-of-the-year

#include "common/factorization/primes_list.h"
#include "common/modular/modular_arithmetic.h"
#include "common/modular/utils/primitive_root.h"
#include "common/numeric/utils/gcd.h"
#include "common/numeric/utils/lcm.h"
#include "common/stl/base.h"

int main_minion_of_the_year()
{
    PrimesList primes_list(10000);
	unsigned T, N, P;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        cin >> P >> N;
        unsigned P1 = P - 1;
        
        // Find Primitive
        unsigned r = unsigned(FindSmallestPrimitive(P, primes_list.Factorize(P1))), rp = 1;
        
        // Prebuild log map
        vector<unsigned> ri(P, 0);
        for (unsigned i = 0; i + 1 < P; ++i)
        {
            ri[rp] = i;
            rp = (rp * r) % P;
        }
        
        for (unsigned iN = 0; iN < N; ++iN)
        {
            uint64_t A, B, C, D;
            cin >> A >> B >> C >> D;
            C %= P; D %= P;
            if ((C * D == 0) && (C + D > 0))
            {
                cout << "wala" << endl;
                continue;
            }
            
            // Find log
            unsigned c = ri[C], d = ri[D];
            unsigned gc = GCD<unsigned>(c, P1), gd = GCD<unsigned>(d, P1), gcd = LCM<unsigned>(gc, gd);
            c /= gc; A *= (gcd / gc);
            d /= gd; B *= (gcd / gd);
            unsigned m = P1 / gcd;

            // Calc answer
            if (m == 1)
            {
                cout << A + B << endl;
                continue;
            }
            // Simple bet
            unsigned c1 = TModularArithmetic_C32U::Div(1, d, m);
            unsigned d1 = TModularArithmetic_C32U::Div(1, c, m);
            uint64_t best = min(A * c1 + B, A + B * d1);
            if (d < c)
            {
                swap(c, d);
                swap(A, B);
            }
            // for (unsigned i = 0; i < c; ++i)
            // {
            //     unsigned x = (i * m) / c + 1;
            //     unsigned y = (c * x) % m;
            //     uint64_t s = A * x + B * y;
            //     best = min(best, s);
            // }
            cout << best << endl;
        }
	}
	return 0;
}
