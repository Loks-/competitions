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
 * @tparam TMatrix The type of the matrix.
 * @param a The cost matrix.
 * @param max_value The maximum value in the cost matrix.
 * @return The minimum cost of the assignment.
 */
template <class TMatrix>
inline typename TMatrix::TValue Assignment(
    const TMatrix& a, const typename TMatrix::TValue& max_value) {
  assignment::Hungarian s;
  return s.Solve(a, max_value);
}

}  // namespace alg
