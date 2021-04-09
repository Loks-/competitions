#pragma once

#include "common/base.h"
#include "common/binary_search_tree/action/none.h"
#include "common/binary_search_tree/base/node.h"
#include "common/binary_search_tree/info/none.h"
#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"
#include "common/nodes_manager_fixed_size.h"
#include "common/vector/rmq/position_value.h"

#include <stack>
#include <vector>

namespace nvector {
namespace rmq {
// Convert RMQ problem to LCA problem for binary tree using Cartesian Tree.
// O(N) memory, O(N) preprocessing time, O(1) request time
template <class TTValue>
class LCA {
 public:
  using TValue = TTValue;
  using TPositionValue = PositionValue<TValue>;
  using TNode =
      bst::base::Node<TValue, bst::info::None, bst::action::None, false, false>;

 protected:
  std::vector<TValue> vv;
  graph::LCA lca;

 public:
  LCA() {}
  LCA(const std::vector<TValue>& v) { Build(v); }

  void Build(const std::vector<TValue>& v) {
    assert(v.size() > 0);
    vv = v;
    size_t n = v.size();
    // Build Cartesian Tree
    NodesManagerFixedSize<TNode> m(n);
    TNode *proot = m.New(), *plast = proot, *p0 = proot;
    proot->data = v[0];
    std::stack<TNode*> s;
    for (size_t j = 1; j < n; ++j) {
      TNode* pj = m.New();
      pj->data = v[j];
      if (!(pj->data < plast->data)) {
        plast->r = pj;
        s.push(plast);
      } else if (pj->data < proot->data) {
        for (; !s.empty();) s.pop();
        pj->l = proot;
        proot = pj;
      } else {
        for (; pj->data < s.top()->data; s.pop()) plast = s.top();
        pj->l = plast;
        s.top()->r = pj;
      }
      plast = pj;
    }
    // Convert to GraphTree
    TreeGraph t(n);
    t.SetRoot(proot - p0);
    for (size_t j = 0; j < n; ++j) {
      auto p = m.NodeByRawIndex(j);
      if (p->l) t.AddEdge(j, p->l - p0);
      if (p->r) t.AddEdge(j, p->r - p0);
    }
    lca.Build(t);
  }

  TPositionValue Minimum(size_t b, size_t e) const {
    assert(b < e);
    unsigned m = lca.GetLCA(b, e - 1);
    return {m, vv[m]};
  }
};
}  // namespace rmq
}  // namespace nvector
