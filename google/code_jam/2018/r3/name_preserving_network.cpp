#include "common/linear_algebra/matrix.h"
#include "common/modular/static/modular.h"
#include "common/modular/static/modular_io.h"
#include "common/stl/base.h"

#include <unordered_map>

using TModular = Modular<>;
using TMatrix = la::Matrix<TModular>;

int main_name_preserving_network() {
  auto generate_random_graph = [](unsigned n) {
    vector<unsigned> v(4 * n);
    for (unsigned i = 0; i < 4 * n; ++i) v[i] = i / 4;
    TMatrix m(n, n);
    for (bool next_try = true; next_try;) {
      next_try = false;
      m.Clear();
      m.SetDiagonal(1);
      random_shuffle(v.begin(), v.end());
      for (unsigned i = 0; i < 4 * n; i += 2) {
        bool found = false;
        for (unsigned j = i + 1; j < 4 * n; ++j) {
          if (m(v[i], v[j]) == 0) {
            found = true;
            m(v[i], v[j]) = m(v[j], v[i]) = 1;
            swap(v[i + 1], v[j]);
            break;
          }
        }
        if (!found) {
          next_try = true;
          break;
        }
      }
    }
    return m;
  };

  unsigned T;
  cin >> T;
  for (unsigned it = 1; it <= T; ++it) {
    unsigned N, N2;
    cin >> N >> N2;
    unsigned trun = 0, k;
    unordered_map<uint64_t, unsigned> um;
    TMatrix mg(0);
    for (bool next_try = true; next_try; ++trun) {
      auto m = generate_random_graph(N);
      mg = m;
      for (k = 0; k < 20; ++k) {
        um.clear();
        for (unsigned i = 0; i < N; ++i) um[m(i, i).Get()] = i;
        if (um.size() == N) {
          next_try = false;
          break;
        }
        m *= m;
      }
    }
    cout << N << endl;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = i + 1; j < N; ++j) {
        if (mg(i, j).Get()) cout << i + 1 << " " << j + 1 << endl;
      }
    }
    cin >> N2;
    mg.Clear();
    mg.SetDiagonal(1);
    for (unsigned i = 0; i < 2 * N; ++i) {
      unsigned v1, v2;
      cin >> v1 >> v2;
      mg(v1 - 1, v2 - 1) = mg(v2 - 1, v1 - 1) = 1;
    }
    for (unsigned i = 0; i < k; ++i) mg *= mg;
    vector<unsigned> vr(N);
    for (unsigned i = 0; i < N; ++i) vr[um[mg(i, i).Get()]] = i;
    for (unsigned x : vr) cout << x + 1 << " ";
    cout << endl;
  }
  return 0;
}
