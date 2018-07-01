// https://www.hackerrank.com/challenges/median

#include "common/base.h"
#include "common/binary_search_tree/splay_tree.h"

using TSplayTree = SplayTree<true, TEmpty>;
using TNode = TSplayTree::TNode;

int main_median_updates_splay()
{
	unsigned N;
	cin >> N;
	TSplayTree splay_tree(N);
	TNode * root = 0;
	for (unsigned i = 0; i < N; ++i)
	{
		char c; int64_t x;
		cin >> c >> x;
		x *= 2;
		if (c == 'a')
		{
			root = splay_tree.InsertNewNode(root, TEmpty(), x);
		}
		else if (c == 'r')
		{
			TNode * node = TSplayTree::FindByKey(root, x);
			if (node == 0)
			{
				cout << "Wrong!" << endl;
				continue;
			}
			root = splay_tree.RemoveAndRelease(node);
		}
		if (root == 0)
		{
			cout << "Wrong!" << endl;
		}
		else
		{
			unsigned size = root->info.size;
			int64_t s = (TSplayTree::FindByOrder(root, (size - 1) / 2)->key + TSplayTree::FindByOrder(root, size / 2)->key) / 2;
			if (s & 1)
				printf("%.1lf\n", s / 2.0);
			else
				printf("%d\n", int(s / 2));
		}
	}
	return 0;
}
