#pragma once

class BSTActionNone
{
public:
	using TSelf = BSTActionNone;
	static const bool is_none = true;

	bool IsEmpty() const { return true; }
	void Set(const TSelf& new_action) {}
	template<class TNode>
	void Apply(const TNode* node) {}
};
