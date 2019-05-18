#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/modular/static/modular.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/full.h"
#include "common/vector/read.h"

using TModular = Modular<>;

int main_contransmutation() {
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned n;
    cin >> n;
    vector<unsigned> u0 = ReadVector<unsigned>(2);
    DirectedGraph g(n);
    for (unsigned i = 1; i < n; ++i) {
      unsigned r1, r2;
      cin >> r1 >> r2;
      g.AddEdge(i, r1 - 1);
      g.AddEdge(i, r2 - 1);
    }
    vector<TModular> vc = ReadVector<TModular>(n);
    TModular last_value = 0;
    unsigned last_change = 0;
    bool unbounded = false;
    for (unsigned i = 0; i < last_change + n + 1; ++i) {
      for (unsigned j = 1; j < n; ++j) {
        TModular k = vc[j];
        vc[g.Edges(j)[0]] += vc[j];
        vc[g.Edges(j)[1]] += vc[j];
        vc[j] -= k;
      }
      if (vc[0] != last_value) {
        last_change = i;
        last_value = vc[0];
      }
      if (i > n * (n + 1) + 100) {
        unbounded = true;
        break;
      }
    }
    if (unbounded)
      cout << "Case #" << it << ": "
           << "UNBOUNDED" << endl;
    else
      cout << "Case #" << it << ": " << vc[0].Get() << endl;
  }
  return 0;
}
