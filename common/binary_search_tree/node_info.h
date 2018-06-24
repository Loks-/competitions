#pragma once

class BSTInfoNone
{
public:
	using TSelf = BSTInfoNone;
	static const bool has_size = false;

	template<class TNode>
	void Update(const TNode* node) {}
};

class BSTInfoSize : public BSTInfoNone
{
public:
	using TSelf = BSTInfoSize;
	static const bool has_size = true;

	unsigned size = 0;

	template<class TNode>
	void Update(const TNode* node)
	{
		static_assert(TNode::TInfo == TSelf);
		size = 1 + (node->l ? node->l->info.size : 0) + (node->r ? node->r->info.size : 0);
	}
};
