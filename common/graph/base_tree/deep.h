#pragma once

#include "base_tree.h"
#include "../base_graph/distance.h"

template<class TTBaseTree>
inline std::vector<unsigned> GetDeep(const TTBaseTree& tree) { return GetDistancesFromSource(tree, tree.GetRoot()); }
