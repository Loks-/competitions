// https://www.hackerrank.com/challenges/fibonacci-numbers-tree

#include "common/binary_indexed_tree/bit.h"
#include "common/graph/base_tree/base_tree.h"
#include "common/graph/base_tree/lca.h"
#include "common/graph/base_tree/nodes_info.h"
#include "common/modular/static/fibonacci_matrix.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

using TModular = Modular<>;
using TBIT = BIT<TModular>;
using TFibonacci = ModularFibonacciMatrix<TModular>;

int main_fibonacci_numbers_tree() {
  unsigned N, Q;
  cin >> N >> Q;
  TBaseTree tree(N);
  tree.ReadTreeEdges();
  BaseTreeNodesInfo nodes_info(tree);
  LCA lca(tree);
  TBIT bit_c(N), bit_f0(N), bit_f1(N);

  auto Calc = [&](unsigned from) {
    unsigned index = nodes_info.preorder[from];
    TModular x0 = bit_f0.Sum(index + 1);
    TModular x1 = bit_f1.Sum(index + 1);
    TModular c = bit_c.Sum(index + 1);
    return TFibonacci::GetU(nodes_info.deep[from] + 2, x0, x1) - x1 + c;
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
               ((z > 0) ? Calc(nodes_info.parent[z]) : TModular(0)))
           << endl;
    } else if (c == 'U') {
      unsigned x;
      int64_t kx, kb;
      cin >> x >> kx;
      x -= 1;
      unsigned l = nodes_info.preorder[x], r = l + nodes_info.subtree_size[x];
      kb = kx - nodes_info.deep[x];
      auto p0 = TFibonacci::GetPS(kb + 1);
      auto pk = TFibonacci::GetPS(kx + 1);
      bit_f0.Add(l, p0.first);
      bit_f0.Add(r, -p0.first);
      bit_f1.Add(l, p0.second);
      bit_f1.Add(r, -p0.second);
      bit_c.Add(l, p0.second - pk.second);
      bit_c.Add(r, -p0.second + pk.second);
    }
  }
  return 0;
}
