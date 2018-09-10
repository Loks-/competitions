#pragma once

#include "../base.h"

template<bool use_height, class THeight>
class TBSTNodeProxyHeight {};

template<class THeight> 
class TBSTNodeProxyHeight<false, THeight> {};

template<class THeight>
class TBSTNodeProxyHeight<true, THeight>
{
public:
	THeight height;
};

template<bool use_key, bool use_height, class TKey, class Height>
class TBSTNodeProxyKey {};

template<bool use_height, class TKey, class THeight>
class TBSTNodeProxyKey<false, use_height, TKey, THeight> : 
	public TBSTNodeProxyHeight<use_height, THeight> {};

template<bool use_height, class TKey, class THeight>
class TBSTNodeProxyKey<true, use_height, TKey, THeight> : 
	public TBSTNodeProxyHeight<use_height, THeight>
{
public:
	TKey key;
};

template<bool use_key, bool use_parent, bool use_height, class TKey, class THeight, class TSelf>
class TBSTNodeProxyParent {};

template<bool use_key, bool use_height, class TKey, class THeight, class TSelf>
class TBSTNodeProxyParent<use_key, false, use_height, TKey, THeight, TSelf> :
	public TBSTNodeProxyKey<use_key, use_height, TKey, THeight>
{
public:
	TSelf *l = 0, *r = 0;

	void SetL(TSelf* node) { l = node; }
	void SetR(TSelf* node) { r = node; }
	void SetParentLink(TSelf* node) {}
	void ResetLinks() { l = r = 0; }
};

template<bool use_key, bool use_height, class TKey, class THeight, class TSelf>
class TBSTNodeProxyParent<use_key, true, use_height, TKey, THeight, TSelf> :
	public TBSTNodeProxyKey<use_key, use_height, TKey, THeight>
{
public:
	TSelf *l = 0, *r = 0, *p = 0;

	void SetL(TSelf* node) { l = node; if (node) node->p = static_cast<TSelf*>(this); }
	void SetR(TSelf* node) { r = node; if (node) node->p = static_cast<TSelf*>(this); }
	void SetParentLink(TSelf* node) { p = node; }
	void ResetLinks() { l = r = p = 0; }
};

template<
	class TTData,
	class TTInfo,
	class TTAction,
	bool _use_key,
	bool _use_parent = true,
	bool _use_height = false,
	class TTKey = int64_t,
	class TTHeight = unsigned>
class BSTNode :
	public TBSTNodeProxyParent<_use_key, _use_parent, _use_height, TTKey, TTHeight,
		BSTNode<TTData, TTInfo, TTAction, _use_key, _use_parent, _use_height, TTKey, TTHeight>>
{
public:
	static const bool use_key = _use_key;
	static const bool use_parent = _use_parent;
	static const bool use_height = _use_height;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using THeight = TTHeight;
	using TSelf = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey, THeight>;
	using TProxyParent = TBSTNodeProxyParent<use_key, use_parent, use_height, TKey, THeight, TSelf>;

	TData data;
	TInfo info;
	TAction action;

	void ClearAction() { action.Clear(); }
	void UpdateInfo() { info.Update(this); }
	template <class TActionValue> void AddAction(const TActionValue& value) { action.Add(this, value); }
	void ApplyAction() { action.Apply(this); }
	void ResetLinksAndUpdateInfo() { TProxyParent::ResetLinks(); UpdateInfo(); }
};

template<class TNode>
inline TNode* Root(TNode* node)
{
	static_assert(TNode::use_parent, "use_parent should be true");
	if (!node) return node;
	for (; node->p; node = node->p);
	return node;
}
