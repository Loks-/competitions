#pragma once

namespace {

template<class TNode>
TNode* BSTSibling(TNode* node, TNode* parent) {	return parent ? (parent->l - node + parent->r) : 0; }

} // namespace
