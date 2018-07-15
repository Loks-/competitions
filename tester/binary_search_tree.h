#pragma once

#include "common/timer.h"

#include "binary_search_tree_action.h"
#include "binary_search_tree_info.h"

class TesterBinarySearchTree
{
public:
	using TKey = uint64_t;

	enum TBSTKeysOrder
	{
		sorted = 0,
		reversed = 1,
		shuffled = 2,
		order_end = 3
	};

protected:
	vector<TKey> keys_sorted, keys_reversed, keys_shuffled;
	vector<vector<TKey>*> keys;

	const vector<TKey>& GetKeys(TBSTKeysOrder order) const
	{
		assert(order < order_end);
		return *keys[order];
	}

	unsigned Size() const { return unsigned(keys_sorted.size()); }

public:
	TesterBinarySearchTree(unsigned size);

	template<class TTree>
	void TestBuild(TBSTKeysOrder order)
	{
		Timer t;
		TTree tree(Size());
		tree.Build(GetKeys(order), GetKeys(order));
		t.Stop();
		cout << "\tBuild\t" << order << "\t" << t.GetMilliseconds() << endl;
	}

	template<class TTree>
	void TestBuildAll()
	{
		for (unsigned order = 0; order < order_end; ++order)
			TestBuild<TTree>(TBSTKeysOrder(order));
	}

	template<class TTree>
	void TestAll(const string& tree_name)
	{
		cout << "Testing " << tree_name << ":" << endl;
		TestBuildAll<TTree>();
	}

	void TestAllTrees();

	static void Test();
};
