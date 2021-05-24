#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace alg {
namespace assignment {
// Hungarian algorithm.
// https://en.wikipedia.org/wiki/Hungarian_algorithm
// O(n^2 m) time
class Hungarian {
protected: 
std::vector<unsigned> p;
unsigned n, m;

public:
  template<class TMatrix>
  typename TMatrix::TValue Solve(const TMatrix& a, const typename TMatrix::TValue& max_value) {
    using TValue = typename TMatrix::TValue;
    n = a.Rows();
    m = a.Columns();
    assert(n <= m);
    p.resize(m + 1);
    std::fill(p.begin(), p.end(), n);
    std::vector<unsigned> used(m + 1), way(m + 1);
    std::vector<TValue> u(n+1), v(m+1), vt(m+1);
    for (unsigned i = 0; i < n; ++i) {
        p[m] = i;
        std::fill(used.begin(), used.end(), 0);
        std::fill(vt.begin(), vt.end(), max_value);
        unsigned j0 = m, j1;
        for (; p[j0] != n; ) {
            used[j0] = true;
            unsigned i0 = p[j0];
            TValue delta = max_value;
            for (unsigned j = 0; j < m; ++j) {
                if (!used[j]) {
                    auto c = a(i0, j) - u[i0] - v[j];
                    if (c < vt[j]) {
                        vt[j] = c;
                        way[j] = j0;
                    }
                    if (vt[j] < delta) {
                        delta = vt[j];
                        j1 = j;
                    }
                }
            }
            for (unsigned j = 0; j <= m; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    vt[j] -= delta;
                }
            }
            j0 = j1;
        }
        do {
            j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 < m);
    }
  return -v[m];
  }

  std::vector<unsigned> Get() const {
      std::vector<unsigned> r(n);
      for (unsigned j = 0; j < m; ++j) {
          if (p[j] != n) r[p[j]] = j;
      }
      return r;
  }
};
}  // namespace assignment
}  // namespace alg
