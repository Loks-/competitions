#pragma once

#include "action.h"
#include "build.h"
#include "info.h"
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
	using TSelf = ScapegoatTree<use_parent, TData, TInfo, TAction, TKey>;
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

public:
	static TNode* RebuildSubtree(TNode* node)
	{
		assert(node);
		vector<TNode*> nodes;
		TraverseInorder(node, nodes);
		return BSTBuild(nodes);
	}

	static TNode* UpdateAndFixSubtree(TNode* node)
	{
		node->UpdateInfo();
		return ((node->l && node->l->info.size > alpha * node->info.size)
			|| (node->r && node->r->info.size > alpha * node->info.size)) ?
			RebuildSubtree(node) : node;
	}

	static TNode* InsertByKey(TNode* root, TNode* node)
	{
		static_assert(use_key, "use_key should be true");
		if (!root) return node;
		root->ApplyAction();
		if (root->key < node->key)
		{
			root->r = InsertByKey(root->r, node);
			root->r->SetParentLink(root);
		}
		else
		{
			root->l = InsertByKey(root->l, node);
			root->l->SetParentLink(root);
		}
		return UpdateAndFixSubtree(root);
	}

protected:
	static TNode* RemoveRootI(TNode* root)
	{
		if (!root->l) return root->r;
		if (!root->r) return root->l;
		stack<TNode*> s;
		TNode* l = root->l, *r = root->r, *new_root;
		if (l->info.size < r->info.size)
		{
			new_root = r;
			for (new_root->ApplyAction(); new_root->l; new_root->ApplyAction())
			{
				s.push(new_root);
				new_root = new_root->l;
			}
			TNode* current = new_root->r;
			for (; !s.empty(); s.pop())
			{
				s.top()->l = current;
				if (current) current->SetParentLink(s.top());
				current = s.top();
				current->UpdateInfo();
				current = UpdateAndFixSubtree(current);
			}
			new_root->l = l;
			new_root->r = current;
		}
		else
		{
			new_root = l;
			for (new_root->ApplyAction(); new_root->r; new_root->ApplyAction())
			{
				s.push(new_root);
				new_root = new_root->r;
			}
			TNode* current = new_root->l;
			for (; !s.empty(); s.pop())
			{
				s.top()->r = current;
				if (current) current->SetParentLink(s.top());
				current = s.top();
				current->UpdateInfo();
				current = UpdateAndFixSubtree(current);
			}
			new_root->l = current;
			new_root->r = r;
		}
		if (new_root->l) new_root->l->SetParentLink(new_root);
		if (new_root->r) new_root->r->SetParentLink(new_root);
		return UpdateAndFixSubtree(new_root);
	}

public:
	static TNode* RemoveByKey(TNode* root, const TKey& key, TNode*& removed_node)
	{
		static_assert(use_key, "use_key should be true");
		if (!root) return root;
		root->ApplyAction();
		if (root->key < key)
		{
			root->r = RemoveByKey(root->r, key, removed_node);
			if (root->r) root->r->SetParentLink(root);
		}
		else if (root->key > key)
		{
			root->l = RemoveByKey(root->l, key, removed_node);
			if (root->l) root->l->SetParentLink(root);
		}
		else
		{
			removed_node = root;
			TNode* new_root = RemoveRootI(root);
			root->ResetLinks();
			root->UpdateInfo();
			return new_root;
		}
		return UpdateAndFixSubtree(root);
	}

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
		for (TNode * node = parent; node; node = parent)
		{
			parent = node->p;
			child = UpdateAndFixSubtree(node);
			if (child != node)
			{
				child->UpdateParentLink(parent);
				if (parent)
				{
					if (parent->l == node)
						parent->l = child;
					else
						parent->r = child;
				}
			}
		}
		return child;
	}
};
