#include "common/stl/base.h"
#include "common/vector/read.h"

// Time: O(K*N^2)
// Memory: O(N^2)
int main_011() {
  int K = 4, N = 20;
  vector<vector<unsigned>> v;
  for (int i = 0; i < N; ++i) v.emplace_back(nvector::Read<unsigned>(N));
  vector<int> dx{1, 1, 0, -1}, dy{0, 1, 1, 1};
  unsigned max_value = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      for (unsigned d = 0; d < dx.size(); ++d) {
        unsigned value = v[i][j];
        for (int k = 1; k < K; ++k) {
          int ik = i + k * dx[d], jk = j + k * dy[d];
          value *= (((ik >= 0) && (ik < N) && (jk >= 0) && (jk < N)) ? v[ik][jk]
                                                                     : 0u);
        }
        max_value = max(max_value, value);
      }
    }
  }
  cout << max_value << endl;
  return 0;
}
