// https://www.hackerrank.com/challenges/hyperspace-travel

#include "common/stl/base.h"

int main_hyperspace_travel() {
  unsigned n, m, k;
  cin >> n >> m;
  k = (n - 1) / 2;
  vector<vector<int>> v(m, vector<int>(n));
  for (unsigned i = 0; i < n; ++i) {
    for (unsigned j = 0; j < m; ++j) {
      cin >> v[j][i];
    }
  }
  for (unsigned j = 0; j < m; ++j) {
    nth_element(v[j].begin(), v[j].begin() + k, v[j].end());
    cout << v[j][k] << " ";
  }
  cout << endl;
  return 0;
}
