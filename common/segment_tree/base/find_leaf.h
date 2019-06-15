#pragma once

namespace st {
template <class TNode>
inline TNode* FindLeaf(TNode* root,
                       const typename TNode::TInfo::TCoordinate& x) {
  using TInfo = typename TNode::TInfo;
  static_assert(TInfo::has_coordinate, "has_coordinate should be true");
  for (TNode* node = root; node != 0;) {
    node->ApplyAction();
    if (node->IsLeaf()) {
      if ((node->info.left == x) && (node->info.right == x))
        return node;
      else
        return 0;
    } else if (node->r->info.left <= x) {
      node = node->r;
    } else if (x <= node->l->info.right) {
      node = node->l;
    } else {
      return 0;
    }
  }
  return 0;
}
}  // namespace st
