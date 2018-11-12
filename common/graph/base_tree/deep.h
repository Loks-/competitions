#pragma once

#include "base_tree.h"
#include "../base_graph/distance.h"

inline std::vector<unsigned> GetDeep(const BaseTree& tree) { return GetDistancesFromSource(tree, tree.GetRoot()); }
