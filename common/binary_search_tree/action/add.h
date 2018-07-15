#pragma once

#include "../action.h"

template <class TData>
class BSTActionAdd : public BSTActionNone
{
public:
	using TBase = BSTActionNone;
	using TSelf = BSTActionAdd<TData>;
	static const bool is_none = false;

	TData add_value;

	BSTActionAdd() : add_value(0) {}
	bool IsEmpty() const { return add_value == 0; }
	void Clear() { add_value = 0; };

	template<class TNode>
	void Add(const TSelf& new_action, TNode* node)
	{
		node->info.sum += node->info.size * new_action.add_value;
		add_value += new_action.add_value;
	}

	template<class TNode>
	void Apply(TNode* node)
	{
		if (IsEmpty()) return;
		node->data += add_value;
		if (node->l) node->l->AddAction(*this);
		if (node->r) node->r->AddAction(*this);
		add_value = 0;
	}
};
