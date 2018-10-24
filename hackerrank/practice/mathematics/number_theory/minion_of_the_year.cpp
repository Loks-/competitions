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
        uint64_t P1 = P - 1;
        
        // Find Primitive
        uint64_t r = FindSmallestPrimitive(P, primes_list.Factorize(P1)), rp = 1;
        
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

            // cout << "\t" << A << "\t" << B << "\t" << C << "\t" << D << "\t" << P1 << endl;
            
            // Find log
            uint64_t c = ri[C], d = ri[D];
            // cout << "\t\t" << c << "\t" << d << endl;
            uint64_t gc = GCD(c, P1), gd = GCD(d, P1), gcd = LCM(gc, gd);
            c *= (gcd / gc); c %= P1; A *= (gcd / gc);
            d *= (gcd / gd); d %= P1; B *= (gcd / gd);
            uint64_t m = P1 / gcd;
            c /= gcd; d /= gcd;

            // cout << "\t\t" << gc << "\t" << gd << "\t" << gcd << endl;
            // cout << "\t" << A << "\t" << B << "\t" << c << "\t" << d << "\t" << m << endl;

            // Calc answer
            if (m == 1)
            {
                cout << A + B << endl;
                continue;
            }
            // Simple bet
            uint64_t c1 = TModularArithmetic_C32U::Div(c, d, m);
            uint64_t d1 = TModularArithmetic_C32U::Div(d, c, m);
            // cout << "\t\t" << c1 << "\t" << d1 << endl;
            uint64_t best = min(A + B * c1, A * d1 + B);
            if (d1 < c1)
            {
                swap(c1, d1);
                swap(A, B);
            }
            for (uint64_t i = 0; i < c1; ++i)
            {
                uint64_t x = (i * m) / c1 + 1;
                uint64_t y = (c1 * x) % m;
                uint64_t s = A * x + B * y;
                best = min(best, s);
            }
            cout << best << endl;
        }
	}
	return 0;
}
