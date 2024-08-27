// https://www.hackerrank.com/challenges/fibonacci-numbers-tree

#include "common/graph/tree.h"
#include "common/graph/tree/hld_sdp.h"
#include "common/modular/static/fibonacci_matrix.h"
#include "common/modular_io.h"
#include "common/stl/base.h"

using TFibonacci = modular::mstatic::FibonacciMatrix<TModularD>;
using THLD = graph::HLDSDP<TModularD, 2>;
using TVector = THLD::TVector;

int main_fibonacci_numbers_tree__hldsdp() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadTreeEdges();
  THLD hld(tree);
  hld.InitSInfo([](unsigned, unsigned d) {
    auto p = TFibonacci::GetPU(d);
    return TVector{p.first, p.second};
  });

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    char c;
    cin >> c;
    if (c == 'Q') {
      unsigned x, y;
      cin >> x >> y;
      cout << hld.Path(x - 1, y - 1).GetInfo().sum_first << endl;
    } else if (c == 'U') {
      unsigned x;
      int64_t kx;
      cin >> x >> kx;
      x -= 1;
      auto p0 = TFibonacci::GetPS(kx - hld.Deep(x) + 1);
      hld.Subtree(x).AddAction(TVector{p0.first, p0.second});
    }
  }
  return 0;
}
