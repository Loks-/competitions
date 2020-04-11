#include "common/data_structures/unsigned_set.h"
#include "common/linear_algebra/matrix.h"
#include "common/linear_algebra/utils/read.h"
#include "common/stl/base.h"

int main_vestigium() {
  unsigned T, N;
  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N;
    auto M = ReadMatrix<unsigned>(N);
    unsigned t = 0, r = 0, c = 0;
    ds::UnsignedSet sr(N + 1), sc(N + 1);
    for (unsigned i = 0; i < N; ++i) {
      t += M(i, i);
      for (unsigned j = 0; j < N; ++j) {
        sr.Insert(M(i, j));
        sc.Insert(M(j, i));
      }
      if (sr.Size() != N) ++r;
      if (sc.Size() != N) ++c;
      sr.Clear();
      sc.Clear();
    }
    cout << "Case #" << it + 1 << ": " << t << " " << r << " " << c << endl;
  }
  return 0;
}
