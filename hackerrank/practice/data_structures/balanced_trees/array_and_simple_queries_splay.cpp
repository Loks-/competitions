// https://www.hackerrank.com/challenges/array-and-simple-queries

#include "common/base.h"
#include "common/binary_search_tree/traversal.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/utils/rotate.h"
#include "common/vector/utils.h"

using TSplayTree = SplayTree<false, int>;
using TNode = TSplayTree::TNode;

int main_array_and_simple_queries_splay()
{
	unsigned N, M;
	cin >> N >> M;
	vector<int> va = ReadVector<int>(N);
	TSplayTree splay_tree(N);
	TNode* root = splay_tree.Build(va);
	for (; M; --M)
	{
		unsigned t, i, j;
		cin >> t >> i >> j;
		if (t == 1)
		{
			Rotate<TSplayTree>(root, 0, i - 1, j);
		}
		else
		{
			Rotate<TSplayTree>(root, i - 1, j, N);
		}
	}
	vector<int> vf = BSTTraversal::Traverse<TNode, int>(root, EBST_Inorder);
	cout << abs(vf[0] - vf.back()) << endl;
	WriteVector(vf);
	return 0;
}
