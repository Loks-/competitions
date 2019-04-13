// https://www.hackerrank.com/challenges/jenny-subtrees

#include "common/graph/graph/distance.h"
#include "common/graph/graph/subgraph.h"
#include "common/graph/tree.h"
#include "common/graph/tree/isomorphic_hash.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_jennys_subtrees() {
  unsigned n, r;
  cin >> n >> r;
  TreeGraph tree(n), tree_temp;
  tree.ReadEdges();
  unordered_set<size_t> hash_set;
  vector<bool> keep(n);
  vector<unsigned> distance;
  for (unsigned i = 0; i < n; ++i) {
    distance = DistanceFromSource(tree, i);
    for (unsigned j = 0; j < n; ++j) keep[j] = (distance[j] <= r);
    tree_temp = graph::Subgraph(tree, keep);
    hash_set.insert(IsomorphicHash(tree_temp));
  }
  cout << hash_set.size() << endl;
  return 0;
}
