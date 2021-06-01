#pragma once

#include "common/base.h"
#include "common/graph/cnone.h"

#include <vector>

namespace graph {
namespace distance {
namespace spfa {
// Double linked list representation for childs for Tarjan and ZDO algorithms.
class ParentTree {
 public:
  std::vector<unsigned> vl, vr, vp, vc;

  ParentTree(unsigned size)
      : vl(size, CNone), vr(size, CNone), vp(size, CNone), vc(size, 0) {}

  void Reset(unsigned n) {
    vc[n] = 0;
    vp[n] = CNone;
  }

  void SetRoot(unsigned r) { vl[r] = vr[r] = r; }

  void SetParent(unsigned c, unsigned p) {
    assert(vc[c] == 0);
    vr[c] = vr[p];
    vl[vr[c]] = c;
    vl[c] = p;
    vr[p] = c;
    vp[c] = p;
    ++vc[p];
  }
};
}  // namespace spfa
}  // namespace distance
}  // namespace graph
