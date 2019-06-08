// https://www.hackerrank.com/challenges/cat-jogging

#include "common/graph/graph.h"
#include "common/graph/graph/degeneracy_order.h"
#include "common/graph/graph/sort_edges_by_order.h"
#include "common/stl/base.h"
#include "common/unsigned_multiset.h"

int main_jogging_cats() {
  unsigned N, M;
  cin >> N >> M;
  UndirectedGraph g(N);
  g.ReadEdges(M);
  auto order = DegeneracyOrder(g);
  SortEdgesByOrder(g, order, true);
  uint64_t s = 0;
  UnsignedMultiSet ums(N);
  for (unsigned u : order) {
    for (unsigned v : g.Edges(u)) {
      auto& vedges = g.Edges(v);
      assert(vedges.back() == u);
      vedges.pop_back();
      for (unsigned w : vedges) ums.Increase(w);
    }
    for (unsigned i : ums.List()) {
      uint64_t x = ums.Count(i);
      s += x * (x - 1) / 2;
    }
    ums.Clear();
  }
  cout << s << endl;
  return 0;
}
