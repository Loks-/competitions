// https://www.hackerrank.com/challenges/heavy-light-white-falcon

#include "common/data_structures/segment_tree/info/max.h"
#include "common/graph/tree.h"
#include "common/graph/tree/hld.h"
#include "common/stl/base.h"

int main_heavy_light_white_falcon() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  graph::HLD<unsigned, ds::st::info::Max<unsigned>> hld(tree);

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, x;
    cin >> t >> u >> x;
    if (t == 1)
      hld.SetData(u, x);
    else if (t == 2)
      cout << hld.Path(u, x).GetInfo().max << endl;
  }
  return 0;
}
