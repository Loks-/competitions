// https://www.hackerrank.com/challenges/merging-communities

#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"

int main_merging_communities() {
  unsigned N, Q;
  cin >> N >> Q;
  ds::DisjointSet djs(N + 1);
  for (; Q; --Q) {
    char c;
    unsigned i, j;
    cin >> c;
    if (c == 'Q') {
      cin >> i;
      cout << djs.GetSize(i) << endl;
    } else if (c == 'M') {
      cin >> i >> j;
      djs.Union(i, j);
    }
  }
  return 0;
}
