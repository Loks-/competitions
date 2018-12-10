// https://www.hackerrank.com/challenges/cyclicquadruples

#include "common/modular/proxy/factorial.h"
#include "common/modular/proxy/modular_proxy.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

int main_value_of_all_permutations()
{
    unsigned p, n, q, l, m;
    cin >> p >> n;
    TModularProxy_P32U mp(p);
    PModularFactorial<TModularProxy_P32U> f(p);
    vector<unsigned> v = ReadVector<unsigned>(n);
    sort(v.begin(), v.end());
    uint64_t lfi = 1, s = 0, sj = 0;
    vector<uint64_t> va(n);
    for (unsigned i = 0; i < v.size(); i += l)
    {
        s = 0;
        for (l = 1; (i + l < v.size()) && (v[i + l] == v[i]);) ++l;
        lfi = mp.Mult(lfi, f.GetI(l));
        for (unsigned j = i; j < i + l; ++j)
            sj += v[j];
        sj %= p;
        for (unsigned j = 1; j <= i + l; ++j)
            s += mp.Mult(sj, mp.Mult(f.Get(i + l - 1), mp.Mult(f.GetI(i + l - j), f.Get(n - j))));
        for (unsigned j = i; j < i + l; ++j)
            va[j] = s % p;
    }
    cin >> q;
    for (unsigned iq = 0; iq < q; ++iq)
    {
        cin >> m;
        auto it = upper_bound(v.begin(), v.end(), m);
        cout << ((m < v[0]) ? 0ull : mp.Mult(va[it - v.begin() - 1], lfi)) << endl;
    }
	return 0;
}
