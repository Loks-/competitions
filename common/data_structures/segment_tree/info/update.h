#pragma once

#include "common/base.h"

namespace ds {
namespace st {
namespace info {
template <class TNode>
inline void Update(TNode* node) {
  assert(node);
  node->ApplyAction();
  if (node->IsLeaf())
    node->info.UpdateData(node->GetData(), node->sinfo);
  else
    node->info.UpdateLR(node->l->info, node->r->info, node->sinfo);
}
}  // namespace info
}  // namespace st
}  // namespace ds
