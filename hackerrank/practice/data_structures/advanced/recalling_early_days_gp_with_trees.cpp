// https://www.hackerrank.com/challenges/recalling-early-days-gp-with-trees

#include "common/graph/tree.h"
#include "common/graph/tree/hld_sdp.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TModular = TModular_P32<100711433>;
using THLD = graph::HLDSDP<TModular, 2>;
using TVector = THLD::TVector;

int main_recalling_early_days_gp_with_trees() {
  unsigned N, R, U, Q;
  cin >> N >> R;
  TreeGraph tree(N);
  tree.ReadEdges();
  THLD hld(tree);

  TModular r(R), ri = (r ? r.Inverse() : 0);
  hld.InitSInfo([&](unsigned, unsigned d) {
    return TVector{r.PowU(d), ri.PowU(d)};
  });

  cin >> U >> Q;
  unsigned a, b;
  TModular x;
  for (unsigned iU = 0; iU < U; ++iU) {
    cin >> a >> b >> x;
    a -= 1;
    b -= 1;
    if (r) {
      unsigned c = hld.LCA(a, b), d = hld.Deep(c);
      auto y = x * r.PowU(hld.DistanceFromAncestor(c, a));
      hld.PathFromAncestor(c, a, false).AddAction(TVector{0, y * r.PowU(d)});
      if (b != c)
        hld.PathFromAncestor(c, b, true).AddAction(TVector{y * ri.PowU(d), 0});
    } else {
      hld.SetValue(a, hld.GetValue(a) + x);
    }
  }
  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    cin >> a >> b;
    cout << hld.Path(a - 1, b - 1).GetInfo().segment_sum << endl;
  }
  return 0;
}
