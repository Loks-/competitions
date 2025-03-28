#pragma once

#include "common/base.h"

namespace bst {
namespace ext {
namespace base {
template <class TNode>
inline void RemoveRightSkipUpdate(TNode* root, TNode*& removed_node,
                                  TNode*& old_parent) {
  assert(root && root->r);
  for (root->r->ApplyAction(); root->r->r; root->r->ApplyAction())
    root = root->r;
  removed_node = root->r;
  root->set_right(removed_node->l);
  removed_node->ResetLinksAndUpdateInfo();
  old_parent = root;
}
}  // namespace base
}  // namespace ext
}  // namespace bst
