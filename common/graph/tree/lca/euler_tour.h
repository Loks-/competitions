#pragma once

#include "common/base.h"
#include "common/graph/tree.h"
#include "common/vector/rmq/ppt.h"

#include <algorithm>
#include <stack>
#include <vector>

namespace graph {
namespace lca {
// Use Euler Tour to convert problem to RMQ1.
// Time: O(V) preprocessing, O(1) lca
template <class TRMQ>
class EulerTour {
 protected:
  std::vector<unsigned> vu, vd, vl;
  TRMQ rmq;

 public:
  EulerTour() {}

  template <class TGraph>
  explicit EulerTour(const Tree<TGraph>& g) {
    Build(g);
  }

  template <class TGraph>
  void Build(const Tree<TGraph>& g) {
    unsigned n = g.Size(), n2 = 2 * n - 1, r = g.GetRoot(), d = 0;
    assert(n > 0);
    vu.clear();
    vu.reserve(n2);
    vd.clear();
    vd.reserve(n2);
    vl.clear();
    vl.resize(n, CNone);
    std::vector<unsigned> vi(n, 0);
    std::stack<unsigned> s;
    for (s.push(r); !s.empty();) {
      unsigned u = s.top();
      vl[u] = vu.size();
      vu.push_back(u);
      vd.push_back(d);
      for (auto& i = vi[u]; i < g.Edges(u).size(); ++i) {
        if (vl[g.Edges(u)[i]] == CNone) break;
      }
      if (vi[u] != g.Edges(u).size()) {
        s.push(g.Edges(u)[vi[u]++]);
        ++d;
      } else {
        s.pop();
        --d;
      }
    }
    rmq.Build(vd);
  }

  unsigned GetLCA(unsigned x, unsigned y) const {
    unsigned ix = vl[x], iy = vl[y];
    return vu[rmq.Minimum(std::min(ix, iy), std::max(ix, iy) + 1).pos];
  }
};
}  // namespace lca
}  // namespace graph
