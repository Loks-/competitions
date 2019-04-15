#include "common/graph/bipartite.h"
#include "common/graph/bipartite/matching.h"
#include "common/stl/base.h"

int main_costume_change() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N;
    cin >> N;
    vector<BipartiteGraph> vg(2 * N + 1, BipartiteGraph(N, N));
    int sc;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        cin >> sc;
        vg[sc + N].AddEdge(i, j + N);
      }
    }
    unsigned s = 0;
    for (const BipartiteGraph& g : vg) s += BipartiteMatching(g);
    cout << "Case #" << it << ": " << N * N - s << endl;
  }
  return 0;
}
