#pragma once

#include "common/graph/graph.h"
#include "common/graph/graph/shortest_cycle/undirected_bfs.h"

// Time: O(V (V+E))
inline unsigned Girth(UndirectedGraph& g) { return graph::scycle::BFS(g); }
