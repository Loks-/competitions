// https://www.hackerrank.com/challenges/palindromic-subsets

#include "common/binary_search_tree/action/vector_rotate.h"
#include "common/binary_search_tree/info/sum.h"
#include "common/binary_search_tree/treap.h"
#include "common/binary_search_tree/utils/add_action_to_segment.h"
#include "common/binary_search_tree/utils/get_segment_info.h"
#include "common/linear_algebra/vector_static_size.h"
#include "common/numeric/modular.h"
#include "common/stl/base.h"

#include <string>

using TVector = VectorStaticSize<unsigned, 26>;
using TTree = Treap<false, false, TVector, BSTInfoSum<TVector, BSTInfoSize>, BSTActionVectorRotate>;
using TNode = TTree::TNode;
using TModular = Modular<>;

int main_palindromic_subsets__treap()
{
	unsigned n, q;
	cin >> n >> q;
	string s;
	cin >> s;
	vector<TVector> vs(n);
	for (unsigned i = 0; i < n; ++i)
		vs[i](s[i] - 'a') = 1;
	TTree tree(n);
	TNode* root = tree.Build(vs);
	 for (unsigned i = 0; i < q; ++i)
	 {
	 	unsigned t, a, b, c;
	 	cin >> t;
	 	if (t == 1)
	 	{
	 		cin >> a >> b >> c;
			root = BSTAddActionToSegment<TTree>(root, a, b + 1, c);
	 	}
	 	else if (t == 2)
	 	{
	 		cin >> a >> b;
	 		TTree::TInfo info;
			root = BSTGetSegmentInfo<TTree>(root, a, b + 1, info);
	 		unsigned nsum = 0, nnonzero = 0;
	 		for (unsigned j = 0; j < 26; ++j)
	 		{
	 			if (info.sum(j))
	 			{
	 				nsum += info.sum(j);
	 				nnonzero += 1;
	 			}
	 		}
	 		TModular two = 2;
	 		cout << (TModular(nnonzero + 1) * two.PowU(nsum - nnonzero) - 1).Get() << endl;
	 	}
	 }
	return 0;
}