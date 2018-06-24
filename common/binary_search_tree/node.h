#pragma once

#include "../base.h"

template <
	class TTData,
	class TTInfo,
	class TTAction>
class TBSTNodeBase
{
public:
	using TData = TTData;
	using TInfo = TTInfo;
	using TAction = TTAction;

	TData data;
	TInfo info;
	TAction action;
};

template<
	class TData,
	class TInfo,
	class TAction,
	bool _use_height,
	class TTHeight>
class TBSTNodeHeight {};

template<
	class TData,
	class TInfo,
	class TAction,
	class TTHeight>
class TBSTNodeHeight<TData, TInfo, TAction, false, TTHeight> : 
	public TBSTNodeBase<TData,  TInfo, TAction> 
{
public:
	static const bool use_height = false;
};

template<
	class TData,
	class TInfo,
	class TAction,
	class TTHeight>
class TBSTNodeHeight<TData, TInfo, TAction, true, TTHeight> : 
	public TBSTNodeBase<TData, TInfo, TAction>
{
public:
	static const bool use_height = false;
	using THeight = TTHeight;

	THeight height;
};

template<
	class TData,
	class TInfo,
	class TAction,
	bool _use_key,
	bool use_height,
	class TTKey,
	class THeight>
class TBSTNodeKey {};

template<
	class TData,
	class TInfo,
	class TAction,
	bool use_height,
	class TTKey,
	class THeight>
class TBSTNodeKey<TData, TInfo, TAction, false, use_height, TTKey, THeight> : 
	public TBSTNodeHeight<TData, TInfo, TAction, use_height, THeight>
{
public:
	static const bool use_key = false;
};

template<
	class TData,
	class TInfo,
	class TAction,
	bool use_height,
	class TTKey,
	class THeight>
class TBSTNodeKey<TData, TInfo, TAction, true, use_height, TTKey, THeight> : 
	public TBSTNodeHeight<TData, TInfo, TAction, use_height, THeight>
{
public:
	static const bool use_key = false;
	using TKey = TTKey;

	TKey key;
};

template<
	class TData,
	class TInfo,
	class TAction,
	bool use_key,
	bool _use_parent,
	bool use_height,
	class TKey,
	class THeight>
class TBSTNodeFinal {};

template<
	class TData,
	class TInfo,
	class TAction,
	bool use_key,
	bool use_height,
	class TKey,
	class THeight>
class TBSTNodeFinal<TData, TInfo, TAction, use_key, false, use_height, TKey, THeight> :
	public TBSTNodeKey<TData, TInfo, TAction, use_key, use_height, TKey, THeight>
{
public:
	static const bool use_parent = false;
	using TSelf = TBSTNodeFinal<TData, TInfo, TAction, use_key, false, use_height, TKey, THeight>;

	TSelf *l = 0, *r = 0;
};

template<
	class TData,
	class TInfo,
	class TAction,
	bool use_key,
	bool use_height,
	class TKey,
	class THeight>
class TBSTNodeFinal<TData, TInfo, TAction, use_key, true, use_height, TKey, THeight> :
	public TBSTNodeKey<TData, TInfo, TAction, use_key, use_height, TKey, THeight>
{
public:
	static const bool use_parent = true;
	using TSelf = TBSTNodeFinal<TData, TInfo, TAction, use_key, false, use_height, TKey, THeight>;

	TSelf *l = 0, *r = 0, *p = 0;
};

template <
	class TData,
	class TInfo,
	class TAction,
	bool use_key,
	bool use_parent = true,
	bool use_height = false,
	class TKey = int64_t,
	class THeight = unsigned>
using BSTNode = TBSTNodeFinal<TData, TInfo, TAction, use_key, use_parent, use_height, TKey, THeight>;
