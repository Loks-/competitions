// https://www.hackerrank.com/challenges/bfsshortreach

#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/stl/base.h"

int main_breadth_first_search_shortest_reach() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n, m, k;
    cin >> n >> m;
    UndirectedGraph g(n);
    g.ReadEdges(m);
    cin >> k;
    auto vd = DistanceFromSource(g, k - 1);
    for (unsigned i = 0; i < n; ++i) {
      if (i == k - 1) continue;
      cout << ((vd[i] == unsigned(-1)) ? -1 : int(6 * vd[i])) << " ";
    }
    cout << endl;
  }
  return 0;
}
