#pragma once

#include "common/base.h"

namespace st {
namespace info {
template <class TNode>
inline void Update(TNode* node) {
  assert(node);
  node->ApplyAction();
  if (node->IsLeaf())
    node->info.UpdateData(node->GetData());
  else
    node->info.UpdateLR(node->l->info, node->r->info);
}
}  // namespace info
}  // namespace st
