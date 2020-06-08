// https://www.hackerrank.com/challenges/heavy-light-2-white-falcon

#include "common/graph/tree.h"
#include "common/graph/tree/hld_sdp.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using THLD = graph::HLDSDP<TModularD, 2>;
using TVector = THLD::TVector;

int main_heavy_light_2_white_falcon() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadEdges(true);
  THLD hld(tree);
  hld.InitSInfo([&](unsigned, unsigned d) { return TVector{1, d}; });

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    unsigned t, u, v;
    cin >> t >> u >> v;
    if (t == 1) {
      TModularD x;
      cin >> x;
      unsigned a = hld.LCA(u, v), d = hld.Deep(a);
      auto y = x * (hld.DistanceFromAncestor(a, u) + 1);
      hld.PathFromAncestor(a, u, false).AddAction(TVector{y + x * d, -x});
      if (v != a)
        hld.PathFromAncestor(a, v, true).AddAction(TVector{y - x * d, x});
    } else if (t == 2) {
      cout << hld.Path(u, v).GetInfo().sum << endl;
    }
  }
  return 0;
}
