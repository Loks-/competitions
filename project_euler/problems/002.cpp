#include "common/stl/base.h"

int main_002() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    uint64_t n, f1 = 1, f2 = 1, s = 0;
    cin >> n;
    for (; f2 <= n;) {
      if ((f2 % 2) == 0) s += f2;
      f2 += f1;
      f1 = f2 - f1;
    }
    cout << s << endl;
  }
  return 0;
}
