#include "common/graph/graph.h"
#include "common/graph/graph/distance.h"
#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/read_lines.h"

#include <unordered_map>

int main_2007a() {
  vector<string> vs = nvector::ReadLines();
  unordered_map<string, unsigned> msi;
  unsigned index = 0;
  vector<pair<unsigned, unsigned>> vp;
  for (auto s : vs) {
    auto ss = Split(s, ' ');
    auto key = ss[0] + " " + ss[1];
    if (msi.find(key) == msi.end()) msi[key] = index++;
    auto ukey = msi[key];
    for (unsigned i = 1; 4 * i + 3 < ss.size(); ++i) {
      auto key2 = ss[4 * i + 1] + " " + ss[4 * i + 2];
      if (msi.find(key2) == msi.end()) msi[key2] = index++;
      vp.push_back({ukey, msi[key2]});
    }
  }
  DirectedGraph g(index);
  for (auto p : vp) g.AddEdge(p.second, p.first);
  auto vd = DistanceFromSource(g, msi["shiny gold"]);
  unsigned r = 0;
  for (auto& v : vd) {
    if (v < index) ++r;
  }
  cout << r - 1 << endl;
  return 0;
}
