#include "common/stl/base.h"
#include "common/string/utils/parse.h"
#include "common/vector/read.h"

int main_2103a() {
  unsigned n, l;
  cin >> n >> l;
  vector<unsigned> v;
  for (unsigned i = 0; i < n; ++i) {
    string s;
    cin >> s;
    v.push_back(Parse<unsigned>(s, 2));
  }
  unsigned x = 0;
  for (unsigned i = 0; i < l; ++i) {
    unsigned m = (1u << i), c = 0;
    for (auto u : v) {
      if (u & m) ++c;
    }
    if (c >= n / 2) x |= m;
  }
  unsigned y = ((1ull << l) - 1) ^ x;
  cout << x * y << endl;
  return 0;
}
