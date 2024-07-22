#include "common/factorization/factorization.h"
#include "common/graph/graph/girth.h"
#include "common/stl/base.h"
#include "common/vector/read.h"
#include "common/vector/unique.h"

int main_1325e() {
  unsigned N;
  cin >> N;
  auto v = nvector::Read<unsigned>(N);
  vector<vector<unsigned>> vvp;
  vector<unsigned> vp(1, 1u);
  for (auto u : v) {
    vector<unsigned> uvp;
    for (auto& p : Factorize(u)) {
      if (p.power & 1) {
        uvp.push_back(p.prime);
        vp.push_back(p.prime);
      }
    }
    for (; uvp.size() < 2;) uvp.push_back(1);
    vvp.push_back(uvp);
  }
  nvector::UniqueUnsorted(vp);
  UndirectedGraph g(vp.size());
  for (auto& v : vvp)
    g.AddEdge(lower_bound(vp.begin(), vp.end(), v[0]) - vp.begin(),
              lower_bound(vp.begin(), vp.end(), v[1]) - vp.begin());
  auto answer = Girth(g);
  std::cout << ((answer <= N) ? int(answer) : -1) << std::endl;
  return 0;
}
