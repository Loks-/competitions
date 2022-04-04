#include "common/stl/base.h"

#include <string>

int main_punched_cards() {
  string s = "+-|.";
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned R, C;
    cin >> R >> C;
    cout << "Case #" << it << ":" << endl;
    for (unsigned i = 0; i < 2 * R + 1; ++i) {
      for (unsigned j = 0; j < 2 * C + 1; ++j)
        cout << ((i + j < 2) ? '.' : s[2 * (i % 2) + (j % 2)]);
      cout << endl;
    }
  }
  return 0;
}
