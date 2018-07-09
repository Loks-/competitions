#pragma once

#include "action.h"
#include "build.h"
#include "info.h"
#include "insert.h"
#include "node.h"
#include "swap.h"
#include "tree.h"

// In this implementation delete operation is different from wiki Scapegoat
// tree. It removes node from tree (similar to other trees), not just mark for
// future deletion.
template <
	bool _use_parent,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class ScapegoatTree : public BSTree<BSTNode<TTData, TTInfo, TTAction, true, _use_parent, false, TTKey>,
									ScapegoatTree<_use_parent, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = true;
	static const bool use_parent = _use_parent;
	static const bool use_height = false;
	static constexpr double alpha = 0.7; // Height is less or equal to 2 * height(fully balanced tree).

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = ScapegoatTree<use_key, TData, TInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;
	friend class BSTree<TNode, TSelf>;

public:
	ScapegoatTree(unsigned max_nodes) : TTree(max_nodes) {}

protected:
	static void TraverseInorder(TNode* node, vector<TNode*>& output)
	{
		if (!node) return;
		node->ApplyAction();
		TraverseInorder(node->l, output);
		output.push_back(node);
		TraverseInorder(node->r, output);
	}

	static TNode* Fix(TNode* node, TNode* parent)
	{
		vector<TNode*> nodes;
		TraverseInorder(node, nodes);
		TNode* new_subtree = BSTBuild::Build(nodes);
		assert(new_subtree);
		new_subtree->SetParentLink(parent);
		if (parent)
		{
			if (parent->l == node)
				parent->l = new_subtree;
			else
				parent->r = new_subtree;
		}
		return new_subtree;
	}

	static TNode* CheckAndFix(TNode* node)
	{
		if (!node) return 0;
		TNode* rebuild_node = 0, * last_node = 0;
		for (; node; node = node->p)
		{
			last_node = node;
			if (node->l && node->l->info.size > alpha * node->info.size) rebuild_node = node;
			if (node->r && node->r->info.size > alpha * node->info.size) rebuild_node = node;
		}
		return rebuild_node ? Root(Fix(rebuild_node, rebuild_node->p)) : last_node;
	}

	// static TNode* InsertI(TNode* root, TNode* node, TFakeFalse) { static_assert(false); return 0; }
	static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeTrue)
	{ 
		BSTInsert<TNode>(root, node);
		return CheckAndFix(node);
	}

public:
	static TNode* RemoveByNode(TNode* node)
	{
		static_assert(use_parent, "use_parent should be true");
		assert(node);
		ApplyActionRootToNode(node);

		// Optional swap
		if (node->l && node->r)
		{
			TNode* temp = node->l;
			for (temp->ApplyAction(); temp->r; )
			{
				temp = temp->r;
				temp->ApplyAction();
			}
			BSTSwap<TNode, false>(node, temp);
		}

		// Drop node from tree
		TNode* child = node->l ? node->l : node->r;
		TNode* parent = node->p;
		if (parent)
		{
			if (parent->l == node)
				parent->l = child;
			else
				parent->r = child;
		}
		if (child) child->p = parent;
		node->ResetLinks();
		UpdateInfoNodeToRoot(parent);
		return parent ? CheckAndFix(parent) : child;
	}
};
