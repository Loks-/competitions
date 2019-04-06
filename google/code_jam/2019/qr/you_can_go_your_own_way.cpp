#include "common/stl/base.h"

int main_you_can_go_your_own_way() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    unsigned n;
    string s;
    cin >> n >> s;
    for (unsigned i = 0; i < s.size(); ++i) s[i] = 'S' + 'E' - s[i];
    cout << "Case #" << it + 1 << ": " << s << endl;
  }
  return 0;
}
