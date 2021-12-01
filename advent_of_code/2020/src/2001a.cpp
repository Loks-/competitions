#include "common/stl/base.h"
#include "common/vector/read.h"

int main_2001a() {
  unsigned n, m = 2020;
  cin >> n;
  auto v = nvector::Read<unsigned>(n);
  sort(v.begin(), v.end());
  for (unsigned i = 0, j = n; i < j;) {
    unsigned a = v[i], b = v[j - 1];
    if (a + b < m) {
      ++i;
    } else if (a + b > m) {
      --j;
    } else {
      cout << a << "\t" << b << endl;
      cout << a * b << endl;
      break;
    }
  }
  return 0;
}
