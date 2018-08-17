#include "common/timer.h"
#include "common/binary_search_tree/red_black_tree_upf.h"
#include "common/binary_search_tree/red_black_tree_upt.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"

#include "tester_binary_search_tree.h"

void TesterBinarySearchTree::AddResult(const string& task, unsigned keys_type, size_t h, size_t time)
{
	if (mode == time_test)
		cout << "\t\t" << task << "\t" << keys_type << "\t" << time << endl;
	results.push_back({ current_job, current_tree, task, keys_type, h, time });
}

void TesterBinarySearchTree::AddMax()
{
	unordered_map<string, Result> m;
	for (const Result& r : results)
	{
		string skey = r.job + "_" + r.tree + "_" + r.task;
		if (m.find(skey) == m.end())
		{
			m[skey] = r;
			m[skey].h = 0;
			m[skey].keys_type = type_end;
		}
		else
		{
			m[skey].time = max(m[skey].time, r.time);
		}
	}
	for (const auto& p : m)
	{
		results.push_back(p.second);
	}
}

bool TesterBinarySearchTree::TestHash() const
{
	unordered_map<string, size_t> m;
	for (const Result& r : results)
	{
		string key = r.job + "_" + r.task + "_" + to_string(r.keys_type);
		if (m[key] == 0)
			m[key] = r.h;
		if (m[key] != r.h)
		{
			cout << "ERROR: " << "unexpected hash found for:" << endl;
			cout << "\t" << r.job << "\t" << r.task << "\t" << r.tree << "\t" << r.keys_type << "\t" << r.h << endl;
			return false;
		}
	}
	return true;
}

void TesterBinarySearchTree::PrintTime() const
{
	vector<Result> vt = results;
	sort(vt.begin(), vt.end(), [](const Result& a, const Result & b) -> bool
		{
		if (a.job != b.job) return a.job < b.job;
		if (a.task != b.task) return a.task < b.task;
		if (a.keys_type != b.keys_type) return a.keys_type < b.keys_type;
		return a.tree < b.tree;
	}
	);
	cout << "Timers:" << endl;
	for (const Result& r : vt)
	{
		cout << r.job << "\t" << r.task << "\t" << r.keys_type << "\t" << r.tree << "\t" << r.time << endl;
	}
}

TesterBinarySearchTree::TesterBinarySearchTree(unsigned size, TBSTMode _mode) : mode(_mode)
{
	keys.resize(type_end);
	keys_sorted.resize(size);
	for (unsigned i = 0; i < size; ++i)
		keys_sorted[i] = 2 * i + 1;
	keys[sorted] = &keys_sorted;
	keys_reversed = keys_sorted;
	reverse(keys_reversed.begin(), keys_reversed.end());
	keys[reversed] = &keys_reversed;
	keys_shuffled = keys_sorted;
	shuffle(keys_shuffled.begin(), keys_shuffled.end(), minstd_rand());
	keys[shuffled] = &keys_shuffled;
	keys_shuffled_dups = keys_shuffled;
	for (TKey& key : keys_shuffled_dups)
		key &= ~2;
	keys[shuffled_dups] = &keys_shuffled_dups;
	keys_one_value.resize(0);
	keys_one_value.resize(size, size | 1);
	keys[one_value] = &keys_one_value;
	keys_sixteen_values = keys_shuffled;
	for (TKey& key : keys_sixteen_values)
		key &= 31;
	keys[sixteen_values] = &keys_sixteen_values;
}

bool TesterBinarySearchTree::TestAllTrees()
{
	cout << "Testing base trees:" << endl;
	current_job = "base";
	TestAll<RedBlackTree<false, TKey, BSTInfoSize, BSTActionNone, TKey>>("rbtree_upf");
	TestAll<RedBlackTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("rbtree_upt");
	TestAll<ScapegoatTree<false, TKey, BSTInfoSize, BSTActionNone, TKey>>("scape_upf");
	TestAll<ScapegoatTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("scape_upt");
	TestAll<SplayTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("splay_upt");
	TestAll<Treap<true, false, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upt");
	cout << "Testing full trees:" << endl;
	current_job = "full";
	TestAll<RedBlackTree<false, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("rbtree_upf");
	TestAll<RedBlackTree<true, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("rbtree_upt");
	TestAll<ScapegoatTree<false, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("scape_upf");
	TestAll<ScapegoatTree<true, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("scape_upt");
	TestAll<SplayTree<true, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("splay_upt");
	TestAll<Treap<true, false, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSum<TKey>, BSTActionAdd<TKey>, TKey>>("treap_upt");
	bool output = TestHash();
	if ((mode == time_test) && output)
	{
		AddMax();
		PrintTime();
	}
	return output;
}

bool TestBinarySearchTree(bool run_time_test)
{
	TesterBinarySearchTree tbst_hash(10000, TesterBinarySearchTree::hash_test);
	bool result = tbst_hash.TestAllTrees();
	if (result && run_time_test)
	{
		Timer t;
		TesterBinarySearchTree tbst_time(10000000, TesterBinarySearchTree::time_test);
		result = result && tbst_time.TestAllTrees();
		cout << "Total time = " << t.GetMilliseconds() << endl;
	}
	return result;
}
