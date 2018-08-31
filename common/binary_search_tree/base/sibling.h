#pragma once

template<class TNode>
inline TNode* BSTSibling(TNode* node, TNode* parent) { return parent ? (parent->l == node ? parent->r : parent->l) : 0; }
