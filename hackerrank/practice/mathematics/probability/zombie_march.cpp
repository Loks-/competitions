// https://www.hackerrank.com/challenges/zombie-march

#include "common/graph/graph.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/sum.h"

int main_zombie_march() {
  unsigned T;
  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    unsigned n, m, k;
    cin >> n >> m >> k;
    UndirectedGraph graph(n);
    graph.ReadEdges(m, true);
    vector<double> va = nvector::Read<double>(n);

    // Base solution. Works only for connected non-bipartite graphs and (k >>
    // n).
    double sa = nvector::Sum(va);
    vector<double> vf(n);
    for (unsigned i = 0; i < n; ++i)
      vf[i] = (sa / (2 * m)) * graph.Edges(i).size();
    sort(vf.begin(), vf.end());
    reverse(vf.begin(), vf.end());
    for (unsigned i = 0; i < 5; ++i) cout << unsigned(vf[i] + 0.5) << " ";
    cout << endl;
  }
  return 0;
}
