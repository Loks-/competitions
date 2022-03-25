#include "common/geometry/d3/axis/rectangle.h"
#include "common/geometry/d3/base.h"
#include "common/geometry/d3/utils/box.h"
#include "common/geometry/kdtree/d3/base_tree.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <queue>

using TTree = geometry::kdtree::D3BaseTree<int64_t, unsigned>;
using TNode = TTree::TNode;
using geometry::kdtree::base::DSet;

int main_2122() {
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
    vector<I3Point> vpa;
    for (auto& s : vs) {
      auto ss = Split(s, ' ');
      unsigned v = (ss[0] == "on" ? 1 : 0);
      auto ss1 = Split(ss[1], ',');
      I3Point p1, p2;
      for (unsigned i = 0; i < 3; ++i) {
        auto ss2 = Split(ss1[i].substr(2), '.');
        p1[i] = stoi(ss2[0]);
        p2[i] = stoi(ss2[2]) + 1;
        vv3[i].push_back(p1[i]);
        vv3[i].push_back(p2[i]);
      }
      if (ab || (rl.Inside(p1) && rl.Inside(p2 - v1))) {
        vi.push_back({v, p1, p2});
        vpa.push_back(p1);
        vpa.push_back(p2);
      }
    }
    auto box = Box(vpa);
    TTree tree;
    tree.Init(box.p1, box.p2);
    for (auto& s : vi) tree.Set(s.p1, s.p2, s.v);
    int64_t r = 0;
    queue<QNode> q;
    for (q.push({tree.root, tree.sb, tree.se}); !q.empty(); q.pop()) {
      auto p = q.front();
      if (p.p->IsLeaf()) {
        int64_t rp = p.p->ldata;
        for (unsigned i = 0; i < 3; ++i) rp *= p.e[i] - p.b[i];
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
