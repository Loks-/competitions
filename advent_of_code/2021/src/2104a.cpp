#include "common/stl/full.h"
#include "common/string/utils/split.h"
#include "common/vector/read.h"

int main_2104a() {
  unsigned n, m = 5, r = 1000;
  cin >> n;
  string s;
  cin >> s;
  vector<vector<vector<unsigned>>> v(n);
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < m; ++j) v[i].push_back(nvector::Read<unsigned>(m));
  }

  auto CheckR = [&](unsigned i, unsigned j) {
    for (unsigned k = 0; k < m; ++k) {
      if (v[i][j][k] != r) return false;
    }
    return true;
  };

  auto CheckC = [&](unsigned i, unsigned k) {
    for (unsigned j = 0; j < m; ++j) {
      if (v[i][j][k] != r) return false;
    }
    return true;
  };

  auto vs = Split(s, ',');
  for (auto is : vs) {
    unsigned u = unsigned(stoi(is));
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = 0; j < m; ++j) {
        for (unsigned k = 0; k < m; ++k) {
          if (v[i][j][k] == u) {
            v[i][j][k] = r;
            if (CheckR(i, j) || CheckC(i, k)) {
              unsigned s = 0;
              for (unsigned j0 = 0; j0 < m; ++j0) {
                for (unsigned k0 = 0; k0 < m; ++k0) {
                  if ((v[i][j0][k0] != r) && (v[i][j0][k0] != u))
                    s += v[i][j0][k0];
                }
              }
              cout << s * u << endl;
              return 0;
            }
          }
        }
      }
    }
  }
  return 0;
}
