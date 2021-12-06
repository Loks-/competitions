#include "common/stl/base.h"
#include "common/vector/read_all.h"
#include "common/vector/transform.h"

int main_2103a() {
  auto vs = nvector::ReadAll<string>();
  unsigned n = vs.size(), l = vs[0].size();
  auto v = nvector::TransformV(vs, [](auto& s) { return stoi(s, nullptr, 2); });
  unsigned x = 0;
  for (unsigned i = 0; i < l; ++i) {
    unsigned m = (1u << i), c = 0;
    for (auto u : v) c += u & m;
    if (c / m >= n / 2) x |= m;
  }
  cout << x * ((1u << l) - x - 1) << endl;
  return 0;
}
