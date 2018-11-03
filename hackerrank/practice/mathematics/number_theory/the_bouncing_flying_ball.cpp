// https://www.hackerrank.com/challenges/the-bouncing-flying-ball

#include "common/modular/modular_arithmetic.h"
#include "common/modular/utils/merge_remainders.h"
#include "common/numeric/utils/gcd.h"
#include "common/vector/read.h"
#include "common/stl/base.h"

using TModularA = TModularArithmetic_C32U;

int main_the_bouncing_flying_ball()
{
    struct DInfo
    {
        uint64_t p;
        vector<uint64_t> vp1, vp2;
    };

    auto Add1 = [](uint64_t L, uint64_t dL, uint64_t g, uint64_t p, int xp, int xs, vector<uint64_t>& v)
    {
        uint64_t dx = TModularA::ApplyS(xp - xs, L);
        if (dx % g) return;
        uint64_t k = TModularA::Div(dx, dL, L);
        v.push_back(TModularA::ApplyU(k, p));
    };

    auto Add2 = [Add1](uint64_t L, uint64_t dL, uint64_t g, uint64_t p, int xp, int xs, vector<uint64_t>& v)
    {
        Add1(L, dL, g, p, xp, xs, v);
        Add1(L, dL, g, p, -xp, xs, v);
    };

    auto Merge = [](uint64_t p1, uint64_t p2, uint64_t g, uint64_t p, const vector<uint64_t>& v1, const vector<uint64_t>& v2, vector<uint64_t>& v)
    {
        for (uint64_t x1 : v1)
        {
            for (uint64_t x2 : v2)
            {
                if ((x1 % g) != (x2 % g)) continue;
                uint64_t x = MergeRemainders<TModularA>(p1, x1, p2, x2);
                v.push_back(TModularA::ApplyU(x, p));
            }
        }
    };

	unsigned T, D = 3;
	cin >> T;
	for (unsigned iT = 0; iT < T; ++iT)
	{
        vector<int> vD, vP1, vP2, vS;
        vD = ReadVector<int>(D);
        vP1 = ReadVector<int>(D);
        vP2 = ReadVector<int>(D);
        vS = ReadVector<int>(D);
        int N;
        cin >> N;
        vector<vector<DInfo>> vvinfo(D);
        for (unsigned d = 0; d < D; ++d)
        {
            for (int dx = -N; dx <= N; ++dx)
            {
                uint64_t L = 2 * uint64_t(vD[d]);
                uint64_t dL = TModularA::ApplyS(dx, L);
                uint64_t g = GCD(dL, L);
                DInfo info;
                info.p = L / g;
                Add2(L, dL, g, info.p, vP1[d], vS[d], info.vp1);
                Add2(L, dL, g, info.p, vP2[d], vS[d], info.vp2);
                if ((info.vp1.size() > 0) || (info.vp2.size() > 0))
                    vvinfo[d].push_back(info);
            }
        }

        vector<DInfo> vcurrent = vvinfo[0], vnext;
        for (unsigned d = 1; d < D; ++d)
        {
            vnext.clear();
            for (const DInfo& d1 : vcurrent)
            {
                for (const DInfo& d2: vvinfo[d])
                {
                    DInfo info;
                    uint64_t g = GCD(d1.p, d2.p);
                    info.p = d1.p * d2.p / g;
                    Merge(d1.p, d2.p, g, info.p, d1.vp1, d2.vp1, info.vp1);
                    Merge(d1.p, d2.p, g, info.p, d1.vp2, d2.vp2, info.vp2);
                    if ((info.vp1.size() > 0) || (info.vp2.size() > 0))
                        vnext.push_back(info);
                }
            }
            vcurrent.swap(vnext);
        }

        unsigned s1 = 0, s2 = 0;
        for (const DInfo& info : vcurrent)
        {
            uint64_t p1 = info.p, p2 = info.p;
            for (uint64_t x : info.vp1)
                p1 = min(x, p1);
            for (uint64_t x : info.vp2)
                p2 = min(x, p2);
            assert(p1 != p2);
            if (p1 < p2)
                ++s1;
            else
                ++s2;
        }
        cout << s1 << " " << s2 << endl;
    }
	return 0;
}
