#include "common/stl/base.h"

int main_2102b() {
  unsigned n;
  cin >> n;
  int64_t x = 0, y = 0, a = 0, d;
  for (unsigned i = 0; i < n; ++i) {
    string s;
    cin >> s >> d;
    if (s[0] == 'f') {
      x += d;
      y += d * a;
    }
    if (s[0] == 'd') a += d;
    if (s[0] == 'u') a -= d;
  }
  cout << x * y << endl;
  return 0;
}
