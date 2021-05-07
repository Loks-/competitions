// https://www.hackerrank.com/challenges/polygon

#include "common/geometry/d2/polygon.h"
#include "common/geometry/d2/point_io.h"
#include "common/geometry/kdtree/d2/isp_tree.h"
#include "common/geometry/kdtree/info/sum.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

using TTree = geometry::kdtree::D2ISPTree<
    int64_t, unsigned, geometry::kdtree::idata::None,
    geometry::kdtree::info::Sum<unsigned, geometry::kdtree::info::None>>;

int main_polygon__isp_tree() {
  unsigned N, Q, M;
  I2Point pb(-200001, -200001), pe(200001, 200001);
  TTree tree;
  tree.Init(pb, pe);
  cin >> N >> Q;
  for (auto p : nvector::Read<I2Point>(N)) tree.Add(p, 1u);
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    cin >> M;
    auto vq = nvector::Read<I2Point>(M);
    auto ap = I2Polygon(vq);
    unsigned s0 = 0, s1 = 0;
    auto p0 = ap[ap.Size() - 1], p1 = ap[0], p2 = ap[1];
    for (unsigned i = 0; i < ap.Size(); ++i) {
      bool ix = (p0.y < p1.y) || (p2.y < p1.y),
           iy = (p0.x < p1.x) || (p2.x < p1.x), i1 = (i & 1);
      (i1 ? s0 : s1) += tree.GetInfo(pb, {p1.x + ((ix != i1) ? 1 : 0),
                                          p1.y + ((iy != i1) ? 1 : 0)})
                            .sum;
      p0 = p1;
      p1 = p2;
      p2 = ap[(i + 2) % ap.Size()];
    }
    cout << s1 - s0 << endl;
  }
  return 0;
}
