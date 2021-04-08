// https://www.hackerrank.com/challenges/good-point

#include "common/geometry/d2/ellipse_io.h"
#include "common/geometry/d2/line.h"
#include "common/geometry/d2/point_io.h"
#include "common/optimization/minimum_2d.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <iomanip>

int main_good_point() {
  cout << setprecision(5) << fixed;
  unsigned n, ni, m;
  cin >> n;
  vector<D2Line> vl;
  for (unsigned in = 0; in < n; ++in) {
    cin >> ni;
    auto vi = nvector::Read<D2Point>(ni);
    for (unsigned i = 0; i < ni; ++i) {
      vl.push_back(D2Line(vi[(i + 1) % ni], vi[i]));
      vl.back().Normalize();
    }
  }
  cin >> m;
  auto ve = nvector::Read<D2Ellipse>(m);

  auto f = [&](double x, double y) {
    D2Point p(x, y);
    double fv = -1e8;
    for (auto& l : vl) fv = max(fv, l(p));
    for (auto& e : ve) fv = max(fv, e(p));
    return fv;
  };
  auto p = opt::Minimum2DSafe(f, -1e4, 1e4, -1e4, 1e4, 1e-8);
  cout << p.x << endl << p.y << endl;
  return 0;
}
