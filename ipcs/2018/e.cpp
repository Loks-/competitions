#include "common/stl/full.h"

int main_e()
{
    mt19937 re;
    struct SMap
    {
        uint64_t a, b, p, m, k0;

        uint64_t Apply(uint64_t k) const { return (((k * a + b) % p) % m); }
        bool Test(uint64_t k) const { return Apply(k) == k0; }
    };

    unsigned T, maxp = 100000;
    cin >> T;
    for (unsigned iT = 0; iT < T; ++iT)
    {
        unsigned p, k, n;
        cin >> p >> k >> n;
        vector<SMap> vm(n);
        for (unsigned i = 0; i < n; ++i)
        {
            vm[i].p = p;
            cin >> vm[i].a >> vm[i].b >> vm[i].m;
            vm[i].k0 = vm[i].Apply(k);
        }
        if (p < maxp)
        {
            unsigned t = 0;
            for (unsigned j = 0; j < p; ++j)
            {
                for (unsigned i = 0; i < n; ++i)
                {
                    if (vm[i].Test(j))
                    {
                        t += 1;
                        break;
                    }
                }
            }
            cout << t << endl;
        }
        else
        {
            uniform_int_distribution<unsigned> uid(0, p-1);
            unsigned t = 0;
            for (unsigned j = 0; j < maxp; ++j)
            {
                for (unsigned i = 0; i < n; ++i)
                {
                    if (vm[i].Test(uid(re)))
                    {
                        t += 1;
                        break;
                    }
                }
            }
            cout << double(t) / double(maxp) * p << endl;
        }
    }
    return 0;
}
