// https://www.hackerrank.com/challenges/lazy-white-falcon

#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/segment_tree/info/sum.h"
#include "common/stl/base.h"

int main_lazy_white_falcon__hld() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  graph::HLD<unsigned, st::info::Sum<unsigned>> hld(tree);

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, x;
    cin >> t >> u >> x;
    if (t == 1)
      hld.SetData(u, x);
    else if (t == 2)
      cout << hld.Path(u, x).GetInfo().sum << endl;
  }
  return 0;
}
