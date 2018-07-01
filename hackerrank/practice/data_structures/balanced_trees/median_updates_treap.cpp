// https://www.hackerrank.com/challenges/median

#include "common/base.h"
#include "common/binary_search_tree/find_by_key.h"
#include "common/binary_search_tree/find_by_order.h"
#include "common/binary_search_tree/treap.h"

using TTreap = Treap<true, true, TEmpty>;
using TNode = TTreap::TNode;

int main_median_updates_treap()
{
	unsigned N;
	cin >> N;
	TTreap treap(N);
	TNode * root = 0;
	for (unsigned i = 0; i < N; ++i)
	{
		char c; int64_t x;
		cin >> c >> x;
		x *= 2;
		if (c == 'a')
		{
			root = treap.InsertNewNode(root, TEmpty(), x);
		}
		else if (c == 'r')
		{
			TNode * node = BSTFindByKey(root, x);
			if (node == 0)
			{
				cout << "Wrong!" << endl;
				continue;
			}
			root = treap.RemoveAndRelease(node);
		}
		if (root == 0)
		{
			cout << "Wrong!" << endl;
		}
		else
		{
			unsigned size = root->info.size;
			int64_t s = (BSTFindByOrder(root, (size - 1) / 2)->key + BSTFindByOrder(root, size / 2)->key) / 2;
			if (s & 1)
				printf("%.1lf\n", s / 2.0);
			else
				printf("%d\n", int(s / 2));
		}
	}
	return 0;
}
