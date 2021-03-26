#include "common/data_structures/edit_distance/string_base.h"
#include "common/stl/base.h"

int main_naming_compromise() {
  ds::ed::StringBase ed;
  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    string s1, s2;
    cin >> s1 >> s2;
    auto vs = ed.Sequence(s1, s2);
    cout << "Case #" << it << ": " << vs[vs.size() / 2] << endl;
  }
  return 0;
}
