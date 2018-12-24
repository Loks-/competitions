// https://www.hackerrank.com/challenges/pairs-again

#include "common/stl/base.h"
#include "common/vector/unsigned_set.h"

#include <unordered_set>

int main_satisfactory_pairs() {
  unsigned n;
  cin >> n;
  vector<vector<unsigned>> vvd(n);
  for (unsigned i = n - 1; i; --i) {
    for (unsigned ix = i; ix < n; ix += i) vvd[ix].push_back(i);
  }
  UnsignedSet us(n);
  uint64_t total = 0;
  for (unsigned i = 1; i < n; ++i) {
    for (unsigned ix = i; ix < n; ix += i) {
      for (unsigned j : vvd[n - ix]) {
        if (i < j)
          us.Insert(j);
        else
          break;
      }
    }
    total += us.Size();
    us.Clear();
  }
  cout << total << endl;
  return 0;
}
