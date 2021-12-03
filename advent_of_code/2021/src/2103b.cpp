#include "common/stl/base.h"
#include "common/string/utils/parse.h"
#include "common/vector/read.h"

int main_2103b() {
  unsigned n, l;
  cin >> n >> l;
  vector<unsigned> v;
  for (unsigned i = 0; i < n; ++i) {
    string s;
    cin >> s;
    v.push_back(Parse<unsigned>(s, 2));
  }
  sort(v.begin(), v.end());

  auto Search = [&](bool major) {
    auto x = v[0];
    for (unsigned i = l - 1, l = 0, r = v.size(); l + 1 < r; --i) {
      unsigned m = (1u << i), m2 = m * 2;
      unsigned t = (v[l] & ~(m2 - 1)) + m;
      auto s = lower_bound(v.begin(), v.end(), t) - v.begin();
      if ((r - s >= s - l) == major) {
        l = s;
        x = v[l];
      } else {
        r = s;
      }
    }
    return x;
  };

  cout << Search(true) * Search(false) << endl;
  return 0;
}
