#pragma once

#include "action.h"
#include "info.h"
#include "insert_by_key.h"
#include "node.h"
#include "rotate.h"
#include "sibling.h"
#include "swap.h"
#include "tree.h"
#include "update_info_with_path.h"

template<class TInfo>
class RBTInfo : public TInfo
{
public:
	bool is_black = false;
};

template <
	bool _use_parent,
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class RedBlackTree: public BSTree<BSTNode<TTData, RBTInfo<TTInfo>, TTAction, true, _use_parent, false, TTKey>,
	                              RedBlackTree<_use_parent, TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = true;
	static const bool use_parent = _use_parent;
	static const bool use_height = false;

	using TData = TTData;
	using TInfo = RBTInfo<TTInfo>;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = RedBlackTree<use_parent, TData, TTInfo, TAction, TKey>;
	using TTree = BSTree<TNode, TSelf>;
	friend class BSTree<TNode, TSelf>;

public:
	RedBlackTree(unsigned max_nodes) : TTree(max_nodes) {}

protected:
	static void BuildTreeIFixColorsR(TNode* root, unsigned height)
	{
		assert(root || !height);
		if (!root) return;
		if (height)
		{
			root->info.is_black = true;
			BuildTreeIFixColorsR(root->l, height - 1);
			BuildTreeIFixColorsR(root->r, height - 1);
		}
		else
		{
			root->info.is_black = false;
			assert(!root->l && !root->r);
		}
	}

public:
	static TNode* BuildTree(const vector<TNode*>& nodes)
	{
		if (nodes.size() == 0) return 0;
		unsigned h = 0;
		for (; unsigned(nodes.size()) >= (1u << h); ) ++h;
		TNode* root = TTree::BuildTree(nodes);
		BuildTreeIFixColorsR(root, h - 1);
		root->info.is_black = true;
		return root;
	}

protected:
	static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeFalse)
	{
		static thread_local vector<TNode*> node_to_root_path;
		if (!root) { node->info.is_black = true; return node; }
		node_to_root_path.clear();
		for (TNode* current = root;;)
		{
			current->ApplyAction();
			node_to_root_path.push_back(current);
			if (current->key < node->key)
			{
				if (current->r)
					current = current->r;
				else
				{
					current->SetR(node);
					break;
				}
			}
			else
			{
				if (current->l)
					current = current->l;
				else
				{
					current->SetL(node);
					break;
				}
			}
		}
		node_to_root_path.push_back(node);
		reverse(node_to_root_path.begin(), node_to_root_path.end());
		UpdateInfoNodeToRootWithPath(node_to_root_path, 1);
		node_to_root_path.push_back(0);
		node->info.is_black = false;
		for (unsigned node_index = 1;;)
		{
			TNode* parent = node_to_root_path[node_index++];
			if (!parent) { node->info.is_black = true; return node; }
			if (parent->info.is_black) { return root; }
			TNode* gparent = node_to_root_path[node_index++];
			TNode* uncle = BSTSibling(parent, gparent);
			if (!uncle || uncle->info.is_black)
			{
				bool rotate_required = ((gparent->l == parent) != (parent->l == node));
				if (rotate_required)
				{
					BSTRotate<TNode, false>(node, parent, gparent);
					parent = node;
				}
				TNode* ggparent = node_to_root_path[node_index++];
				BSTRotate<TNode, true>(parent, gparent, ggparent);
				gparent->info.is_black = false;
				parent->info.is_black = true;
				return ggparent ? root : parent;
			}
			gparent->info.is_black = false;
			parent->info.is_black = true;
			uncle->info.is_black = true;
			node = gparent;
		}
		assert(false);
		return 0;
	}

	static TNode* InsertByKeyI(TNode* root, TNode* node, TFakeTrue)
	{
		BSTInsertByKey<TNode>(root, node);
		node->info.is_black = false;
		for (;;)
		{
			TNode* parent = node->p;
			if (!parent) { node->info.is_black = true; return node; }
			if (parent->info.is_black) return root;
			TNode* gparent = parent->p;
			TNode* uncle = BSTSibling(parent, gparent);
			if (!uncle || uncle->info.is_black)
			{
				bool rotate_required = ((gparent->l == parent) != (parent->l == node));
				if (rotate_required)
				{
					BSTRotateUp<TNode, false>(node);
					parent = node;
				}
				BSTRotateUp<TNode, true>(parent);
				gparent->info.is_black = false;
				parent->info.is_black = true;
				return parent->p ? root : parent;
			}
			parent->info.is_black = true;
			uncle->info.is_black = true;
			gparent->info.is_black = false;
			node = gparent;
		}
		assert(false);
		return 0;
	}

public:
	static TNode* InsertByKey(TNode* root, TNode* node) { assert(node);	return InsertByKeyI(root, node, TFakeBool<use_parent>()); }

protected:
	static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node, TFakeFalse)
	{
		static thread_local vector<TNode*> node_to_root_path;
		// Find node
		if (!root) return root;
		node_to_root_path.clear();
		TNode* node = root;
		for (; node;)
		{
			node->ApplyAction();
			node_to_root_path.push_back(node);
			if (node->key < key)
				node = node->r;
			else if (node->key > key)
				node = node->l;
			else
			{
				removed_node = node;
				break;
			}
		}
		if (!node) return root;

		// Optional swap
		if (node->l && node->r)
		{
			unsigned node_index = unsigned(node_to_root_path.size() - 1);
			TNode* temp = node->l;
			for (temp->ApplyAction(); temp->r; )
			{
				node_to_root_path.push_back(temp);
				temp = temp->r;
				temp->ApplyAction();
			}
			BSTSwapI(node, (node_index > 0) ? node_to_root_path[node_index - 1] : 0, temp, node_to_root_path.back());
			swap(node->info.is_black, temp->info.is_black);
			node_to_root_path[node_index] = temp;
			node_to_root_path.push_back(node);
			root = node_to_root_path[0];
		}

		// Drop node from tree
		reverse(node_to_root_path.begin(), node_to_root_path.end());
		TNode* child = node->l ? node->l : node->r;
		TNode* parent = node_to_root_path.size() > 1 ? node_to_root_path[1] : 0;
		if (parent)
		{
			if (parent->l == node)
				parent->SetL(child);
			else
				parent->SetR(child);
		}
		else if (child) 
			child->SetParentLink(0);
		node->ResetLinks();
		UpdateInfoNodeToRootWithPath(node_to_root_path, 1);

		// Fix colors
		node_to_root_path.push_back(0);
		unsigned current_index = 2;
		if (!node->info.is_black) return (parent ? root : child);
		for (;;)
		{
			if (child && !child->info.is_black)
			{
				child->info.is_black = true;
				return (parent ? root : child);
			}
			if (!parent) return child;
			TNode* sibling = BSTSibling(child, parent);
			assert(sibling);
			sibling->ApplyAction();
			TNode* gparent = node_to_root_path[current_index];
			if (!sibling->info.is_black)
			{
				assert(parent->info.is_black);
				BSTRotate<TNode, true>(sibling, parent, gparent);
				node_to_root_path[--current_index] = sibling;
				if (!gparent)
					root = sibling;
				gparent = sibling;
				sibling->info.is_black = true;
				parent->info.is_black = false;
				sibling = BSTSibling(child, parent);
				sibling->ApplyAction();
			}
			assert(sibling && sibling->info.is_black);
			if (parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				child = parent;
				parent = node_to_root_path[current_index++];
				continue;
			}
			if (!parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				parent->info.is_black = true;
				return root;
			}
			if ((parent->l == child) && (!sibling->r || sibling->r->info.is_black))
			{
				assert(sibling->l && !sibling->l->info.is_black);
				BSTRotate<TNode, false, true>(sibling->l, sibling, parent);
				sibling->info.is_black = false;
				sibling = parent->r;
				sibling->info.is_black = true;
			}
			else if ((parent->r == child) && (!sibling->l || sibling->l->info.is_black))
			{
				assert(sibling->r && !sibling->r->info.is_black);
				BSTRotate<TNode, false, true>(sibling->r, sibling, parent);
				sibling->info.is_black = false;
				sibling = parent->l;
				sibling->info.is_black = true;
			}
			sibling->info.is_black = parent->info.is_black;
			parent->info.is_black = true;
			if (parent->l == child)
				sibling->r->info.is_black = true;
			else
				sibling->l->info.is_black = true;
			BSTRotate<TNode, true>(sibling, parent, gparent);
			return gparent ? root : sibling;
		}
		assert(false);
		return 0;
	}

	static TNode* RemoveByKeyI(TNode* root, const TKey& key, TNode*& removed_node, TFakeTrue)
	{
		removed_node = TTree::FindByKey(root, key);
		return (removed_node ? RemoveByNode(removed_node) : root);
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
			swap(node->info.is_black, temp->info.is_black);
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

		// Fix colors
		if (!node->info.is_black) return (parent ? Root(parent) : child);
		for (;;)
		{
			if (child && !child->info.is_black)
			{
				child->info.is_black = true;
				return Root(child);
			}
			if (!parent) return child;
			TNode* sibling = BSTSibling(child, parent);
			assert(sibling);
			sibling->ApplyAction();
			if (!sibling->info.is_black)
			{
				assert(parent->info.is_black);
				BSTRotateUp<TNode, true>(sibling);
				sibling->info.is_black = true;
				parent->info.is_black = false;
				sibling = BSTSibling(child, parent);
				sibling->ApplyAction();
			}
			assert(sibling && sibling->info.is_black);
			if (parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				child = parent;
				parent = child->p;
				continue;
			}
			if (!parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				parent->info.is_black = true;
				return Root(parent);
			}
			if ((parent->l == child) && (!sibling->r || sibling->r->info.is_black))
			{
				assert(sibling->l && !sibling->l->info.is_black);
				BSTRotateUp<TNode, false, true>(sibling->l);
				sibling->info.is_black = false;
				sibling = sibling->p;
				sibling->info.is_black = true;
			}
			else if ((parent->r == child) && (!sibling->l || sibling->l->info.is_black))
			{
				assert(sibling->r && !sibling->r->info.is_black);
				BSTRotateUp<TNode, false, true>(sibling->r);
				sibling->info.is_black = false;
				sibling = sibling->p;
				sibling->info.is_black = true;
			}
			sibling->info.is_black = parent->info.is_black;
			parent->info.is_black = true;
			if (parent->l == child)
				sibling->r->info.is_black = true;
			else
				sibling->l->info.is_black = true;
			BSTRotateUp<TNode, true>(sibling);
			return Root(sibling);
		}
		assert(false);
		return 0;
	}
};
