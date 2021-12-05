#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2103a() {
  unsigned n, l;
  cin >> n >> l;
  vector<unsigned> v;
  for (unsigned i = 0; i < n; ++i) {
    string s;
    cin >> s;
    v.push_back(stoi(s, nullptr, 2));
  }
  unsigned x = 0;
  for (unsigned i = 0; i < l; ++i) {
    unsigned m = (1u << i), c = 0;
    for (auto u : v) {
      if (u & m) ++c;
    }
    if (c >= n / 2) x |= m;
  }
  cout << x * ((1u << l) - x - 1) << endl;
  return 0;
}
