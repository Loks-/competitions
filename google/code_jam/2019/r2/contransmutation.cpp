#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/graph/graph/distance_to_sink.h"
#include "common/modular_io.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

#include <queue>

int main_contransmutation() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    DirectedGraph g(n + 1);
    for (unsigned i = 0; i < n; ++i) {
      unsigned r1, r2;
      cin >> r1 >> r2;
      g.AddEdge(i, r1 - 1);
      g.AddEdge(i, r2 - 1);
    }
    vector<TModularD> vc = nvector::Read<TModularD>(n);
    for (unsigned i = 0; i < n; ++i) {
      if (vc[i].Get()) g.AddEdge(n, i);
    }
    auto vd1 = DistanceFromSource(g, n);
    auto vd2 = DistanceToSink(g, 0);
    vector<bool> valid(n + 1, false);
    for (unsigned i = 0; i < n; ++i)
      valid[i] = (vd1[i] != -1u) && (vd2[i] != -1u);
    vector<unsigned> edges_in(n, 0);
    queue<unsigned> q;
    for (unsigned i = 0; i < n; ++i) {
      if (!valid[i]) continue;
      for (unsigned j : g.InvertedEdges(i)) {
        if (valid[j]) ++edges_in[i];
      }
      if (!edges_in[i]) q.push(i);
    }
    for (; !q.empty(); q.pop()) {
      unsigned i = q.front();
      valid[i] = false;
      if (i == 0) break;
      for (unsigned j : g.Edges(i)) {
        if (!valid[j]) continue;
        vc[j] += vc[i];
        if (--edges_in[j] == 0) q.push(j);
      }
    }
    bool unbounded = false;
    TModularD s = 0;
    for (unsigned i = 0; i < n; ++i) {
      if (i && !valid[i]) continue;
      s += vc[i];
      unsigned e = 0;
      for (unsigned j : g.Edges(i)) {
        if (valid[j]) ++e;
      }
      if (e > 1) unbounded = true;
    }
    cout << "Case #" << it << ": "
         << (unbounded ? "UNBOUNDED" : to_string(s.Get())) << endl;
  }
  return 0;
}
