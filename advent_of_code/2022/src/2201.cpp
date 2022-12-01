#include "common/stl/base.h"
#include "common/vector/read_lines.h"

#include <numeric>

int main_2201() {
  auto vs = nvector::ReadLines();
  int current = 0;
  std::vector<int> v;
  for (auto& s : vs) {
    if (s.empty()) {
      v.push_back(current);
      current = 0;
    } else {
      current += stoi(s);
    }
  }
  v.push_back(current);
  std::sort(v.begin(), v.end());
  for (auto l : {1u, 3u})
    cout << std::accumulate(v.rbegin(), v.rbegin() + l, 0ll) << endl;
  return 0;
}
