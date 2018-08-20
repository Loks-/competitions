// https://www.hackerrank.com/challenges/find-the-running-median

#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/utils/median.h"
#include "common/stl_base.h"

using TTree = AVLTree<false, TEmpty>;
using TNode = TTree::TNode;

int main_find_the_running_median()
{
	unsigned N;
	cin >> N;
	TTree tree(N);
	TNode* root = 0;
	for (unsigned i = 0; i < N; ++i)
	{
		int64_t x;
		cin >> x;
		root = tree.InsertNewNode(root, TEmpty(), x);
		printf("%.1lf\n", BSTMedian<TNode, double>(root));
	}
	return 0;
}
