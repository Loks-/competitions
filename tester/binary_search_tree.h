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
		type_end = 3
	};

protected:
	vector<TKey> keys_sorted, keys_reversed, keys_shuffled;
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

	size_t AddResult(const string& task, unsigned keys_type, size_t h, size_t time);
	bool TestHash() const;
	void PrintTime() const;

public:
	TesterBinarySearchTree(unsigned size);

	template<class TTree>
	size_t TestBuild(TBSTKeysType type)
	{
		Timer t;
		TTree tree(Size());
		tree.Build(GetKeys(type), GetKeys(type));
		t.Stop();
		return AddResult("Build", type, 0, t.GetMilliseconds());
	}

	template<class TTree>
	void TestBuildAll()
	{
		size_t max_time = 0;
		for (unsigned type = 0; type < type_end; ++type)
			max_time = max(max_time, TestBuild<TTree>(TBSTKeysType(type)));
		AddResult("Build", type_end, 0, max_time);
	}

	template<class TTree>
	size_t TestInsert(TBSTKeysType type)
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
		return AddResult("Insert", type, h, t.GetMilliseconds());
	}

	template<class TTree>
	void TestInsertAll()
	{
		size_t max_time = 0;
		for (unsigned type = 0; type < type_end; ++type)
			max_time = max(max_time, TestInsert<TTree>(TBSTKeysType(type)));
		AddResult("Insert", type_end, 0, max_time);
	}

	template<class TTree>
	void TestAll(const string& tree_name)
	{
		cout << "Testing " << tree_name << ":" << endl;
		current_tree = tree_name;
		TestBuildAll<TTree>();
		TestInsertAll<TTree>();
	}

	bool TestAllTrees();

	static bool Test();
};
