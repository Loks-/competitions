#include "common/stl/base.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"

#include <numeric>

int main_2005a() {
  vector<string> vs = nvector::ReadLines();
  auto v = nvector::TransformV(vs, [](auto s) {
    unsigned x = 0;
    for (char c : s) x = 2 * x + (((c == 'B') || (c == 'R')) ? 1 : 0);
    return x;
  });
  cout << *max_element(v.begin(), v.end()) << endl;
  return 0;
}
