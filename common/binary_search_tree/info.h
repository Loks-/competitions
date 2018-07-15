#pragma once

#include "../base.h"
#include "../template.h"

class BSTInfoNone
{
public:
	using TSelf = BSTInfoNone;
	static const bool is_none = true;
	static const bool use_data = false;
	static const bool has_size = false;

	template<class TNode>
	void Update(const TNode* node) {}
};

class BSTInfoSize : public BSTInfoNone
{
public:
	using TSelf = BSTInfoSize;
	static const bool is_none = false;
	static const bool has_size = true;

	unsigned size = 0;

	template<class TNode>
	void Update(TNode* node)
	{
		node->ApplyAction();
		size = 1 + (node->l ? node->l->info.size : 0) + (node->r ? node->r->info.size : 0);
	}
};

namespace {

template<class TNode>
void UpdateInfoNodeToRoot(TNode* node, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRoot(TNode * node, TFakeTrue)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	for (; node; node = node->p) node->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRoot(TNode* node) { UpdateInfoNodeToRoot(node, TFakeBool<!TNode::TInfo::is_none>()); }

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node, TFakeFalse) {}

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node, TFakeTrue)
{
	for (auto it = path_root_to_node.rbegin(); it != path_root_to_node.rend(); ++it)
		(*it)->UpdateInfo();
}

template<class TNode>
void UpdateInfoNodeToRoot(const vector<TNode*>& path_root_to_node) { UpdateInfoNodeToRoot(path_root_to_node, TFakeBool<!TNode::TInfo::is_none>()); }

} // namespace
