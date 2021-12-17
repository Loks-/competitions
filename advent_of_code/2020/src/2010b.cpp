#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2010b() {
  auto v = nvector::ReadAll<int64_t>();
  sort(v.begin(), v.end());
  v.insert(v.begin(), 0);
  v.push_back(v.back() + 3);
  vector<uint64_t> vc(v.size(), 0);
  vc[0] = 1;
  for (unsigned i = 1; i < v.size(); ++i) {
    for (unsigned j = (i > 3) ? i - 3 : 0; j < i; ++j) {
      if (v[i] - v[j] < 4) vc[i] += vc[j];
    }
  }
  cout << vc.back() << endl;
  return 0;
}
