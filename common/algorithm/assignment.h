#pragma once

#include "common/algorithm/assignment/hungarian.h"

namespace alg {

/**
 * @brief Solves the assignment problem using the Hungarian algorithm.
 *
 * This function is a convenience alias to run the Hungarian algorithm for the
 * assignment problem. It takes a cost matrix and a maximum value, and returns
 * the minimum cost of the assignment.
 *
 * @tparam Matrix The type of the matrix.
 * @param cost_matrix The cost matrix.
 * @param max_value The maximum value in the cost matrix.
 * @return The minimum cost of the assignment.
 */
template <typename Matrix>
[[nodiscard]] inline typename Matrix::ValueType assignment_cost(
    const Matrix& cost_matrix, const typename Matrix::ValueType& max_value) {
  assignment::Hungarian solver;
  return solver.solve(cost_matrix, max_value);
}

}  // namespace alg
