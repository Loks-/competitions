// https://www.hackerrank.com/challenges/cube-summation

#include "common/geometry/d3/base.h"
#include "common/geometry/d3/point_io.h"
#include "common/geometry/d3/vector.h"
#include "common/geometry/kdtree/d3/isp_tree.h"
#include "common/geometry/kdtree/info/sum.h"
#include "common/stl/base.h"

#include <string>

using TPoint = geometry::d3::Point<unsigned>;
using TVector = geometry::d3::Vector<unsigned>;
using TTree = geometry::kdtree::D3ISPTree<
    unsigned, int64_t, geometry::kdtree::idata::None,
    geometry::kdtree::info::Sum<int64_t, geometry::kdtree::info::None>>;

int main_cube_summation__isp_tree() {
  unsigned T, N, M;
  TPoint p1, p2;
  TVector v1(1, 1, 1);
  cin >> T;
  TTree tree;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> M;
    tree.Init(TPoint(0, 0, 0), TPoint(N + 1, N + 1, N + 1));
    for (unsigned im = 0; im < M; ++im) {
      string s;
      cin >> s;
      if (s == "UPDATE") {
        int64_t v;
        cin >> p1 >> v;
        tree.Set(p1, v);
      } else if (s == "QUERY") {
        cin >> p1 >> p2;
        cout << tree.GetInfo(p1, p2 + v1).sum << endl;
      }
    }
  }
  return 0;
}
