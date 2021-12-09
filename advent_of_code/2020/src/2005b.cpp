#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"

int main_2005b() {
  vector<string> vs = nvector::ReadLines();
  auto v = nvector::TransformV(vs, [](auto s) {
    unsigned x = 0;
    for (char c : s) x = 2 * x + (((c == 'B') || (c == 'R')) ? 1 : 0);
    return x;
  });
  sort(v.begin(), v.end());
  for (unsigned i = 1; i < v.size(); ++i) {
    if (v[i] - v[i - 1] > 1) {
      cout << v[i] - 1 << endl;
    }
  }
  return 0;
}
