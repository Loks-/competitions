// https://www.hackerrank.com/challenges/lazy-white-falcon

#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"
#include "common/graph/tree/sto.h"
#include "common/segment_tree/action/add_each.h"
#include "common/stl/base.h"

int main_lazy_white_falcon__sto() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  graph::STO<int64_t, st::info::None, st::action::AddEach<int64_t>> sto(tree);
  graph::LCA lca(tree);
  vector<int64_t> current_values(N, 0);

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, x;
    cin >> t >> u >> x;
    if (t == 1) {
      int64_t d = int64_t(x) - current_values[u];
      sto.Subtree(u).AddAction(d);
      current_values[u] = x;
    } else if (t == 2) {
      unsigned z = lca.GetLCA(u, x);
      cout << (sto.GetData(u) + sto.GetData(x) - sto.GetData(z) -
               ((z != tree.GetRoot()) ? sto.GetData(sto.Parent(z)) : 0))
           << endl;
    }
  }
  return 0;
}
