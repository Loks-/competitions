#include "common/data_structures/coordinate_compression.h"
#include "common/geometry/d3/axis/rectangle.h"
#include "common/geometry/d3/base.h"
#include "common/geometry/kdtree/d3/isp_tree.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <queue>

using TTree = geometry::kdtree::D3ISPTree<size_t, unsigned>;
using TNode = TTree::TNode;
using geometry::kdtree::base::DSet;

int main_2122__cc_isp() {
  struct Input {
    unsigned v;
    I3Point p1, p2;
  };

  struct QNode {
    TNode* p;
    TTree::TPoint b;
    TTree::TPoint e;
  };

  const int maxv = 50;
  const I3Vector v1(1, 1, 1);
  const I3Point pl = (v1 * maxv).ToPoint();
  const I3ARectagnle rl(-pl, pl);

  auto vs = nvector::ReadLines();
  for (bool ab : {false, true}) {
    vector<vector<int>> vv3(3);
    vector<Input> vi;
    for (auto& s : vs) {
      auto ss = Split(s, " =.xyz");
      unsigned v = (ss[0] == "on" ? 1 : 0);
      I3Point p1, p2;
      for (unsigned i = 0; i < 3; ++i) {
        p1[i] = stoi(ss[2 * i + 1]);
        p2[i] = stoi(ss[2 * i + 2]) + 1;
        vv3[i].push_back(p1[i]);
        vv3[i].push_back(p2[i]);
      }
      if (ab || (rl.Inside(p1) && rl.Inside(p2 - v1)))
        vi.push_back({v, p1, p2});
    }
    vector<ds::CoordinateCompression<int64_t>> vcc(3);
    for (unsigned i = 0; i < 3; ++i) vcc[i].InitUnsorted(vv3[i]);
    TTree tree;
    tree.Init({0, 0, 0}, {vcc[0].Size(), vcc[1].Size(), vcc[2].Size()});
    for (auto& s : vi) {
      auto x0 = vcc[0].GetNew(s.p1.x), x1 = vcc[0].GetNew(s.p2.x);
      auto y0 = vcc[1].GetNew(s.p1.y), y1 = vcc[1].GetNew(s.p2.y);
      auto z0 = vcc[2].GetNew(s.p1.z), z1 = vcc[2].GetNew(s.p2.z);
      tree.Set({x0, y0, z0}, {x1, y1, z1}, s.v);
    }
    int64_t r = 0;
    queue<QNode> q;
    for (q.push({tree.root, tree.sb, tree.se}); !q.empty(); q.pop()) {
      auto p = q.front();
      if (p.p->IsLeaf()) {
        int64_t rp = p.p->ldata;
        for (unsigned i = 0; i < 3; ++i)
          rp *= vcc[i].GetOld(p.e[i]) - vcc[i].GetOld(p.b[i]);
        r += rp;
      } else {
        q.push({p.p->l, p.b, DSet(p.p->split_dim, p.e, p.p->split_value)});
        q.push({p.p->r, DSet(p.p->split_dim, p.b, p.p->split_value), p.e});
      }
    }
    cout << r << endl;
  }
  return 0;
}
