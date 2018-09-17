// https://www.hackerrank.com/challenges/palindromic-subsets

#include "common/binary_search_tree/treap.h"
#include "common/stl/base.h"

#include <string>

using TTree = Treap<false, false, unsigned>; //, CBTreeDataInfoSet, CBTreeDataActionRotate>;
using TNode = TTree::TNode;

int main_palindromic_subsets__treap()
{
	unsigned n, q;
	cin >> n >> q;
	string s;
	cin >> s;
	assert(s.size() == n);
	vector<unsigned> vs(n);
	for (unsigned i = 0; i < n; ++i)
	{
		assert(('a' <= s[i]) && (s[i] <= 'z'));
		vs[i] = (s[i] - 'a');
	}
	// TTreap treap(n);
	// TNode * root = treap.Build(), *pl, *pt, *pm, *pr;
	// for (unsigned i = 0; i < q; ++i)
	// {
	// 	unsigned t, a, b, c;
	// 	cin >> t;
	// 	if (t == 1)
	// 	{
	// 		cin >> a >> b >> c;
	// 		treap.SplitBySize(root, b + 1, pt, pr);
	// 		treap.SplitBySize(pt, a, pl, pm);
	// 		pm->action.Set(c);
	// 		pt = treap.Merge(pl, pm);
	// 		root = treap.Merge(pt, pr);
	// 	}
	// 	else if (t == 2)
	// 	{
	// 		cin >> a >> b;
	// 		TTreap::TInfo tinfo;
	// 		root = treap.GetInfo(root, a, b, tinfo);
	// 		unsigned nsum = 0, nnonzero = 0;
	// 		for (unsigned j = 0; j < 26; ++j)
	// 		{
	// 			if (tinfo.vcnt[j])
	// 			{
	// 				nsum += tinfo.vcnt[j];
	// 				nnonzero += 1;
	// 			}
	// 		}
	// 		assert(nsum == b - a + 1);
	// 		TModular two = 2;
	// 		cout << (TModular(nnonzero + 1) * two.PowU(nsum - nnonzero) - 1).Get() << endl;
	// 	}
	// 	else
	// 		assert(false);
	// }
	return 0;
}
