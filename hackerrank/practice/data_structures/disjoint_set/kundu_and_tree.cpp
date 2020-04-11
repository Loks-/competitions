// https://www.hackerrank.com/challenges/kundu-and-tree

#include "common/data_structures/disjoint_set.h"
#include "common/stl/base.h"

int main_kundu_and_tree() {
  unsigned N, i, j;
  char c;
  cin >> N;
  ds::DisjointSet djs(N);
  for (unsigned k = 1; k < N; ++k) {
    cin >> i >> j >> c;
    if (c == 'b') djs.Union(i - 1, j - 1);
  }
  vector<unsigned> vr = djs.GetRepresentatives();
  uint64_t S1 = 0, S2 = 0, S3 = 0;
  for (unsigned i = 0; i < vr.size(); ++i) {
    uint64_t s = djs.GetSize(vr[i]);
    S1 += s;
    S2 += s * s;
    S3 += s * s * s;
  }
  cout << ((S1 * S1 * S1 - 3 * S1 * S2 + 2 * S3) / 6) % 1000000007 << endl;
  return 0;
}
