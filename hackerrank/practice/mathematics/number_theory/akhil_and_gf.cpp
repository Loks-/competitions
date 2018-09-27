// https://www.hackerrank.com/challenges/akhil-and-gf

#include "common/numeric/modular_arithmetic.h"
#include "common/numeric/utils/merge_remainders.h"
#include "common/stl/base.h"

using TModular = TModularArithmetic_C32U;

int main_akhil_and_gf()
{
	unsigned T;
	cin >> T;
	for (unsigned it = 0; it < T; ++it)
	{
        uint64_t N, M, M3 = 1, ten = 10;
        cin >> N >> M;
        for (; (M % 3) == 0; M /= 3)
            M3 *= 3;
        uint64_t r = TModular::DivSafe(TModular::Sub(TModular::PowUSafe(10, N, M), 1, M), 9, M);
        uint64_t r3 = TModular::Sub(TModular::PowUSafe(10, N, 9 * M3), 1, 9 * M3) / 9;
        cout << MergeRemainders<TModular>(M3, r3, M, r) << endl;
	}
	return 0;
}
