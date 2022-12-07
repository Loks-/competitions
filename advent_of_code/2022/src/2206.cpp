#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2206() {
  auto vs = nvector::ReadLines();
  auto& s = vs[0];
  for (unsigned l : {4, 14}) {
    for (unsigned i = 0;; ++i) {
      bool ok = true;
      for (unsigned j = 0; ok && (j < l); ++j) {
        for (unsigned k = j + 1; ok && (k < l); ++k) {
          if (s[i + j] == s[i + k]) ok = false;
        }
      }
      if (ok) {
        cout << i + l << endl;
        break;
      }
    }
  }
  return 0;
}
