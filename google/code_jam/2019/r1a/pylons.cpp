#include "common/graph/graph.h"
#include "common/graph/graph/hamiltonian_path_undirected_graph.h"
#include "common/hash.h"
#include "common/stl/base.h"

int main_pylons() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned R, C;
    cin >> R >> C;
    unsigned N = R * C;
    UndirectedGraph g(N);
    for (unsigned i = 0; i < N; ++i) {
      unsigned ir = i / C, ic = i % C;
      for (unsigned j = 0; j < N; ++j) {
        unsigned jr = j / C, jc = j % C;
        if ((ir == jr) || (ic == jc) || (ir + ic == jr + jc) ||
            (ir + jc == ic + jr))
          continue;
        g.AddEdge(i, j);
      }
    }
    vector<unsigned> hpath = HamiltonianPath(g);
    cout << "Case #" << it << ": "
         << (hpath.empty() ? "IMPOSSIBLE" : "POSSIBLE") << endl;
    for (unsigned u : hpath) {
      cout << u / C + 1 << " " << u % C + 1 << endl;
    }
  }
  return 0;
}
