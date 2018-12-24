// https://www.hackerrank.com/challenges/lazy-white-falcon

#include "common/binary_indexed_tree/bit.h"
#include "common/graph/base_tree/base_tree.h"
#include "common/graph/base_tree/lca.h"
#include "common/graph/base_tree/nodes_info.h"
#include "common/stl/base.h"

using TBIT = BIT<>;

int main_lazy_white_falcon() {
  unsigned N, Q;
  cin >> N >> Q;
  TBaseTree tree(N);
  tree.ReadEdges(true);
  BaseTreeNodesInfo nodes_info(tree);
  LCA lca(tree);
  TBIT bit(N);
  vector<int64_t> current_values(N, 0);

  auto Calc = [&](unsigned from) {
    unsigned index = nodes_info.preorder[from];
    return bit.Sum(index + 1);
  };

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, x;
    cin >> t >> u >> x;
    if (t == 1) {
      unsigned l = nodes_info.preorder[u], r = l + nodes_info.subtree_size[u];
      int64_t d = int64_t(x) - current_values[u];
      bit.Add(l, d);
      bit.Add(r, -d);
      current_values[u] = x;
    } else if (t == 2) {
      unsigned z = lca.GetLCA(u, x);
      cout << (Calc(u) + Calc(x) - Calc(z) -
               ((z != tree.GetRoot()) ? Calc(nodes_info.parent[z]) : 0))
           << endl;
    }
  }
  return 0;
}
