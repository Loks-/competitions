// https://www.hackerrank.com/challenges/pairs-again

#include "common/numeric/primes_list.h"
#include "common/stl/pair.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_satisfactory_pairs()
{
    PrimesList pl(600);
    unordered_set<pair<unsigned, unsigned>> s;
    unsigned n;
    cin >> n;
    for (unsigned i = 1; i < n; ++i)
    {
        auto vp1 = pl.GetDivisors(i);
        auto vp2 = pl.GetDivisors(n - i);
        for (uint64_t p1 : vp1)
            for (uint64_t p2 : vp2)
                if (p1 < p2)
                    s.insert(make_pair(unsigned(p1), unsigned(p2)));
    }
    cout << s.size() << endl; 
	return 0;
}
