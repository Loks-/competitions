#include "common/cover/latin_square.h"
#include "common/linear_algebra/utils/write.h"
#include "common/stl/base.h"

#include <functional>

int main_indicium() {
  unsigned T, N, K;
  vector<size_t> rows;
  cover::LatinSquare ls;

  std::function<bool(size_t, size_t, size_t)> SearchAny =
      [&](size_t trace_left, size_t index, size_t value) -> bool {
    if (index == N) {
      if (trace_left) return false;
      return ls.SearchAny();
    }
    value = std::min(value, trace_left);
    for (; (value <= N) && (trace_left <= value * (N - index)); --value) {
      ls.Set(index, index, value);
      if (SearchAny(trace_left - value, index + 1, value)) return true;
      ls.EraseLast();
    }
    return false;
  };

  cin >> T;
  for (unsigned it = 0; it < T; ++it) {
    cin >> N >> K;
    K -= N;
    ls.Init(N);
    // ls.Set(0, 0, 0);
    bool b = SearchAny(K, 0, N - 1);
    // bool b = ls.SearchAny();
    cout << "Case #" << it + 1 << ": " << (b ? "POSSIBLE" : "IMPOSSIBLE")
         << endl;
    if (b) {
      WriteMatrix(ls.GetSolution() + la::Matrix<size_t>(N, N, 1));
    }

    // ls.SearchAny();
    // auto m = ls.GetSolution();
    // m += la::Matrix<size_t>(N, N, 1);
    // WriteMatrix(m);
  }
  return 0;
}
