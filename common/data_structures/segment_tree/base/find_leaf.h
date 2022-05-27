#pragma once

namespace ds {
namespace st {
template <class TNode>
inline TNode* FindLeaf(TNode* root, const typename TNode::TCoordinate& x) {
  static_assert(TNode::TSInfo::has_coordinate, "has_coordinate should be true");
  for (TNode* node = root; node != nullptr;) {
    node->ApplyAction();
    if (node->IsLeaf()) {
      if ((node->sinfo.left == x) && (node->sinfo.right == x))
        return node;
      else
        return nullptr;
    } else if (node->r->sinfo.left <= x) {
      node = node->r;
    } else if (x <= node->l->sinfo.right) {
      node = node->l;
    } else {
      return nullptr;
    }
  }
  return nullptr;
}
}  // namespace st
}  // namespace ds
