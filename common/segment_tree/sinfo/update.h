#pragma once

#include "common/base.h"

namespace st {
namespace sinfo {
template <class TNode>
inline void SUpdate(TNode* node) {
  assert(node);
  if (node->IsLeaf())
    node->sinfo.UpdateData(node->GetData());
  else
    node->sinfo.UpdateLR(node->l->sinfo, node->r->sinfo);
}
}  // namespace sinfo
}  // namespace st
