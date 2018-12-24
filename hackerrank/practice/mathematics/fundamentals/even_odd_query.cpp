// https://www.hackerrank.com/challenges/even-odd-query

#include "common/stl/base.h"
#include "common/vector/read.h"

int main_even_odd_query() {
  int N, Q, x, y;
  cin >> N;
  vector<int> vx = ReadVector<int>(N);
  vx.insert(vx.begin(), 1);
  cin >> Q;
  for (; Q; --Q) {
    cin >> x >> y;
    bool b = (x > y) ? true
                     : (vx[x] == 0)
                           ? false
                           : (vx[x] & 1) ? true
                                         : (x >= y) ? false : (vx[x + 1] == 0);
    cout << (b ? "Odd" : "Even") << endl;
  }
  return 0;
}
