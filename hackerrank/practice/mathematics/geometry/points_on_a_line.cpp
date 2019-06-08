// https://www.hackerrank.com/challenges/points-on-a-line

#include "common/stl/base.h"

#include <unordered_set>

int main_points_on_a_line() {
  unsigned n;
  cin >> n;
  unordered_set<int> mx, my;
  for (unsigned i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    mx.insert(x);
    my.insert(y);
  }
  cout << (((mx.size() == 1) || (my.size() == 1)) ? "YES" : "NO") << endl;
  return 0;
}
