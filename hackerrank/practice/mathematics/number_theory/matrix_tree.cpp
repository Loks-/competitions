// https://www.hackerrank.com/challenges/matrix-tree

#include "common/graph/tree.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <functional>

int main_matrix_tree() {
  unsigned N;
  cin >> N;
  vector<TModularD> vw = ReadVector<TModularD>(N);
  TreeGraph tree(N);
  tree.ReadEdges();

  std::function<TModularD(unsigned, unsigned, TModularD)> SolveR =
      [&](unsigned node, unsigned p, TModularD pw) -> TModularD {
    TModularD r = vw[node] - pw;
    for (unsigned c : tree.Edges(node)) {
      if (c == p) continue;
      r *= SolveR(c, node, vw[node]);
    }
    return r;
  };

  cout << SolveR(0, 0, 0) << endl;
  return 0;
}
