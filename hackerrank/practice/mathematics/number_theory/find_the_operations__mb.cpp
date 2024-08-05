// https://www.hackerrank.com/challenges/flip

#include "common/linear_algebra/bool/matrix.h"
#include "common/linear_algebra/bool/read.h"
#include "common/linear_algebra/bool/solve.h"
#include "common/linear_algebra/bool/vector.h"
#include "common/stl/base.h"

using TModular = ModularBool;
using TVector = la::VectorBool;
using TMatrix = la::MatrixBool;

int main_find_the_operations__mb() {
  unsigned N, D;
  cin >> N >> D;
  unsigned NN = N * N;
  TVector v = la::ReadVectorBool(NN), x(NN);
  TMatrix M(NN);
  for (unsigned i = 0; i < NN; ++i) {
    int i1 = int(i / N), i2 = int(i % N);
    for (unsigned j = 0; j < NN; ++j) {
      int j1 = int(j / N), j2 = int(j % N);
      if (abs(i1 - j1) + abs(i2 - j2) <= int(D))
        M.Set(i, j, ModularBool::True());
    }
  }
  if (la::Solve(M, v, x)) {
    cout << "Possible" << endl;
    uint64_t s = 0;
    for (unsigned i = 0; i < NN; ++i) s += x.GetBit(i);
    cout << s << endl;
    for (unsigned i = 0; i < NN; ++i) {
      if (x.GetBit(i)) cout << i / N << " " << i % N << endl;
    }
  } else {
    cout << "Impossible" << endl;
  }
  return 0;
}
