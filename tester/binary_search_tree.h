#pragma once

#include "common/hash.h"
#include "common/timer.h"

#include "binary_search_tree_action.h"
#include "binary_search_tree_info.h"

class TesterBinarySearchTree
{
public:
	using TKey = uint64_t;

	enum TBSTKeysType
	{
		sorted = 0,
		reversed = 1,
		shuffled = 2,
		shuffled_dups = 3,
		one_value = 4,
		sixteen_values = 5,
		type_end = 6
	};

	enum TBSTMode
	{
		hash_test,
		time_test
	};

protected:
	TBSTMode mode;
	vector<TKey> keys_sorted, keys_reversed, keys_shuffled, keys_shuffled_dups, keys_one_value, keys_sixteen_values;
	vector<vector<TKey>*> keys;

	const vector<TKey>& GetKeys(TBSTKeysType type) const
	{
		assert(type < type_end);
		return *keys[type];
	}

	unsigned Size() const { return unsigned(keys_sorted.size()); }

	class Result
	{
	public:
		string job;
		string tree;
		string task;
		unsigned keys_type;
		size_t h;
		size_t time;
	};

	string current_job, current_tree;
	vector<Result> results;

	void AddResult(const string& task, unsigned keys_type, size_t h, size_t time);
	void AddMax();
	bool TestHash() const;
	void PrintTime() const;

public:
	TesterBinarySearchTree(unsigned size, TBSTMode _mode);

	template<class TTree>
	void TestBuild(TBSTKeysType type)
	{
		Timer t;
		TTree tree(Size());
		tree.Build(GetKeys(type), GetKeys(type));
		t.Stop();
		AddResult("Build", type, 0, t.GetMilliseconds());
	}

	template<class TTree>
	void TestBuildAll()
	{
		for (unsigned type = 0; type < type_end; ++type)
			TestBuild<TTree>(TBSTKeysType(type));
	}

	template<class TTree>
	void TestFindByOrder(TBSTKeysType type, TTree& tree, typename TTree::TNode*& root)
	{
		Timer t;
		size_t h = 0;
		for (unsigned i = 0; i < Size(); ++i)
		{
			typename TTree::TNode* node = tree.FindByOrder(root, i);
			assert(node);
			h = hash_combine(h, node->key);
		}
		t.Stop();
		AddResult("FindO", type, h, t.GetMilliseconds());
	}

	template<class TTree>
	void TestFindByKey0(TBSTKeysType type, TTree& tree, typename TTree::TNode*& root)
	{
		Timer t;
		size_t h = 0;
		for (unsigned i = 0; i <= Size(); ++i)
		{
			typename TTree::TNode* node = tree.FindByKey(root, 2 * i);
			assert(!node);
			h = hash_combine(h, reinterpret_cast<size_t>(node));
		}
		t.Stop();
		AddResult("FindK0", type, h, t.GetMilliseconds());
	}

	template<class TTree>
	void TestFindByKey1(TBSTKeysType type, TTree& tree, typename TTree::TNode*& root)
	{
		const vector<TKey>& vkeys = GetKeys(type);
		Timer t;
		size_t h = 0;
		for (const TKey& key : vkeys)
		{
			typename TTree::TNode* node = tree.FindByKey(root, key);
			assert(node);
			h = hash_combine(h, (type <= shuffled) ? node->data : node->key);
		}
		t.Stop();
		AddResult("FindK1", type, h, t.GetMilliseconds());
	}

	template<class TTree>
	void TestInsert(TBSTKeysType type)
	{
		const vector<TKey>& vkeys = GetKeys(type);
		Timer t;
		TTree tree(Size());
		typename TTree::TNode* root = 0;
		size_t h = 0;
		for (TKey key : vkeys)
		{
			AddAction(root);
			root = tree.InsertNewNode(root, key, key);
			h = hash_combine(h, GetInfoValue(root));
		}
		t.Stop();
		AddResult("Insert", type, h, t.GetMilliseconds());
		TestFindByOrder<TTree>(type, tree, root);
		TestFindByKey0<TTree>(type, tree, root);
		TestFindByKey1<TTree>(type, tree, root);
	}

	template<class TTree>
	void TestInsertAll()
	{
		for (unsigned type = 0; type < type_end; ++type)
			TestInsert<TTree>(TBSTKeysType(type));
	}

	template<class TTree>
	void TestAll(const string& tree_name)
	{
		cout << "\tTesting " << tree_name << ":" << endl;
		current_tree = tree_name;
		TestBuildAll<TTree>();
		TestInsertAll<TTree>();
	}

	bool TestAllTrees();

	static bool Test(bool run_time_test = false);
};
