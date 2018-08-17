#pragma once

#include "action.h"
#include "info.h"

template<class TInfo>
class RBTInfo : public TInfo
{
public:
	bool is_black = false;
};

template <
	bool _use_parent,
	class TData,
	class TInfo = BSTInfoSize,
	class TAction = BSTActionNone,
	class TKey = int64_t>
class RedBlackTree;
