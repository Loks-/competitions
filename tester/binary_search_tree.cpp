#pragma once

#include "common/binary_search_tree/splay_tree.h"
#include "common/binary_search_tree/treap.h"

#include "binary_search_tree.h"

size_t TesterBinarySearchTree::AddResult(const string& task, unsigned keys_type, size_t h, size_t time)
{
	printf("%zx", h);
	cout << "\t" << task << "\t" << keys_type << "\t" << time << endl;
	results.push_back({ current_job, current_tree, task, keys_type, h, time });
	return time;
}

bool TesterBinarySearchTree::TestHash() const
{
	unordered_map<string, size_t> m;
	for (const Result& r : results)
	{
		if (r.h == 0) continue;
		string key = r.job + "_" + r.task;
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

TesterBinarySearchTree::TesterBinarySearchTree(unsigned size)
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
	random_shuffle(keys_shuffled.begin(), keys_shuffled.end());
	keys[shuffled] = &keys_shuffled;
}

bool TesterBinarySearchTree::TestAllTrees()
{
	cout << "Testing base trees:" << endl;
	current_job = "base";
	TestAll<SplayTree<true, TKey, BSTInfoSize, BSTActionNone, TKey>>("splay_upt");
	TestAll<Treap<true, false, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upf");
	TestAll<Treap<true, true, TKey, BSTInfoSize, BSTActionNone, TKey>>("treap_upt");
	bool output = TestHash();
	if (output) PrintTime();
	return output;
}

bool TesterBinarySearchTree::Test()
{
	TesterBinarySearchTree tbst(10000000);
	return tbst.TestAllTrees();
}
