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
   * @tparam Matrix The type of the matrix.
   * @param cost_matrix The cost matrix.
   * @param max_value The maximum value in the cost matrix.
   * @return The minimum cost of the assignment.
   */
  template <typename Matrix>
  typename Matrix::ValueType solve(
      const Matrix& cost_matrix, const typename Matrix::ValueType& max_value) {
    using ValueType = typename Matrix::ValueType;
    rows_ = cost_matrix.Rows();
    cols_ = cost_matrix.Columns();
    assert(rows_ <= cols_);  // Ensure the number of rows is less than or equal
                             // to the number of columns
    matches_.resize(cols_ + 1);
    std::fill(matches_.begin(), matches_.end(), rows_);
    std::vector<unsigned> used(cols_ + 1), way(cols_);
    std::vector<ValueType> u(rows_ + 1), v(cols_ + 1), vt(cols_ + 1);

    for (unsigned i = 0; i < rows_; ++i) {
      matches_[cols_] = i;
      std::fill(used.begin(), used.end(), 0);
      std::fill(vt.begin(), vt.end(), max_value);
      unsigned j0 = cols_, j1 = cols_;

      // Main loop to find the minimum cost matching
      while (matches_[j0] != rows_) {
        used[j0] = true;
        const unsigned i0 = matches_[j0];
        ValueType delta = max_value;

        // Update potentials and find the minimum delta
        for (unsigned j = 0; j < cols_; ++j) {
          if (!used[j]) {
            const auto c = cost_matrix(i0, j) - u[i0] - v[j];
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
        for (unsigned j = 0; j <= cols_; ++j) {
          if (used[j]) {
            u[matches_[j]] += delta;
            v[j] -= delta;
          } else {
            vt[j] -= delta;
          }
        }
        j0 = j1;
      }

      // Update the matching
      while (j0 < cols_) {
        j1 = way[j0];
        matches_[j0] = matches_[j1];
        j0 = j1;
      }
    }
    return -v[cols_];
  }

  /**
   * @brief Retrieves the optimal assignment.
   *
   * @return A vector containing the optimal assignment.
   */
  [[nodiscard]] std::vector<unsigned> get_assignment() const {
    std::vector<unsigned> result(rows_);
    for (unsigned j = 0; j < cols_; ++j) {
      if (matches_[j] != rows_) {
        result[matches_[j]] = j;
      }
    }
    return result;
  }

 protected:
  std::vector<unsigned> matches_;  // Array to store the matchings
  unsigned rows_, cols_;           // Dimensions of the cost matrix
};

}  // namespace assignment
}  // namespace alg
