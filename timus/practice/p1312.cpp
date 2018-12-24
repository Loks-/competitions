// http://acm.timus.ru/problem.aspx?space=1&num=1312

#include "common/stl/base.h"
#include "common/vector/read.h"

#include <cmath>
#include <iomanip>

int main_p1312() {
  cout << setprecision(4) << fixed;
  auto sqr = [](double x) { return x * x; };

  int64_t h, w;
  cin >> w >> h;
  vector<int64_t> vr = ReadVector<int64_t>(3);
  bool impossible = false;
  for (unsigned r : vr) {
    if (2 * r > min(h, w)) impossible = true;
  }
  if (impossible) {
    cout << 0 << endl;
    return 0;
  }

  bool found = false;
  double x1, x2, x3, y1, y2, y3;
  unsigned rotate = 0;
  vector<unsigned> vp{0, 1, 2};

  auto Print = [&]() {
    if (rotate) {
      swap(x1, y1);
      swap(x2, y2);
      swap(x3, y3);
    }
    vector<pair<double, double>> vpd{{x1, y1}, {x2, y2}, {x3, y3}};
    for (unsigned i = 0; i < 3; ++i) {
      for (unsigned j = 0; j < 3; ++j) {
        if (vp[j] == i) cout << vpd[j].first << " " << vpd[j].second << " ";
      }
    }
    cout << endl;
    found = true;
  };

  for (; !found && rotate < 2; ++rotate) {
    if (rotate) swap(h, w);
    for (unsigned t = 0; t < 6; ++t) {
      int64_t r1 = vr[vp[0]], r2 = vr[vp[1]], r3 = vr[vp[2]];
      x1 = r1, y1 = r1, y2 = h - r2;
      x2 =
          max((y2 - y1 < r1 + r2) ? x1 + sqrt(sqr(r1 + r2) - sqr(y2 - y1)) : x1,
              double(r2));
      if (x2 + r2 > w + 1e-10) {
        next_permutation(vp.begin(), vp.end());
        continue;
      }
      x3 = w - r3, y3 = r3;
      if ((sqr(x3 - x1) + sqr(y3 - y1) >= sqr(r3 + r1) - 1e-10) &&
          (sqr(x3 - x2) + sqr(y3 - y2) >= sqr(r3 + r2) - 1e-10)) {
        Print();
        break;
      }
      y3 = h - r3;
      if ((sqr(x3 - x1) + sqr(y3 - y1) >= sqr(r3 + r1) - 1e-10) &&
          (sqr(x3 - x2) + sqr(y3 - y2) >= sqr(r3 + r2) - 1e-10)) {
        Print();
        break;
      }
      y3 =
          max((x3 - x1 < r1 + r3) ? y1 + sqrt(sqr(r1 + r3) - sqr(x3 - x1)) : x1,
              double(r3));
      if ((y3 + r3 <= h + 1e-10) &&
          (sqr(x3 - x2) + sqr(y3 - y2) >= sqr(r3 + r2) - 1e-10)) {
        Print();
        break;
      }
      next_permutation(vp.begin(), vp.end());
    }
  }
  if (!found) cout << 0 << endl;
  return 0;
}
