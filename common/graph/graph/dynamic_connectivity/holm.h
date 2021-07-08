#pragma once

#include "common/graph/tree/euler_tour_trees.h"

#include <vector>

namespace graph {
namespace dc {
// Insert - O(log^2 N) amortized
// Remove - O(log^2 N) amortized
// Check  - O(log N)
class Holm {
 protected:
  std::vector<EulerTourTrees> vf;

 public:
  void Reset(unsigned size) {}

  explicit Holm(unsigned size) { Reset(size); }
};
}  // namespace dc
}  // namespace graph
