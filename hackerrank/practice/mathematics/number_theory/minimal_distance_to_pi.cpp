// https://www.hackerrank.com/challenges/minimal-distance-to-pi

#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/continued_fraction/convergent.h"
#include "common/numeric/fraction.h"
#include "common/numeric/long_unsigned.h"
#include "common/stl/base.h"

int main_minimal_distance_to_pi()
{
    int64_t l, r;
    cin >> l >> r;
    l -= 1;
    int64_t nn = 1, nd = 0, dn = 0, dd = 1;
    // Based on https://oeis.org/A001203 .
    vector<int64_t> pi_seq { 3, 7, 15, 1, 292, 1, 1, 1, 2, 1, 3, 1, 14, 2, 1, 1, 2, 2, 2, 2, 1, 84, 2, 1, 1, 15, 3, 13, 1, 4, 2, 6, 6, 99, 1, 2, 2, 6, 3, 5, 1, 1, 6, 8, 1, 7, 1, 2, 3, 7, 1, 2, 1, 1, 12, 1, 1, 1, 3, 1, 1, 8, 1, 1, 2, 1, 6, 1, 1, 5, 2, 2, 3, 1, 2, 4, 4, 16, 1, 161, 45, 1, 22, 1, 2, 2, 1, 4, 1, 2, 24, 1, 2, 1, 3, 1, 2, 1 };
    ContinuedFraction bestl(TIFraction(3, 1)), bestr(TIFraction(4, 1)), cf_pi(pi_seq);
    unsigned i = 0;
    for (; ; ++i)
    {
        int64_t a = pi_seq[i], tnn = nn * a + nd, tnd = nn, tdn = dn * a + dd, tdd = dn;
        bool solution_exist = false;
        if (l / tdn < r / tdn)
        {
            solution_exist = true;
            TIFraction f(tnn, tdn);
            if (i & 1)
                bestr = f;
            else
                bestl = f;                
        }
        if (l / (tdn + tdd) < r / (tdn + tdd))
        {
            solution_exist = true;
            TIFraction f(tnn + tnd, tdn + tdd);
            if (i & 1)
                bestl = f;
            else
                bestr = f;
        }
        if (!solution_exist)
        {
            for (int64_t n = l / (tdn + tdd) + 1; n * tdn <= r; ++n)
            {
                int64_t ld = (l - tdn * n + tdd) / tdd, rd = (r - tdn * n + tdd) / tdd;
                if (ld < rd)
                {
                    solution_exist = true;
                    int64_t d = ld;
                    TIFraction f(tnn * n + tnd * d, tdn * n + tdd * d);
                    ContinuedFraction cf(f);
                    if (cf < cf_pi)
                    {
                        if (bestl < cf)
                            bestl = cf;
                    }
                    else
                    {
                        if (cf < bestr)
                            bestr = cf;
                    }
                    break;
                }
            }
        }
        if (!solution_exist) break;
        nn = tnn; nd = tnd; dn = tdn; dd = tdd;
    }
    unsigned maxi = min(bestl.Size(), bestr.Size());
    nn = 1, nd = 0, dn = 0, dd = 1;
    for (i = 0; i < maxi; ++i)
    {
        int64_t a = pi_seq[i], tnn = nn * a + nd, tnd = nn, tdn = dn * a + dd, tdd = dn;
        if ((bestl(i) != pi_seq[i]) || (bestr(i) != pi_seq[i])) break;
        nn = tnn; nd = tnd; dn = tdn; dd = tdd;
    }
    if (i < 6)
    {
        // Fix later
        int64_t a = pi_seq[i], tnn = nn * a + nd, tnd = nn, tdn = dn * a + dd, tdd = dn;
        nn = tnn; nd = tnd; dn = tdn; dd = tdd;
    }

    for (int64_t n = l / (dn + dd) + 1; n * dn <= r; ++n)
    {
        int64_t ld = dd ? (l - dn * n + dd) / dd : 0, rd = dd ? (r - dn * n + dd) / dd : 1;
        for (int64_t d = ld; d < rd; ++d)
        {
            TIFraction f(nn * n + nd * d, dn * n + dd * d);
            ContinuedFraction cf(f);
            if (cf < cf_pi)
            {
                if (bestl < cf)
                    bestl = cf;
            }
            else
            {
                if (cf < bestr)
                    bestr = cf;
            }
        }
    }

    CFConvergent cfit(cf_pi);
    for (; cfit.Get().GetD() < 10 * r; cfit.Next()) ;
    TIFraction fpi = cfit.Get(), fl = bestl.ToFraction(), fr = bestr.ToFraction(), bestf;
    {
        LongUnsigned lul1 = LongUnsigned(uint64_t(fpi.GetD())) * uint64_t(fl.GetN()) * uint64_t(fr.GetD());
        LongUnsigned lul2 = LongUnsigned(uint64_t(fpi.GetD())) * uint64_t(fl.GetD()) * uint64_t(fr.GetN());
        LongUnsigned lur = LongUnsigned(uint64_t(fpi.GetN())) * uint64_t(fl.GetD()) * uint64_t(fr.GetD()) * 2u;
        bestf = (lul1 + lul2 < lur) ? fr : fl;
    }

    int64_t fn = bestf.GetN(), fd = bestf.GetD(), k = (l / fd) + 1;
    cout << fn * k << "/" << fd * k << endl;
	return 0;
}
