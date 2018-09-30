// https://www.hackerrank.com/challenges/pairs-again

#include "common/numeric/primes_list.h"
#include "common/vector/unsigned_set.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_satisfactory_pairs()
{
    PrimesList pl(600);
    unsigned n;
    cin >> n;
    vector<vector<uint64_t>> vvd(n);
    for (unsigned i = 1; i < n; ++i)
    {
        vvd[i] = pl.GetDivisors(i);
        sort(vvd[i].begin(), vvd[i].end());
    }
    UnsignedSet us(n);
    uint64_t total = 0;
    for (unsigned i = 1; i < n; ++i)
    {
        for (unsigned ix = i; ix < n; ix += i)
        {
            for (unsigned j : vvd[n - ix])
                if (i < j)
                    us.Insert(j);
        }
        total += us.Size();
        us.Clear();
    }
    cout << total << endl; 
	return 0;
}
