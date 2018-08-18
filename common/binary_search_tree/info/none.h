#pragma once

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
