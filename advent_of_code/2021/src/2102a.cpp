#include "common/stl/base.h"

int main_2102a() {
  unsigned n;
  cin >> n;
  int x = 0, y = 0, d;
  for (unsigned i = 0; i < n; ++i) {
    string s;
    cin >> s >> d;
    if (s[0] == 'f') x += d;
    if (s[0] == 'd') y += d;
    if (s[0] == 'u') y -= d;
  }
  cout << x * y << endl;
  return 0;
}
