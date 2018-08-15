// https://www.hackerrank.com/challenges/the-chosen-one

#include "common/base.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/info/gcd.h"
#include "common/vector/utils.h"

using TTreap = Treap<false, true, uint64_t, BSTInfoGCD<uint64_t>>;
using TNode = TTreap::TNode;

int main_the_chosen_one_treap()
{
	unsigned n;
	cin >> n;
	vector<uint64_t> v = ReadVector<uint64_t>(n);
	if (n == 1)
	{
		cout << v[0] + 1 << endl;
		return 0;
	}
	TTreap treap(n);
	TNode * root = treap.Build(v);
	for (unsigned i = 0; i < n; ++i)
	{
		TNode* node = treap.GetNodeByRawIndex(i);
		uint64_t value = node->data;
		node->data = 0;
		UpdateInfoNodeToRoot(node);
		uint64_t gcd = root->info.gcd;
		assert(gcd > 0);
		if ((value % gcd) != 0)
		{
			cout << gcd << endl;
			return 0;
		}
		node->data = value;
		UpdateInfoNodeToRoot(node);
	}
	assert(false);
	return 0;
}
