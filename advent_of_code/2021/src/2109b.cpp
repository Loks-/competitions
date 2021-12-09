#include "common/data_structures/disjoint_set.h"
#include "common/stl/full.h"
#include "common/stl/pair_io.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"
#include "common/vector/read_all.h"
#include "common/vector/read_from_line.h"
#include "common/vector/read_lines.h"
#include "common/vector/transform.h"
#include "common/vector/write.h"

int main_2109b() {
  // auto v = nvector::ReadAll<int64_t>();
  //   auto v = nvector::ReadFromLine<int64_t>(',');
  auto vs = nvector::ReadLines();
  for (auto& s : vs) s = '9' + s + '9';
  int l = vs[0].size();
  string s9(l, '9');
  vs.push_back(s9);
  vs.insert(vs.begin(), s9);
  int m = vs.size();
  vector<pair<int, int>> vd{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  ds::DisjointSet s(l * m);
  vector<int> vl;
  int64_t r = 0;
  for (int i = 1; i + 1 < m; ++i) {
    for (int j = 1; j + 1 < l; ++j) {
      if (vs[i][j] == '9') continue;
      bool f = false;
      bool ok = true;
      for (auto d : vd) {
        if (vs[i + d.first][j + d.second] < vs[i][j]) {
          f = true;
          s.Union(i * l + j, (i + d.first) * l + j + d.second);
        }
        if (vs[i + d.first][j + d.second] <= vs[i][j] - 1) {
          ok = false;
        }
      }
      if (ok) {
        vl.push_back(i * l + j);
      } else if (!f) {
        cout << i << "\t" << j << endl;
      }
    }
  }
  // nvector::Write(vl);
  for (auto& x : vl) x = s.GetSize(x);
  sort(vl.begin(), vl.end());
  reverse(vl.begin(), vl.end());
  cout << r << endl;
  cout << vl[0] * vl[1] * vl[2] << endl;
  return 0;
}
