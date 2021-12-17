#include "common/stl/base.h"
#include "common/vector/read_all.h"

int main_2010a() {
  auto v = nvector::ReadAll<int64_t>();
  sort(v.begin(), v.end());
  v.insert(v.begin(), 0);
  v.push_back(v.back() + 3);
  vector<unsigned> vc(4, 0);
  for (unsigned i = 1; i < v.size(); ++i) vc[v[i] - v[i - 1]]++;
  cout << vc[1] * vc[3] << endl;
  return 0;
}
