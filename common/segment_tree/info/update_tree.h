#pragma once

#include "common/base.h"

namespace st {
namespace info {
template <class TNode>
inline void UpdateTree(TNode* node) {
  assert(node);
  if (node->IsLeaf()) {
    node->info.UpdateData(node->GetData());
  } else {
    UpdateTree(node->l);
    UpdateTree(node->r);
    node->info.UpdateLR(node->l->info, node->r->info);
  }
}
}  // namespace info
}  // namespace st
