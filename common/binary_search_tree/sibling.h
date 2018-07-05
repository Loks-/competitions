#pragma once

namespace {

template<class TNode>
TNode* BSTSibling(TNode* node, TNode* parent)
{
	return reinterpret_cast<TNode*>(reinterpret_cast<size_t>(node) ^ reinterpret_cast<size_t>(parent->l) ^ reinterpret_cast<size_t>(parent->r));
}

} // namespace
