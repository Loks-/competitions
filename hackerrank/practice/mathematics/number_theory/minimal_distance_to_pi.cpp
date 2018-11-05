// https://www.hackerrank.com/challenges/minimal-distance-to-pi

#include "common/numeric/continued_fraction/continued_fraction.h"
#include "common/numeric/fraction_io.h"
#include "common/stl/base.h"

#include <cmath>

#include "common/vector/write.h"

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
                    break;
                }
            }
        }
        if (!solution_exist) break;
        nn = tnn; nd = tnd; dn = tdn; dd = tdd;
    }
    // cout << bestl.ToFraction() << "\t" << bestr.ToFraction() << endl;
    // WriteVector(bestl.GetVector());
    // WriteVector(bestr.GetVector());

    for (int64_t n = l / (dn + dd) + 1; n * dn <= r; ++n)
    {
        int64_t ld = (l - dn * n + dd) / dd, rd = (r - dn * n + dd) / dd;
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
    // cout << bestl.ToFraction() << "\t" << bestr.ToFraction() << endl;
    // WriteVector(bestl.GetVector());
    // WriteVector(bestr.GetVector());

    auto F2D = [](const TIFraction& f) { return ((long double)(f.GetN())) / ((long double)(f.GetD())); };
    unsigned first_diff = 0;
    for (; first_diff < bestl.Size() && first_diff < bestr.Size(); ++first_diff)
    {
        if (bestl(first_diff) != pi_seq[first_diff]) break;
        if (bestr(first_diff) != pi_seq[first_diff]) break;
    }
    first_diff &= ~1;
    long double dl = F2D(ContinuedFraction(vector<int64_t>(bestl.GetVector().begin() + first_diff, bestl.GetVector().end())).ToFraction());
    long double dr = F2D(ContinuedFraction(vector<int64_t>(bestr.GetVector().begin() + first_diff, bestr.GetVector().end())).ToFraction());
    long double pi = F2D(ContinuedFraction(vector<int64_t>(pi_seq.begin() + first_diff, pi_seq.begin() + first_diff + 10)).ToFraction());
    // cout << pi - dl << "\t" << dr - pi << endl;
    TIFraction bestf = ((dl + dr < 2 * pi) ? bestr.ToFraction() : bestl.ToFraction());

    int64_t fn = bestf.GetN(), fd = bestf.GetD(), k = (l / fd) + 1;
    cout << fn * k << "/" << fd * k << endl;
	return 0;
}
