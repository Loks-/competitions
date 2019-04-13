#pragma once

#include "common/graph/graph/distance.h"
#include "common/graph/tree.h"

template <class TGraph>
inline std::vector<unsigned> Deep(const graph::Tree<TGraph>& tree) {
  return DistanceFromSource(tree, tree.GetRoot());
}
