#pragma once

#include "common/base.h"

#include <algorithm>
#include <vector>

namespace alg {
namespace assignment {

// Hungarian algorithm.
// https://en.wikipedia.org/wiki/Hungarian_algorithm
// O(n^2 * m) time
// All the elements should be less than max_value.
class Hungarian {
 public:
  /**
   * @brief Solves the assignment problem using the Hungarian algorithm.
   *
   * @tparam TMatrix The type of the matrix.
   * @param a The cost matrix.
   * @param max_value The maximum value in the cost matrix.
   * @return The minimum cost of the assignment.
   */
  template <class TMatrix>
  typename TMatrix::TValue Solve(const TMatrix& a,
                                 const typename TMatrix::TValue& max_value) {
    using TValue = typename TMatrix::TValue;
    n = a.Rows();
    m = a.Columns();
    assert(n <= m);  // Ensure the number of rows is less than or equal to the
                     // number of columns
    p.resize(m + 1);
    std::fill(p.begin(), p.end(), n);
    std::vector<unsigned> used(m + 1), way(m);
    std::vector<TValue> u(n + 1), v(m + 1), vt(m + 1);

    for (unsigned i = 0; i < n; ++i) {
      p[m] = i;
      std::fill(used.begin(), used.end(), 0);
      std::fill(vt.begin(), vt.end(), max_value);
      unsigned j0 = m, j1 = m;

      // Main loop to find the minimum cost matching
      while (p[j0] != n) {
        used[j0] = true;
        const unsigned i0 = p[j0];
        TValue delta = max_value;

        // Update potentials and find the minimum delta
        for (unsigned j = 0; j < m; ++j) {
          if (!used[j]) {
            const auto c = a(i0, j) - u[i0] - v[j];
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

        // Update dual variables
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

      // Update the matching
      while (j0 < m) {
        j1 = way[j0];
        p[j0] = p[j1];
        j0 = j1;
      }
    }
    return -v[m];
  }

  /**
   * @brief Retrieves the optimal assignment.
   *
   * @return A vector containing the optimal assignment.
   */
  std::vector<unsigned> Get() const {
    std::vector<unsigned> result(n);
    for (unsigned j = 0; j < m; ++j) {
      if (p[j] != n) {
        result[p[j]] = j;
      }
    }
    return result;
  }

 protected:
  std::vector<unsigned> p;  // Array to store the matchings
  unsigned n, m;            // Dimensions of the cost matrix
};

}  // namespace assignment
}  // namespace alg
