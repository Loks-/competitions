// https://www.hackerrank.com/challenges/array-and-simple-queries

#include "common/base.h"
#include "common/binary_search_tree/traversal.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/rotate.h"
#include "common/vector/utils.h"

using TTreap = Treap<false, false, int>;
using TNode = TTreap::TNode;

int main_array_and_simple_queries_treap()
{
	unsigned N, M;
	cin >> N >> M;
	vector<int> va = ReadVector<int>(N);
	TTreap treap(N);
	TNode* root = treap.Build(va);
	for (; M; --M)
	{
		unsigned t, i, j;
		cin >> t >> i >> j;
		if (t == 1)
		{
			Rotate<TTreap>(root, 0, i - 1, j);
		}
		else
		{
			Rotate<TTreap>(root, i - 1, j, N);
		}
	}
	vector<int> vf = BSTTraversal::Traverse<TNode, int>(root, EBST_Inorder);
	cout << abs(vf[0] - vf.back()) << endl;
	WriteVector(vf);
	return 0;
}
