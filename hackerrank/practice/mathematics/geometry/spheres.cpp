// https://www.hackerrank.com/challenges/spheres

#include "common/geometry/d3/distance.h"
#include "common/geometry/d3/line.h"
#include "common/geometry/d3/point_io.h"
#include "common/geometry/d3/vector_io.h"
#include "common/stl/base.h"

int main_spheres() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned r1, r2;
    D3Point p1, p2;
    D3Vector v1, v2;
    cin >> r1 >> r2 >> p1 >> v1 >> p2 >> v2;
    D3Line l(p1, v1 - v2);
    double t = l.Project(p2);
    cout << ((t > 0) && (Distance(p2, l(t)) <= (r1 + r2)) ? "YES" : "NO")
         << endl;
  }
  return 0;
}
