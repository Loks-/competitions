#pragma once

#include "common/base.h"

namespace ds {
namespace st {
namespace info {
template <class TNode>
inline void UpdateTree(TNode* node) {
  assert(node);
  node->ApplyAction();
  if (node->IsLeaf()) {
    node->info.UpdateData(node->GetData(), node->sinfo);
  } else {
    UpdateTree(node->l);
    UpdateTree(node->r);
    node->info.UpdateLR(node->l->info, node->r->info, node->sinfo);
  }
}
}  // namespace info
}  // namespace st
}  // namespace ds
