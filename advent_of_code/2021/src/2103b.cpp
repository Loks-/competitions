#include "common/stl/base.h"
#include "common/vector/read_all.h"
#include "common/vector/transform.h"

int main_2103b() {
  auto vs = nvector::ReadAll<string>();
  auto v =
      nvector::Transform(vs, [](auto& s, auto) { return stoi(s, nullptr, 2); });
  sort(v.begin(), v.end());

  auto Search = [&](bool major) {
    unsigned l = 0, r = v.size();
    for (unsigned i = vs[0].size() - 1; l + 1 < r; --i) {
      unsigned m = (1u << i), m2 = m * 2;
      unsigned t = (v[l] & ~(m2 - 1)) + m;
      auto s = lower_bound(v.begin(), v.end(), t) - v.begin();
      (((r - s >= s - l) == major) ? l : r) = s;
    }
    return v[l];
  };

  cout << Search(true) * Search(false) << endl;
  return 0;
}
