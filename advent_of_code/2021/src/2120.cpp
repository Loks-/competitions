#include "common/stl/base.h"
#include "common/vector/make.h"
#include "common/vector/read_lines.h"
#include "common/vector/sum.h"

int main_2120() {
  auto vs = nvector::ReadLines();
  vector<unsigned> vm;
  for (auto c : vs[0]) vm.push_back((c == '#') ? 1 : 0);
  unsigned n = vs.size() - 2, m = vs[2].size();
  for (unsigned steps : {2, 50}) {
    unsigned shift = steps + 2, N = n + 2 * shift, M = m + 2 * shift;
    auto v = nvector::Make<unsigned>(N, M);
    for (unsigned i = 0; i < n; ++i) {
      auto& s = vs[i + 2];
      for (unsigned j = 0; j < s.size(); ++j)
        v[i + shift][j + shift] = ((s[j] == '#') ? 1 : 0);
    }
    auto v2 = v;
    unsigned d = 0;
    for (unsigned is = 0; is < steps; ++is) {
      for (unsigned i = 0; i + 2 < N; ++i) {
        for (unsigned j = 0; j + 2 < M; ++j) {
          unsigned x = 0;
          for (unsigned di = 0; di < 3; ++di) {
            for (unsigned dj = 0; dj < 3; ++dj) {
              x = 2 * x + v[i + di][j + dj];
            }
          }
          v2[i + 1][j + 1] = vm[x];
        }
      }
      d = vm[d * 511];
      for (unsigned i = 0; i < N; ++i) v2[i][0] = v2[i][M - 1] = d;
      for (unsigned j = 0; j < M; ++j) v2[0][j] = v2[N - 1][j] = d;
      v.swap(v2);
    }
    cout << nvector::SumVV(v) << endl;
  }
  return 0;
}
