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

protected:
	template<class TNode>
	size_t TreeHash(TNode* root, size_t h)
	{
		if (!root) return h;
		h = TreeHash(root->l, h);
		h = hash_combine(h, root->key);
		return TreeHash(root->r, h);
	}

	template<class TNode>
	void VerifyParentLinksI(TNode* root, TFakeFalse) {}

	template<class TNode>
	void VerifyParentLinksI(TNode* root, TFakeTrue f)
	{
		if (root->l)
		{
			VerifyParentLinksI(root->l, f);
			if (root->l->p != root)
			{
				cout << "Parent link is incorrect!" << endl;
				assert(false);
			}
		}
		if (root->r)
		{
			VerifyParentLinksI(root->r, f);
			if (root->r->p != root)
			{
				cout << "Parent link is incorrect!" << endl;
				assert(false);
			}
		}
	}

	template<class TNode>
	void VerifyParentLinks(TNode* root) { if (mode == hash_test) VerifyParentLinksI(root, TFakeBool<TNode::use_parent>()); }

	template<class TNode>
	void VerifyParentLinksLazy(TNode* root)
	{
		if (root && !(root->info.size & (root->info.size - 1)))
			VerifyParentLinks(root);
	}

	template<class TTree>
	typename TTree::TNode* TestBuild(TTree& tree, TBSTKeysType type)
	{
		Timer t;
		typename TTree::TNode* root = tree.Build(GetKeys(type), GetKeys(type));
		t.Stop();
		AddResult("Build", type, TreeHash(root, 0), t.GetMilliseconds());
		VerifyParentLinks(root);
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestInsert(TTree& tree, TBSTKeysType type)
	{
		const vector<TKey>& vkeys = GetKeys(type);
		Timer t;
		typename TTree::TNode* root = 0;
		size_t h = 0;
		for (TKey key : vkeys)
		{
			AddAction(root);
			root = tree.InsertNewNode(root, key, key);
			VerifyParentLinksLazy(root);
			h = hash_combine(h, GetInfoValue(root));
		}
		AddResult("Insert", type, h, t.GetMilliseconds());
		VerifyParentLinks(root);
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestFindByOrder(TTree& tree, typename TTree::TNode* root, TBSTKeysType type)
	{
		Timer t;
		size_t h = 0;
		for (unsigned i = 0; i < Size(); ++i)
		{
			typename TTree::TNode* node = tree.FindByOrder(root, i);
			assert(node);
			h = hash_combine(h, node->key);
		}
		AddResult("FindO", type, h, t.GetMilliseconds());
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestFindByKey0(TTree& tree, typename TTree::TNode* root, TBSTKeysType type)
	{
		Timer t;
		size_t h = 0;
		for (unsigned i = 0; i <= Size(); ++i)
		{
			typename TTree::TNode* node = tree.FindByKey(root, 2 * i);
			assert(!node);
			h = hash_combine(h, reinterpret_cast<size_t>(node));
		}
		AddResult("FindK0", type, h, t.GetMilliseconds());
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestFindByKey1(TTree& tree, typename TTree::TNode* root, TBSTKeysType type)
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
		AddResult("FindK1", type, h, t.GetMilliseconds());
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestDeleteByKey(TTree& tree, typename TTree::TNode* root, TBSTKeysType type)
	{
		const vector<TKey>& vkeys = GetKeys(type);
		Timer t;
		size_t h = 0;
		for (const TKey& key : vkeys)
		{
			AddAction(root);
			root = tree.RemoveAndReleaseByKey(root, key);
			VerifyParentLinksLazy(root);
			h = hash_combine(h, (type <= shuffled) ? GetInfoValue(root) : root ? root->info.size : 0);
		}
		AddResult("DelKey", type, h, t.GetMilliseconds());
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestDeleteByNodeI(TTree& tree, typename TTree::TNode* root, TBSTKeysType type, TFakeFalse) { tree.ReleaseTree(root); return 0; }

	template<class TTree>
	typename TTree::TNode* TestDeleteByNodeI(TTree& tree, typename TTree::TNode* root, TBSTKeysType type, TFakeTrue)
	{
		Timer t;
		size_t h = 0;
		for (unsigned i = 0; i < Size(); ++i)
		{
			AddAction(root);
			root = tree.RemoveAndReleaseByNode(tree.GetNodeByRawIndex(i));
			VerifyParentLinksLazy(root);
			h = hash_combine(h, GetInfoValue(root));
		}
		AddResult("DelNode", type, h, t.GetMilliseconds());
		return root;
	}

	template<class TTree>
	typename TTree::TNode* TestDeleteByNode(TTree& tree, typename TTree::TNode* root, TBSTKeysType type) { return TestDeleteByNodeI(tree, root, type, TFakeBool<TTree::use_parent>()); }

	template<class TTree>
	void TestAll(const string& tree_name)
	{
		cout << "\tTesting " << tree_name << ":" << endl;
		current_tree = tree_name;
		typename TTree::TNode* root = 0;
		TTree tree(Size());
		for (unsigned type = 0; type < type_end; ++type)
		{
			TBSTKeysType ktype = TBSTKeysType(type);
			root = TestBuild<TTree>(tree, ktype);
			root = TestDeleteByNode(tree, root, ktype);
			// tree.ReleaseTree(root); root = 0;
			assert(!root);
			root = TestInsert<TTree>(tree, ktype);
			root = TestFindByOrder<TTree>(tree, root, ktype);
			root = TestFindByKey0<TTree>(tree, root, ktype);
			root = TestFindByKey1<TTree>(tree, root, ktype);
			root = TestDeleteByKey<TTree>(tree, root, ktype);
			// tree.ReleaseTree(root); root = 0;
			assert(!root);
			tree.ResetNodes();
		}
	}

public:
	bool TestAllTrees();
};
