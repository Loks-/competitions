// https://www.hackerrank.com/challenges/fibonacci-numbers-tree

#include "common/graph/tree.h"
#include "common/graph/tree/lca.h"
#include "common/graph/tree/sto.h"
#include "common/modular/static/fibonacci_matrix.h"
#include "common/modular_io.h"
#include "common/segment_tree/action/add_each.h"
#include "common/stl/base.h"

using TFibonacci = modular::mstatic::FibonacciMatrix<TModularD>;

namespace {
class FData {
 public:
  TModularD c, f0, f1;

  FData& operator+=(const FData& r) {
    c += r.c;
    f0 += r.f0;
    f1 += r.f1;
    return *this;
  }

  bool operator==(const FData& r) const {
    return (c == r.c) && (f0 == r.f0) && (f1 == r.f1);
  }

  TModularD Get(unsigned deep) const {
    return TFibonacci::GetU(deep + 2, f0, f1) - f1 + c;
  }
};
}  // namespace

int main_fibonacci_numbers_tree__sto() {
  unsigned N, Q;
  cin >> N >> Q;
  TreeGraph tree(N);
  tree.ReadTreeEdges();
  graph::STO<FData, st::info::Position<>, st::action::AddEach<FData>> sto(tree);
  graph::LCA lca(tree);

  auto Calc = [&](unsigned from) {
    return sto.GetData(from).Get(sto.Deep(from));
  };

  for (unsigned iQ = 0; iQ < Q; ++iQ) {
    char c;
    cin >> c;
    if (c == 'Q') {
      unsigned x, y, z;
      cin >> x >> y;
      x -= 1;
      y -= 1;
      z = lca.GetLCA(x, y);
      cout << (Calc(x) + Calc(y) - Calc(z) -
               ((z > 0) ? Calc(sto.Parent(z)) : TModularD(0)))
           << endl;
    } else if (c == 'U') {
      unsigned x;
      int64_t kx, kb;
      cin >> x >> kx;
      x -= 1;
      kb = kx - sto.Deep(x);
      auto p0 = TFibonacci::GetPS(kb + 1);
      auto pk = TFibonacci::GetPS(kx + 1);
      sto.Subtree(x).AddAction(
          FData{p0.second - pk.second, p0.first, p0.second});
    }
  }
  return 0;
}
