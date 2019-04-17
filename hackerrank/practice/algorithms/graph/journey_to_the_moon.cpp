// https://www.hackerrank.com/challenges/journey-to-the-moon

#include "common/graph/graph.h"
#include "common/graph/graph/connected_components.h"
#include "common/stl/base.h"

int main_journey_to_the_moon() {
  unsigned n, p;
  cin >> n >> p;
  UndirectedGraph g(n);
  g.ReadEdges(p, true);
  auto gcc = ConnectedComponents(g);
  uint64_t s = uint64_t(n) * (n - 1);
  for (const auto& v : gcc) s -= v.size() * (v.size() - 1);
  cout << s / 2 << endl;
  return 0;
}
