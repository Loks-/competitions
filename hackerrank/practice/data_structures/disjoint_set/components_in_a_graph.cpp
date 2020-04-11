// https://www.hackerrank.com/challenges/components-in-graph

#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"

int main_components_in_a_graph() {
  unsigned N, i, j;
  cin >> N;
  ds::DisjointSet djs(2 * N + 1);
  for (unsigned k = 0; k < N; ++k) {
    cin >> i >> j;
    djs.Union(i, j);
  }
  unsigned mins = 2 * N, maxs = 1;
  for (unsigned k = 1; k <= N; ++k) {
    unsigned s = djs.GetSize(k);
    if (s > 1) {
      mins = min(mins, s);
      maxs = max(maxs, s);
    }
  }
  cout << mins << " " << maxs << endl;
  return 0;
}
