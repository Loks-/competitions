#include "common/stl/base.h"
#include "common/vector/read_lines.h"

int main_2125() {
  auto vs = nvector::ReadLines(), vs2(vs);
  unsigned n = vs.size(), m = vs[0].size();
  for (unsigned s = 1;; ++s) {
    bool stop = true;
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = 0; j < m; ++j) {
        if ((vs[i][j] == '>') && (vs[i][(j + 1) % m] == '.')) {
          swap(vs2[i][j], vs2[i][(j + 1) % m]);
          stop = false;
        }
      }
    }
    vs = vs2;
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = 0; j < m; ++j) {
        if ((vs[i][j] == 'v') && (vs[(i + 1) % n][j] == '.')) {
          swap(vs2[i][j], vs2[(i + 1) % n][j]);
          stop = false;
        }
      }
    }
    vs = vs2;
    if (stop) {
      cout << s << endl;
      break;
    }
  }
  return 0;
}
