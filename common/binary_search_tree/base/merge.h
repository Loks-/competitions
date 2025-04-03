#pragma once

#include "common/binary_search_tree/subtree_data/size.h"

#include <utility>

namespace bst {
namespace base {

template <typename TreeType, typename TNode>
[[nodiscard]] static constexpr TNode* merge(TNode* root1, TNode* root2) {
  static_assert(TNode::has_key, "has_key should be true");
  if (!root1) return root2;
  if (!root2) return root1;
  if constexpr (TNode::SubtreeDataType::has_size) {
    if (bst::subtree_data::size(root1) < bst::subtree_data::size(root2))
      std::swap(root1, root2);
  }
  root1->apply_deferred();
  NodeType *r1l = root1->left, *r1r = root1->right, *r2l = nullptr,
           *r2r = nullptr;
  if (r1l) r1l->set_parent(nullptr);
  if (r1r) r1r->set_parent(nullptr);
  root1->set_left(nullptr);
  root1->set_right(nullptr);
  TreeType::split(root2, root1->key, r2l, r2r);
  return TreeType::join3(merge<TreeType, TNode>(r1l, r2l), root1,
                         merge<TreeType, TNode>(r1r, r2r));
}

}  // namespace base
}  // namespace bst
