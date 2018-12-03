// https://www.hackerrank.com/challenges/manasa-and-combinatorics

#include "common/modular/static/binomial_coefficient_prime.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<99991>;

int main_manasa_and_combinatorics()
{
    unsigned T;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t n;
        cin >> n;
        TModular s = 0;
        for (unsigned kl = 0; kl < n; ++kl)
        {
            unsigned kr = n - kl;
            TModular sl = ModularPrimeBinomialCoefficient<TModular>(n + kl - 1, n - 1) - ModularPrimeBinomialCoefficient<TModular>(n + kl - 1, n);
            TModular sr = ModularPrimeBinomialCoefficient<TModular>(n + kr, n) - ModularPrimeBinomialCoefficient<TModular>(n + kr, n + 1);
            s += sl * sr;
        }
        cout << s << endl;
    }
	return 0;
}
