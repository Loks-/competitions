#include "common/timer.h"
#include "common/binary_search_tree/avl_tree.h"
#include "common/binary_search_tree/red_black_tree.h"
#include "common/binary_search_tree/scapegoat_tree.h"
#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"

#include "tester_binary_search_tree.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>

void TesterBinarySearchTree::AddResult(const std::string& task, unsigned keys_type, size_t h, size_t time)
{
	if (mode == time_test)
		std::cout << "\t\t" << task << "\t" << keys_type << "\t" << time << std::endl;
	results.push_back({ current_job, current_tree, task, keys_type, h, time });
}

void TesterBinarySearchTree::AddMax()
{
	std::unordered_map<std::string, Result> m;
	for (const Result& r : results)
	{
		std::string skey = r.job + "_" + r.tree + "_" + r.task;
		if (m.find(skey) == m.end())
		{
			m[skey] = r;
			m[skey].h = 0;
			m[skey].keys_type = type_end;
		}
		else
		{
			m[skey].time = std::max(m[skey].time, r.time);
		}
	}
	for (const auto& p : m)
	{
		results.push_back(p.second);
	}
}

bool TesterBinarySearchTree::TestHash() const
{
	std::unordered_map<std::string, size_t> m;
	for (const Result& r : results)
	{
		std::string key = r.job + "_" + r.task + "_" + std::to_string(r.keys_type);
		if (m[key] == 0)
			m[key] = r.h;
		if (m[key] != r.h)
		{
			std::cout << "ERROR: " << "unexpected hash found for:" << std::endl;
			std::cout << "\t" << r.job << "\t" << r.task << "\t" << r.tree << "\t" << r.keys_type << "\t" << r.h << std::endl;
			return false;
		}
	}
	return true;
}

void TesterBinarySearchTree::PrintTime() const
{
	std::vector<Result> vt = results;
	std::sort(vt.begin(), vt.end(), [](const Result& a, const Result & b) -> bool
		{
		if (a.job != b.job) return a.job < b.job;
		if (a.task != b.task) return a.task < b.task;
		if (a.keys_type != b.keys_type) return a.keys_type < b.keys_type;
		return a.tree < b.tree;
	}
	);
	std::cout << "Timers:" << std::endl;
	for (const Result& r : vt)
	{
		std::cout << r.job << "\t" << r.task << "\t" << r.keys_type << "\t" << r.tree << "\t" << r.time << std::endl;
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
	std::shuffle(keys_shuffled.begin(), keys_shuffled.end(), std::minstd_rand());
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
	std::cout << "Testing base trees:" << std::endl;
	current_job = "base";
	TestAll<AVLTree<false, TKey, BSTInfoSize, BSTActionNone, TKey>>("avl_upf");
	TestAll<AVLTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("avl_upt");
	TestAll<RedBlackTree<false, TKey, BSTInfoSize, BSTActionNone, TKey>>("rbtree_upf");
	TestAll<RedBlackTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("rbtree_upt");
	TestAll<ScapegoatTree<false, TKey, BSTInfoSize, BSTActionNone, TKey>>("scape_upf");
	TestAll<ScapegoatTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("scape_upt");
	TestAll<SplayTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("splay_upt");
	TestAll<Treap<true, false, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upt");
	std::cout << "Testing full trees:" << std::endl;
	current_job = "full";
	TestAll<AVLTree<false, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("avl_upf");
	TestAll<AVLTree<true, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("avl_upt");
	TestAll<RedBlackTree<false, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("rbtree_upf");
	TestAll<RedBlackTree<true, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("rbtree_upt");
	TestAll<ScapegoatTree<false, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("scape_upf");
	TestAll<ScapegoatTree<true, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("scape_upt");
	TestAll<SplayTree<true, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("splay_upt");
	TestAll<Treap<true, false, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSum<TKey, BSTInfoSize>, BSTActionAdd<TKey>, TKey>>("treap_upt");
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
		std::cout << "Total time = " << t.GetMilliseconds() << std::endl;
	}
	return result;
}
