#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/split_empty_string.h"

#include <numeric>

int main_2201() {
  auto vs = nvector::ReadLines();
  auto vvs = nvector::SplitEmptyString(vs);
  std::vector<int> v;
  for (auto& vsi : vvs) {
    v.push_back(0);
    for (auto& s : vsi) v.back() += stoi(s);
  }
  std::sort(v.begin(), v.end());
  for (auto l : {1u, 3u})
    cout << std::accumulate(v.rbegin(), v.rbegin() + l, 0ll) << endl;
  return 0;
}
