// https://www.hackerrank.com/challenges/mehta-and-the-typical-supermarket

#include "common/numeric/utils/gcd.h"
#include "common/stl/base.h"
#include "common/vector/read.h"

int main_mehta_and_the_typical_supermarket() {
  uint64_t mv = -1ull;
  unsigned N, T;
  cin >> N;
  vector<uint64_t> va = ReadVector<uint64_t>(N);
  vector<uint64_t> vlcm(1, 1), vsign(1, mv);
  for (unsigned j = 0; j < N; ++j) {
    size_t l = vlcm.size();
    for (size_t i = 0; i < l; ++i) {
      vsign.push_back(vsign[i] * mv);
      uint64_t g = GCD(vlcm[i], va[j]);
      vlcm.push_back(mv / g > vlcm[i] ? vlcm[i] * (va[j] / g) : mv);
    }
  }

  auto Solve = [&](uint64_t r) {
    uint64_t s = 0;
    for (size_t j = 1; j < vlcm.size(); ++j) s += vsign[j] * (r / vlcm[j]);
    return s;
  };

  cin >> T;
  for (unsigned iT = 0; iT < T; ++iT) {
    uint64_t L, R;
    cin >> L >> R;
    cout << Solve(R) - Solve(L - 1) << endl;
  }
  return 0;
}
