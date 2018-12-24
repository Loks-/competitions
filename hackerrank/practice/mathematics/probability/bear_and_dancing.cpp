// https://www.hackerrank.com/challenges/bear-and-dancing

#include "common/stl/base.h"

#include <iomanip>

int main_bear_and_dancing() {
  struct Status {
    double p, ed;

    void clear() { p = ed = 0.; }
    Status() { clear(); }
  };

  cout << setprecision(10) << fixed;
  unsigned n, m;
  double r;
  cin >> n >> m >> r;
  if (n > m) swap(n, m);
  double sbp = 0, sbpd = 0;
  double sfp = 0, sfpd = 0;
  unsigned maxl = n * (m - 1) + 1;
  vector<vector<Status>> vlast(n, vector<Status>(m)), vcurrent = vlast;
  {
    // l = 0
    Status& s = vlast[n - 1][m - 1];
    s.p = 1.0;
    s.ed = 1.0;
    if (m == 1) {
      sfp += 1.0;
      sfpd += 1.0;
    }
  }
  for (unsigned l = 1; l < maxl; ++l) {
    for (unsigned in = 0; in < n; ++in) {
      for (unsigned im = 0; im < m; ++im) {
        vcurrent[in][im].clear();
        if ((in == 0) && (im == 0)) continue;  // We don't need new dances
        const Status& sl = vlast[in][im];
        if (sl.p == 0.) continue;
        double p00 = (1.0 * (n - in) * (m - im)) / (n * m);
        double p01 = (1.0 * (n - in) * im) / (n * m);
        double p10 = (1.0 * in * (m - im)) / (n * m);
        double p11 = (1.0 * in * im) / (n * m);
        double p000 = (p00 * l) / ((n - in) * (m - im));
        double p001 = (p00 * ((n - in) * (m - im) - l)) / ((n - in) * (m - im));
        double pb = p000 * r;
        double ps = p000 * (1 - r);
        double ed = 1.0;

        // Remove ps chance because it's not affect anything (except ed)
        p00 = (p00 - ps) / (1 - ps);
        p01 /= (1 - ps);
        p10 /= (1 - ps);
        p11 /= (1 - ps);
        p000 = (p000 - ps) / (1 - ps);
        p001 /= (1 - ps);
        pb /= (1 - ps);
        ed /= (1 - ps);

        // Apply pb
        sbp += pb * sl.p;
        sbpd += pb * sl.ed + pb * sl.p * (ed - 1);  // last dance is cancelled

        // Adjust other
        vcurrent[in][im].p += p001 * sl.p;
        vcurrent[in][im].ed += p001 * sl.ed + p001 * sl.p * ed;
        if (im > 0) {
          vcurrent[in][im - 1].p += p01 * sl.p;
          vcurrent[in][im - 1].ed += p01 * sl.ed + p01 * sl.p * ed;
        }
        if (in > 0) {
          vcurrent[in - 1][im].p += p10 * sl.p;
          vcurrent[in - 1][im].ed += p10 * sl.ed + p10 * sl.p * ed;
        }
        if ((in > 0) && (im > 0)) {
          vcurrent[in - 1][im - 1].p += p11 * sl.p;
          vcurrent[in - 1][im - 1].ed += p11 * sl.ed + p11 * sl.p * ed;
        }
      }
    }
    sfp += vcurrent[0][0].p;
    sfpd += vcurrent[0][0].ed;
    vlast.swap(vcurrent);
  }
  double fp = sfp / (1.0 - sbp);
  double fpd = sfpd / (1.0 - sbp) + sbpd / (1.0 - sbp);
  cout << fpd << endl;
  return 0;
}
