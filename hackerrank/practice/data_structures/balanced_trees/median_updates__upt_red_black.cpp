// https://www.hackerrank.com/challenges/median

#include "common/binary_search_tree/red_black_tree.h"
#include "common/binary_search_tree/utils/median.h"
#include "common/stl_base.h"

using TTree = RedBlackTree<true, TEmpty>;
using TNode = TTree::TNode;

int main_median_updates__upt_red_black()
{
	unsigned N;
	cin >> N;
	TTree tree(N);
	TNode * root = 0;
	for (unsigned i = 0; i < N; ++i)
	{
		char c; int64_t x;
		cin >> c >> x;
		x *= 2;
		if (c == 'a')
		{
			root = tree.InsertNewNode(root, TEmpty(), x);
		}
		else if (c == 'r')
		{
			TNode * node = TTree::FindByKey(root, x);
			if (node == 0)
			{
				cout << "Wrong!" << endl;
				continue;
			}
			root = tree.RemoveAndReleaseByKey(root, x);
		}
		if (root == 0)
		{
			cout << "Wrong!" << endl;
		}
		else
		{
			unsigned size = root->info.size;
			int64_t s = BSTMedian<TNode, int64_t>(root);
			if (s & 1)
				printf("%.1lf\n", s / 2.0);
			else
				printf("%d\n", int(s / 2));
		}
	}
	return 0;
}
