// https://www.hackerrank.com/challenges/real-estate-broker

#include "common/graph/bipartite.h"
#include "common/graph/bipartite/matching.h"
#include "common/stl/base.h"

#include <numeric>

int main_real_estate_broker() {
  unsigned n, m, a, p;
  cin >> n >> m;
  vector<unsigned> va, vp;
  for (unsigned i = 0; i < n + m; ++i) {
    cin >> a >> p;
    va.push_back(a);
    vp.push_back(p);
  }
  BipartiteGraph g(n, m);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = n; j < n + m; ++j) {
      if ((va[i] <= va[j]) && (vp[i] >= vp[j])) {
        g.AddEdge(i, j);
      }
    }
  }
  cout << BipartiteMatching(g) << endl;
  return 0;
}
