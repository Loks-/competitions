// https://www.hackerrank.com/challenges/jenny-subtrees

#include "common/graph/base_tree/base_tree.h"
#include "common/graph/base_tree/isomorphic_hash.h"
#include "common/graph/base_graph/distance.h"
#include "common/graph/base_graph/subgraph.h"
#include "common/stl/base.h"

#include <unordered_set>

int main_jennys_subtrees()
{
	unsigned n, r;
	cin >> n >> r;
	BaseTree tree(n), tree_temp;
	tree.ReadEdges();
	unordered_set<size_t> hash_set;
	vector<bool> keep(n);
	vector<unsigned> distance;
	for (unsigned i = 0; i < n; ++i)
	{
		distance = GetDistancesFromSource(tree, i);
		for (unsigned j = 0; j < n; ++j)
			keep[j] = (distance[j] <= r);
		BaseSubgraph::CreateSubgraph(tree, keep, tree_temp);
		hash_set.insert(BaseTreeIsomorphicHash::Hash(tree_temp));
	}
	cout << hash_set.size() << endl;
	return 0;
}
