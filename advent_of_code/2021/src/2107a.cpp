#include "common/stl/base.h"
#include "common/string/utils/split.h"
#include "common/vector/transform.h"

int main_2107a() {
  string s;
  cin >> s;
  auto v = nvector::TransformV(Split(s, ','), [](auto& s) { return stoi(s); });
  sort(v.begin(), v.end());
  int m = v[v.size() / 2], r = 0;
  for (auto x : v) r += abs(x - m);
  cout << r << endl;
  return 0;
}
