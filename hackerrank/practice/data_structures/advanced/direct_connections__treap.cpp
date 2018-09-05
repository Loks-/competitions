// https://www.hackerrank.com/challenges/direct-connections

#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum_keys.h"
#include "common/binary_search_tree/treap.h"
#include "common/numeric/modular.h"
#include "common/vector/read.h"
#include "common/stl_base.h"

using TModular = Modular<>;
using TTree = Treap<true, false, TEmpty, BSTInfoSumKeys<uint64_t, BSTInfoSize>, BSTActionNone, uint64_t>;
using TNode = TTree::TNode;

int main_direct_connections__treap()
{
	unsigned T, N;
	cin >> T;
	TTree tree(200000);
	for (unsigned it = 0; it < T; ++it)
	{
		cin >> N;
		vector<uint64_t> vx = ReadVector<uint64_t>(N);
		vector<uint64_t> vp = ReadVector<uint64_t>(N);
		vector<pair<uint64_t, uint64_t>> vpx;
		for (unsigned i = 0; i < N; ++i)
			vpx.push_back({ vp[i], vx[i] });
		sort(vpx.begin(), vpx.end());
		TModular total = 0;
		TNode* root = 0, *l, *r;
		for (auto px : vpx)
		{
			tree.SplitByKey(root, px.second, l, r);
			TModular s = 0;
			if (l)
				s += TModular(l->info.size * px.second - l->info.sum_keys);
			if (r)
				s += TModular(r->info.sum_keys - r->info.size * px.second);
			total += s * TModular(px.first);
			TNode* m = tree.GetNewNode(TEmpty(), px.second);
			root = tree.Join(tree.Join(l, m), r);
		}
		tree.ReleaseTree(root);
		cout << total.Get() << endl;
	}
	return 0;
}
