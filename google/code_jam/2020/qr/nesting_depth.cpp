#include "common/stl/base.h"

#include <ostream>
#include <sstream>
#include <string>

int main_nesting_depth() {
  unsigned T;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    string s;
    ostringstream o;
    cin >> s;
    char last = '0';
    for (char c : s) {
      for (; last < c; ++last) o << "(";
      for (; c < last; --last) o << ")";
      o << c;
    }
    for (; '0' < last; --last) o << ")";
    cout << "Case #" << it + 1 << ": " << o.str() << endl;
  }
  return 0;
}
