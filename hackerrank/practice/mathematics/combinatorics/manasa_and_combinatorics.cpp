// https://www.hackerrank.com/challenges/manasa-and-combinatorics

#include "common/modular/static/factorial.h"
#include "common/modular/static/factorial_extended_prime.h"
#include "common/modular/static/modular_io.h"
#include "common/numeric/utils/factorial_prime_power.h"
#include "common/stl/base.h"

using TModular = TModular_P32<99991>; // Modular<>; // TModular_P32<99991>;
using TFactorial = ModularFactorial<TModular>;

int main_manasa_and_combinatorics()
{
    TFactorial f;
    const uint64_t p = TModular::GetMod();

    auto F = [&](uint64_t n1, uint64_t d1, uint64_t d2)
    {
        return (FactorialPrimePower(n1, p) == FactorialPrimePower(d1, p) + FactorialPrimePower(d2, p)) ?
            ModularFactorialExtendedPrime(n1, f, false) * ModularFactorialExtendedPrime(d1, f, true) * ModularFactorialExtendedPrime(d2, f, true) :
            TModular(0);
    };

    auto G = [&](uint64_t c, uint64_t n, uint64_t k)
    {
        return F(2*c, c, c+1) * F(n, k, n - k);
    };

    unsigned T;
    cin >> T;
    // T = 10;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        uint64_t n;
        cin >> n;
        // n = iT + 1;
        // cout << f.Catalan(n) * f.Catalan(n) << endl;
        TModular s = 0;
        for (unsigned kl = 0; kl < n; ++kl)
        {
            unsigned kr = n - kl;
            TModular sl = F(n + kl - 1, kl, n - 1) - (kl ? F(n + kl - 1, kl - 1, n) : TModular(0));
            // // TModular sl = f.BinomialCoefficient(n + kl - 1, kl);
            // for (unsigned i = 1; i <= kl; ++i)
            //     sl -= G(i - 1, n + kl - 2 * i, kl - i);
            //     // sl -= f.Catalan(i - 1) * f.BinomialCoefficient(n + kl - 2 * i, kl - i);
            TModular sr = F(n + kr, kr, n) - F(n + kr, kr - 1, n + 1);
            // TModular sr = f.BinomialCoefficient(n + kr, kr);
            // for (unsigned i = 1; i <= kr; ++i)
            //     sr -= G(i - 1, n + kr - 2 * i + 1, kr - i);
                // sr -= f.Catalan(i - 1) * f.BinomialCoefficient(n + kr - 2 * i + 1, kr - i);
            // if (sl.Get() && sr.Get())
            //     cout << kl << "\t" << sl << "\t" << sr << endl;
            // cout << kl << "\t" << sl << "\t" << sr << endl;
            s += sl * sr;
        }
        cout << s << endl;
    }
	return 0;
}
