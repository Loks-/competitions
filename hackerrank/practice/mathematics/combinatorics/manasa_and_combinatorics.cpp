// https://www.hackerrank.com/challenges/manasa-and-combinatorics

#include "common/modular/static/factorial_extended_prime.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/factorial_prime_power.h"
#include "common/stl/base.h"

using TModular = TModular_P32<99991>;

int main_manasa_and_combinatorics()
{
    const uint64_t p = TModular::GetMod();
    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t n;
        cin >> n;
        unsigned pp = FactorialPrimePower(3 * n, p) - FactorialPrimePower(2 * n + 2, p) - FactorialPrimePower(n, p);
        cout << (pp ? TModular(0) :
            ModularFactorialExtendedPrime<TModular>(3 * n, false) * ModularFactorialExtendedPrime<TModular>(2 * n + 2, true) * ModularFactorialExtendedPrime<TModular>(n, true) * (TModular(n) * n + n + 2))
            << endl;
    }
	return 0;
}
