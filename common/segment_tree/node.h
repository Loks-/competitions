#pragma once

#include "../base.h"

template<bool use_key, class TKey, class TSelf>
class TSTNodeProxyKey {};

template<class TKey, class TSelf>
class TSTNodeProxyKey<false, TKey, TSelf>
{
public:
	void CopyKey(const TSelf* p) {}
};

template<class TKey, class TSelf>
class TSTNodeProxyKey<true, TKey, TSelf>
{
public:
	TKey key;
	void CopyKey(const TSelf* p) { assert(p); key = p->key; }
};

template<bool use_key, bool use_parent, class TKey, class TSelf>
class TSTNodeProxyParent {};

template<bool use_key, class TKey, class TSelf>
class TSTNodeProxyParent<use_key, false, TKey, TSelf> :
	public TSTNodeProxyKey<use_key, TKey>
{
public:
	TSelf *l = 0, *r = 0;

	void SetL(TSelf* node) { l = node; }
	void SetR(TSelf* node) { r = node; }
	void SetP(TSelf* node) {}
	void ResetLinks() { l = r = 0; }
};

template<bool use_key, class TKey, class TSelf>
class TSTNodeProxyParent<use_key, true, TKey, TSelf> :
	public TSTNodeProxyKey<use_key, TKey>
{
public:
	TSelf *l = 0, *r = 0, *p = 0;

	void SetL(TSelf* node) { l = node; if (node) node->p = static_cast<TSelf*>(this); }
	void SetR(TSelf* node) { r = node; if (node) node->p = static_cast<TSelf*>(this); }
	void SetP(TSelf* node) { p = node; }
	void ResetLinks() { l = r = p = 0; }
};

template<
	class TTData,
	class TTInfo,
	class TTAction,
	bool _use_key = false,
	bool _use_parent = true,
	class TTKey = int64_t>
class STNode :
	public TSTNodeProxyParent<_use_key, _use_parent, TTKey,
		STNode<TTData, TTInfo, TTAction, _use_key, _use_parent, TTKey>>
{
public:
	static const bool use_key = _use_key;
	static const bool use_parent = _use_parent;

	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;
	using TKey = TTKey;
	using TSelf = BSTNode<TData, TInfo, TAction, use_key, use_parent, TKey>;
	using TProxyParent = TBSTNodeProxyParent<use_key, use_parent, TKey, TSelf>;

	TData * data = 0;
	TInfo info;
	TAction action;

	bool IsLeaf() const { return data; } 
    void ClearAction() { action.Clear(); }
	void UpdateInfo() { info.Update(this); }
	template <class TActionValue> void AddAction(const TActionValue& value) { action.Add(this, value); }
	void ApplyAction() { action.Apply(this); }
};
