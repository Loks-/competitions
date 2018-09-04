// https://www.hackerrank.com/challenges/median

#include "common/binary_search_tree/fixed_tree.h"
#include "common/binary_search_tree/base/find_by_key.h"
#include "common/binary_search_tree/info/size.h"
#include "common/binary_search_tree/info/sum.h"
#include "common/binary_search_tree/info/update_info.h"
#include "common/binary_search_tree/utils/upper_bound.h"
#include "common/stl_base.h"

#include <iomanip>
#include <unordered_set>

using TTree = FixedTree<true, unsigned, BSTInfoSum<unsigned, BSTInfoSize>>;
using TNode = TTree::TNode;

int main_median_updates__fixed_tree()
{
	cout << setprecision(1) << fixed;
	unsigned N;
	cin >> N;
	vector<pair<char, int64_t>> input;
	unordered_set<int64_t> set_x;
	for (unsigned i = 0; i < N; ++i)
	{
		char c; int64_t x;
		cin >> c >> x;
		x *= 2;
		input.push_back({ c, x });
		set_x.insert(x);
	}
	vector<int64_t> vx(set_x.begin(), set_x.end());
	TTree tree(unsigned(vx.size()));
	TNode* root = tree.Build(vector<unsigned>(vx.size(), 0), vx);

	for (auto p : input)
	{
		TNode* node = BSTFindByKey(root, p.second);
		assert(node);
		if (p.first == 'a')
		{
			node->data += 1;
		}
		else if (p.first == 'r')
		{
			if (node->data == 0)
			{
				cout << "Wrong!" << endl;
				continue;
			}
			node->data -= 1;
		}
		UpdateInfoNodeToRoot(node);
		if (root->info.sum == 0)
		{
			cout << "Wrong!" << endl;
			continue;
		}
		unsigned size = root->info.sum;
		int64_t s = (BSTPrefixSumUpperBound(root, (size - 1) / 2)->key + BSTPrefixSumUpperBound(root, size / 2)->key) / 2;
		if (s & 1)
			cout << s / 2.0 << endl;
		else
			cout << s / 2 << endl;
	}
	return 0;
}
